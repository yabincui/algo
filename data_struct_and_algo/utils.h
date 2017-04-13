#ifndef _DS_UTILS_H
#define _DS_UTILS_H

double getTime();
void sleepFully(double seconds);


int rand32();
int randInt(int begin, int end);
void shuffle(void* s, int n, int elem_size);
void swap(void* s, int i, int j, int elem_size);
void swapTwo(void* a, void* b, int size);

int maxInt(int a, int b);

int cmpInt(const void* a, const void* b);
const void* bsearchLowBound(const void* s, int nmemb, int elem_size, const void* key,
		int (*cmp)(const void*, const void*));

#define max(a, b) \
   ((a) > (b) ? (a) : (b))

#define min(a, b) \
	((a) < (b) ? (a) : (b))

#endif  // _DS_UTILS_H
