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

int randInt(int begin, int end) {
	return begin + rand32() % (end - begin + 1);
}

void shuffle(void* s, int n, int elem_size) {
  for (int i = 0; i < n; ++i) {
    int t = i + rand32() % (n - i);
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

void swapTwo(void* a, void* b, int size) {
	char tmp[size];
	memcpy(tmp, a, size);
	memcpy(a, b, size);
	memcpy(b, tmp, size);
}

int maxInt(int a, int b) {
  return a < b ? b : a;
}

int cmpInt(const void* a, const void* b) {
	int v1 = *(int*)a;
	int v2 = *(int*)b;
	if (v1 != v2) {
		return v1 > v2 ? 1 : -1;
	}
	return 0;
}

const void* bsearchLowBound(const void* s, int nmemb, int elem_size, const void* key,
		int (*cmp)(const void*, const void*)) {
	int low = 0;
	int high = nmemb - 1;
	while (low < high) {
		int mid = low + (high - low) / 2;
		int cmp_res = cmp(s + mid * elem_size, key);
		if (cmp_res < 0) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	if (cmp(s + high * elem_size, key) >= 0) {
		return s + high * elem_size;
	}
	return NULL;
}
