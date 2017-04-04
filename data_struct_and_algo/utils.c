#include "utils.h"

#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "logging.h"

double getTime() {
  struct timespec ts;
  int ret = clock_gettime(CLOCK_MONOTONIC, &ts);
  CHECK(ret == 0);
  return ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

void sleepFully(double seconds) {
  double cur_time = getTime();
  double end_time = cur_time + seconds;
  while (cur_time < end_time) {
    int usec = (int)((end_time - cur_time) * 1000000 + 1);
    int ret = usleep(usec);
    if (ret < 0) {
      CHECK(errno == EINTR);
    }
    cur_time = getTime();
  }
}

int rand32() {
  int a = rand() & 0xffff;
  int b = rand() & 0x7fff;
  return (b << 16) | a;
}

void shuffle(void* s, int n, int elem_size) {
  for (int i = 0; i < n; ++i) {
    int t = rand32() % n;
    swap(s, i, t, elem_size);
  }
}

void swap(void* s, int i, int j, int elem_size) {
  char tmp[elem_size];
  char* a = s + elem_size * i;
  char* b = s + elem_size * j;
  memcpy(tmp, a, elem_size);
  memcpy(a, b, elem_size);
  memcpy(b, tmp, elem_size);
}

int maxInt(int a, int b) {
  return a < b ? b : a;
}
