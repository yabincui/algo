#ifndef _DS_UTILS_H
#define _DS_UTILS_H

double getTime();
void sleepFully(double seconds);


int rand32();
void shuffle(void* s, int n, int elem_size);
void swap(void* s, int i, int j, int elem_size);

#endif  // _DS_UTILS_H
