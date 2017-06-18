#ifndef _LOGGING_H
#define _LOGGING_H

#include <stdlib.h>
#include <stdio.h>

#define CHECK(stat) \
do {  \
  if (!(stat)) { \
	fprintf(stderr, "%s:%d: Assertion failed: %s\n", __FILE__, __LINE__, #stat); \
	abort();    \
  } \
} while (0)


#endif // _LOGGING_H
