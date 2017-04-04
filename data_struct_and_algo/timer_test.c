#include "timer.h"

#include <stdio.h>
#include <unistd.h>

#include "logging.h"
#include "utils.h"

void timerFunc(void* arg) {
  int* counter = (int*)arg;
  printf("timerFunc\n");
  if (*counter < 10) {
    ++*counter;
  }
}

void printHello(void* arg) {
  printf("hello\n");
}

void busyLoop(void* arg) {
  printf("busyLoop\n");
  while (1);
}

int main() {
  struct timer* timer;
  int counter = 0;
  CHECK(timer = createTimer(TIMER_OPTION_ONE_TIME, 1000, timerFunc, &counter));
  sleepFully(11);
  CHECK(counter == 1);
  CHECK(destroyTimer(timer));
  counter = 0;
  CHECK(timer = createTimer(TIMER_OPTION_PERIODIC, 1000, timerFunc, &counter));
  sleepFully(11);
  CHECK(counter == 10);
  CHECK(destroyTimer(timer));

  double ret = runMethodWithTimeout(1000, printHello, NULL);
  printf("runMethodWithTimeout(printHello) = %f\n", ret);
  ret = runMethodWithTimeout(1000, busyLoop, NULL);
  printf("runMethodWithTimeout(busyLoop) = %f\n", ret);
  return 0;
}
