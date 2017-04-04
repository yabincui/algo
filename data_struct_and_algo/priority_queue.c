#include "priority_queue.h"

#include <stdlib.h>
#include <string.h>

#include "logging.h"
#include "utils.h"

struct priority_queue {
  int elem_size;
  char* s;
  int size;
  int capacity;
  bool (*compare)(const void*, const void*);
};

priority_queue_t createPriorityQueue(int elem_size, bool (*compare)(const void*, const void*)) {
  priority_queue_t q = (priority_queue_t)calloc(1, sizeof(struct priority_queue));
  if (q == NULL) return q;
  CHECK(elem_size > 0);
  q->elem_size = elem_size;
  q->s = NULL;
  q->size = 0;
  q->capacity = 0;
  q->compare = compare;
  return q;
}

static bool priorityQueueAdjustCapacity(priority_queue_t q, int new_capacity) {
  new_capacity = maxInt(new_capacity, maxInt(q->size, 16));
  char* s = (char*)calloc(1, q->elem_size * new_capacity);
  if (s == NULL) {
    return false;
  }
  if (q->size > 0) {
    memcpy(s, q->s, q->elem_size * q->size);
    free(q->s);
  }
  q->s = s;
  q->capacity = new_capacity;
  return true;
}

static bool compare(priority_queue_t q, int i, int j) {
  return q->compare(q->s + q->elem_size * i, q->s + q->elem_size * j);
}

static void tearUp(priority_queue_t q, int i) {
  int cur = i;
  while (cur != 0) {
    int parent = (cur - 1) / 2;
    if (compare(q, cur, parent)) {
      swap(q->s, cur, parent, q->elem_size);
      cur = parent;
    } else {
      break;
    }
  }
}

static void tearDown(priority_queue_t q, int i) {
  int cur = i;
  while (cur * 2 + 1 < q->size) {
    int left_child = cur * 2 + 1;
    int right_child = cur * 2 + 2;
    if (right_child < q->size && compare(q, right_child, left_child)) {
      left_child = right_child;
    }
    if (compare(q, left_child, cur)) {
      swap(q->s, left_child, cur, q->elem_size);
      cur = left_child;
    } else {
      break;
    }
  }
}

bool priorityQueuePush(priority_queue_t q, void* n) {
  if (q->size == q->capacity) {
    if (!priorityQueueAdjustCapacity(q, q->capacity * 2)) {
      return false;
    }
  }
  memcpy(q->s + q->elem_size * q->size, n, q->elem_size);
  q->size++;
  tearUp(q, q->size - 1);
  return true;
}

bool priorityQueuePop(priority_queue_t q, void* n) {
  if (q->size == 0) return false;
  memcpy(n, q->s, q->elem_size);
  q->size--;
  if (q->size > 0) {
    memcpy(q->s, q->s + q->elem_size * q->size, q->elem_size);
    tearDown(q, 0);
  }
  if (q->size * 4 <= q->capacity) {
    priorityQueueAdjustCapacity(q, q->capacity / 2);
  }
  return true;
}

bool priorityQueueTop(priority_queue_t q, void* n) {
  if (q->size == 0) return false;
  memcpy(n, q->s, q->elem_size);
  return true;
}

bool priorityQueueEmpty(priority_queue_t q) {
  return q->size == 0;
}

int priorityQueueSize(priority_queue_t q) {
  return q->size;
}

void destroyPriorityQueue(priority_queue_t q) {
  if (q->s) free(q->s);
  free(q);
}
