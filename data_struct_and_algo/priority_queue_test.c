#include "priority_queue.h"

#include <stdio.h>

#include "logging.h"

bool compareNormal(const void* a1, const void* a2) {
  int v1 = *(const int*)a1;
  int v2 = *(const int*)a2;
  return v1 < v2;
}

bool compareReverse(const void* a1, const void* a2) {
  int v1 = *(const int*)a1;
  int v2 = *(const int*)a2;
  return v1 > v2;
}

int main() {
  priority_queue_t q;
  q = createPriorityQueue(sizeof(int), compareNormal);
  for (int i = 0; i < 100; ++i) {
    CHECK(priorityQueuePush(q, &i));
  }
  for (int i = 0; i < 100; ++i) {
    int n;
    CHECK(priorityQueuePop(q, &n));
    CHECK(n == i);
  }
  destroyPriorityQueue(q);

  q = createPriorityQueue(sizeof(int), compareReverse);
  for (int i = 0; i < 100; ++i) {
    CHECK(priorityQueuePush(q, &i));
  }
  for (int i = 0; i < 100; ++i) {
    int n;
    CHECK(priorityQueuePop(q, &n));
    CHECK(n == 99 - i);
  }
  destroyPriorityQueue(q);
}
