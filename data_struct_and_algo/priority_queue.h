#ifndef _DS_PRIORITY_QUEUE_H
#define _DS_PRIORITY_QUEUE_H

#include <stdbool.h>

// This is a min heap. The minimum value is at the top of the heap.
typedef struct priority_queue* priority_queue_t;

priority_queue_t createPriorityQueue(int elem_size, bool (*compare)(const void*, const void*));

bool priorityQueuePush(priority_queue_t q, void* n);
bool priorityQueuePop(priority_queue_t q, void* n);
bool priorityQueueTop(priority_queue_t q, void* n);
bool priorityQueueEmpty(priority_queue_t q);
int priorityQueueSize(priority_queue_t q);

void destroyPriorityQueue(priority_queue_t q);

#endif  // _DS_PRIORITY_QUEUE_H
