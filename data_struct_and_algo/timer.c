#include "timer.h"

#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <syscall.h>
#include <time.h>
#include <unistd.h>

#include "logging.h"
#include "utils.h"

struct timer {
  timer_t timer_id;
  void (*callback)(void*);
  void* callback_arg;
};


void sigAlarmHandler(int signo, siginfo_t* sig_info, void* context) {
  CHECK(signo == SIGALRM);
  struct timer* timer = (struct timer*)sig_info->si_ptr;
  CHECK(timer);
  timer->callback(timer->callback_arg);
}

static void installSigAlarmHandler() {
  struct sigaction act;
  memset(&act, 0, sizeof(act));
  act.sa_sigaction = sigAlarmHandler;
  act.sa_flags = SA_SIGINFO;
  sigaction(SIGALRM, &act, NULL);
}

static pthread_once_t once_control = PTHREAD_ONCE_INIT;

struct timer* createTimer(int options, int interval_in_ms,
                          void (*callback)(void*), void* callback_arg) {
  pthread_once(&once_control, installSigAlarmHandler);
  struct timer* timer = calloc(1, sizeof(struct timer));
  if (timer == NULL) {
    return NULL;
  }
  timer->callback = callback;
  timer->callback_arg = callback_arg;
  struct sigevent sev;
  int tid = syscall(__NR_gettid);
  memset(&sev, 0, sizeof(sev));
  sev.sigev_notify = SIGEV_THREAD_ID;
  sev._sigev_un._tid = tid;
  sev.sigev_signo = SIGALRM;
  sev.sigev_value.sival_ptr = (void*)timer;
  int ret = timer_create(CLOCK_MONOTONIC, &sev, &timer->timer_id);
  if (ret < 0) {
    free(timer);
    return NULL;
  }
  struct itimerspec ts;
  memset(&ts, 0, sizeof(ts));
  ts.it_value.tv_sec = interval_in_ms / 1000;
  ts.it_value.tv_nsec = interval_in_ms % 1000 * 1000000;
  if (options & TIMER_OPTION_PERIODIC) {
    ts.it_interval = ts.it_value;
  }
  ret = timer_settime(timer->timer_id, 0, &ts, NULL);
  if (ret < 0) {
    timer_delete(timer->timer_id);
    free(timer);
    return NULL;
  }
  return timer;
}

bool destroyTimer(struct timer* timer) {
  int ret = timer_delete(timer->timer_id);
  free(timer);
  if (ret < 0) {
    return false;
  }
  return true;
}

struct TimeoutThreadData {
  int timeout_in_ms;
  void (*method)(void*);
  void* method_arg;
  bool timeout;
  double time_in_sec;
  struct timer* timer;
};

static void timeoutCallback(void* arg) {
  struct TimeoutThreadData* data = (struct TimeoutThreadData*)arg;
  destroyTimer(data->timer);
  data->timeout = true;
  pthread_exit(NULL);
}

static void* threadFunc(void* arg) {
  struct TimeoutThreadData* data = (struct TimeoutThreadData*)arg;
  data->timer = createTimer(TIMER_OPTION_ONE_TIME, data->timeout_in_ms, timeoutCallback, data);
  if (data->timer == NULL) {
    return (void*)-1;
  }

  double start_time = getTime();
  data->method(data->method_arg);
  double end_time = getTime();

  destroyTimer(data->timer);
  data->time_in_sec = end_time - start_time;
  return NULL;
}

double runMethodWithTimeout(int timeout_in_ms, void (*method)(void*), void* method_arg) {
  pthread_t thread;
  struct TimeoutThreadData data;
  data.timeout_in_ms = timeout_in_ms;
  data.method = method;
  data.method_arg = method_arg;
  data.timeout = false;
  data.time_in_sec = 0.0;
  int ret = pthread_create(&thread, NULL, threadFunc, &data);
  if (ret != 0) return -2;
  pthread_join(thread, (void*)&ret);
  if (ret != 0) return -2;
  if (data.timeout) return -1;
  return data.time_in_sec;
}
