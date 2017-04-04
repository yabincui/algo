#ifndef _DS_TIMER_H
#define _DS_TIMER_H

#include <stdbool.h>

// The implementation of timer module uses SIGALRM signal. So make sure you don't use
// SIGALARM in other parts of your program.
struct timer;

enum {
  TIMER_OPTION_ONE_TIME = 0,
  TIMER_OPTION_PERIODIC = 1,
};

struct timer* createTimer(int options, int interval_in_ms,
                          void (*callback)(void*), void* callback_arg);

bool destroyTimer(struct timer* timer);

// If you allocate any resource in method(method_arg), please record them in method_arg,
// and free them afterward.
// Return value of runMethodWithTimeout().
// -1 if timeout. -2 if having internal error. otherwise time in second.
double runMethodWithTimeout(int timeout_in_ms, void (*method)(void*), void* method_arg);

#endif  // _DS_TIMER_H
