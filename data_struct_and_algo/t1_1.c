// 1.1 Write a program to solve the selection problem. Let k=n/2. Draw a table showing the runtime
// of you program for various values of n.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "priority_queue.h"
#include "timer.h"
#include "utils.h"

struct SelectData {
  int* s;
  int n;
  int k;
  int res;
};

typedef void (*SelectFuncType)(void*);

static int compareInt(const void* a1, const void* a2) {
  const int* v1 = (const int*)a1;
  const int* v2 = (const int*)a2;
  if (*v1 != *v2) {
    return (*v1 > *v2) ? 1 : -1;
  }
  return 0;
}

void findKthBySort(void* arg) {
  struct SelectData* data = (struct SelectData*)arg;
  int* s = data->s;
  int n = data->n;
  int k = data->k;
  if (k <= 0 || k > n) {
    data->res = -1;
    return;
  }
  qsort(s, n, sizeof(int), compareInt);
  data->res = s[k-1];
}

void findKthBySelect(void* arg) {
  struct SelectData* data = (struct SelectData*)arg;
  int* s = data->s;
  int n = data->n;
  int k = data->k;
  if (k <= 0 || k > n) {
    data->res = -1;
    return;
  }
  int buf[k + 1];
  int buf_size = 0;
  for (int i = 0; i < n; ++i) {
    int j = buf_size;
    while (j > 0 && buf[j-1] > s[i]) {
      buf[j] = buf[j-1];
      j--;
    }
    buf[j] = s[i];
    if (buf_size < k) {
      buf_size++;
    }
  }
  data->res = buf[k-1];
}


bool compareIntReverse(const void* a1, const void* a2) {
  int v1 = *(const int*)a1;
  int v2 = *(const int*)a2;
  return v1 > v2;
}

void findKthByPriorityQueue(void* arg) {
  struct SelectData* data = (struct SelectData*)arg;
  int* s = data->s;
  int n = data->n;
  int k = data->k;
  if (k <= 0 || k > n) {
    data->res = -1;
    return;
  }
  priority_queue_t q = createPriorityQueue(sizeof(int), compareIntReverse);
  for (int i = 0; i < n; ++i) {
    if (priorityQueueSize(q) < k) {
      priorityQueuePush(q, &s[i]);
    } else {
      int v;
      priorityQueueTop(q, &v);
      if (s[i] < v) {
        priorityQueuePush(q, &s[i]);
        priorityQueuePop(q, &v);
      }
    }
  }
  priorityQueuePop(q, &data->res);
  destroyPriorityQueue(q);
}

void findKthByBinarySearch(void* arg) {
  struct SelectData* data = (struct SelectData*)arg;
  int* s = data->s;
  int n = data->n;
  int k = data->k;
  if (k <= 0 || k > n) {
    data->res = -1;
    return;
  }
  k--;
  while (n != 1) {
    int left = 0;
    int right = n - 1;
    int mid = left + (right - left) / 2;
    int key = s[mid];
    swap(s, mid, right, sizeof(int));
    right--;
    while (left <= right) {
      while (left <= right && s[left] < key) left++;
      while (left <= right && s[right] > key) right--;
      if (left < right) {
        swap(s, left, right, sizeof(int));
      }
    }
    if (left == k) {
      data->res = key;
      return;
    }
    if (left > k) {
      n = left;
    } else {
      s += left;
      n -= left + 1;
      k -= left + 1;
    }
  }
  data->res = s[0];
}

struct TestMethod {
  const char* name;
  SelectFuncType func;
} table[] = {
  { "sort", &findKthBySort},
  { "select", &findKthBySelect},
  { "priority_queue", &findKthByPriorityQueue},
  { "binary_search", &findKthByBinarySearch},
};

int main() {
  static int s[1000000];
  static int tmp[1000000];
  for (int i = 0; i < sizeof(s) / sizeof(s[0]); ++i) {
    s[i] = i+1;
  }
  int table_size = sizeof(table) / sizeof(table[0]);
  printf("n");
  for (int i = 0; i < table_size; ++i) {
    printf("\t%s", table[i].name);
  }
  printf("\n");
  for (int n = 10; n <= 1000000; n *= 2) {
    shuffle(s, n, sizeof(int));
    double times[table_size];
    for (int i = 0; i < table_size; ++i) {
      memcpy(tmp, s, n * sizeof(int));
      struct SelectData data;
      data.s = tmp;
      data.n = n;
      data.k = n / 2;
      data.res = 0;
      double time = runMethodWithTimeout(5000, table[i].func, &data);
      if (time > 0) {
        if (data.res != n / 2) {
          time = -2;
        }
      }
      times[i] = time;
    }
    printf("%d", n);
    for (int i = 0; i < table_size; ++i) {
      printf("\t%.6fs", times[i]);
    }
    printf("\n");
  }
}
