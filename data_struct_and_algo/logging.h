#ifndef _LOGGING_H
#define _LOGGING_H

#include <stdlib.h>
#include <stdio.h>

#define CHECK(stat) \
do {  \
  if (!(stat)) abort();    \
} while (0)


#endif // _LOGGING_H
