// 2.7 Generate a random permutation of n integers, given rand_int(i, j).
// Try three algorithms:
//  1) To fill a[i], generate random numbers until you get one that is not
//     already in a[0..i-1].
//  2) Keep an used[i] array.
//  3) Use random swap.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logging.h"
#include "timer.h"
#include "utils.h"

struct GenData {
  int* s;
  int n;
  int64_t res;
};

typedef void (*GenFuncType)(void*);

void genByCheck(void* arg) {
	struct GenData* data = (struct GenData*)arg;
	int* s = data->s;
	int n = data->n;

	for (int i = 0; i < n; ++i) {
		while (true) {
			int tmp = randInt(0, n - 1);
			int j;
			for (j = 0; j < i; ++j) {
				if (tmp == s[j]) {
					break;
				}
			}
			if (j == i) {
				s[i] = tmp;
				break;
			}
		}
	}

	data->res = 0;
	for (int i = 0; i < n; ++i) {
		data->res += s[i];
	}
}

void genByUsed(void* arg) {
	struct GenData* data = (struct GenData*)arg;
	int* s = data->s;
	int n = data->n;

	int* used = (int*)calloc(sizeof(int), n);

	for (int i = 0; i < n; ++i) {
		while (true) {
			int tmp = randInt(0, n - 1);
			if (!used[tmp]) {
				used[tmp] = 1;
				s[i] = tmp;
				break;
			}
		}
	}

	data->res = 0;
	for (int i = 0; i < n; ++i) {
		data->res += s[i];
	}
	free(used);
}

void genBySwap(void* arg) {
	struct GenData* data = (struct GenData*)arg;
	int* s = data->s;
	int n = data->n;

	for (int i = 0; i < n; ++i) {
		s[i] = i;
		int j = randInt(0, i);
		swap(s, i, j, sizeof(int));
	}

	data->res = 0;
	for (int i = 0; i < n; ++i) {
		data->res += s[i];
	}
}

struct TestMethod {
  const char* name;
  GenFuncType func;
} table[] = {
  { "check_all", &genByCheck},
  { "used", &genByUsed},
  { "swap", &genBySwap},
};

int main() {
  static int s[1000000];
  int table_size = sizeof(table) / sizeof(table[0]);
  printf("n");
  for (int i = 0; i < table_size; ++i) {
    printf("\t%s", table[i].name);
  }
  printf("\n");
  for (int n = 10; n <= 1000000; n *= 2) {
    double times[table_size];
    for (int i = 0; i < table_size; ++i) {
      struct GenData data;
      data.s = s;
      data.n = n;
      data.res = 0;
      double time = runMethodWithTimeout(5000, table[i].func, &data);
      if (time > 0) {
        if (data.res != (int64_t)n * (n - 1) / 2) {
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
