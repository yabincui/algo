// 2.19 Find the majority element appears > n/2 times in an array.

#include <string.h>

#include "logging.h"
#include "utils.h"

int* initData(int n, int special) {
	int* s = (int*)calloc(sizeof(int), n);
	CHECK(s);
	for (int i = 0; i < special; ++i) {
		s[i] = 0;
	}
	for (int i = special; i < n; ++i) {
		s[i] = randInt(1, 10);
	}
	return s;
}

int findMajorByDivide(int* data, int n) {
	int* help = (int*)calloc(sizeof(int), n);
	CHECK(help);
	memcpy(help, data, n * sizeof(int));
	int size = n;
	while (size > 1) {
		int j = 0;
		for (int i = 0; i < size; i += 2) {
			if (i + 1 == size || help[i] == help[i + 1]) {
				help[j++] = help[i];
			}
		}
		size = j;
	}
	int count = 0;
	for (int i = 0; i < n; ++i) {
		if (help[0] == data[i]) {
			count++;
		}
	}

	free(help);
	if (count > n/2) {
		return help[0];
	}
	return -1;
}

int findMajorLinear(int* s, int n) {
	if (n < 1) return -1;
	int value = 0;
	int repeat = 0;
	for (int i = 0; i < n; ++i) {
		if (repeat == 0) {
			value = s[i];
			repeat = 1;
		} else if (value != s[i]) {
			repeat--;
		} else {
			repeat++;
		}
	}
	if (repeat) {
		int count = 0;
		for (int i = 0; i < n; ++i) {
			if (value == s[i]) {
				count++;
			}
		}
		if (count > n / 2) {
			return value;
		}
	}
	return -1;
}

int checkMajor(int* s, int n) {
	int used[11];
	memset(used, 0, sizeof(used));
	for (int i = 0; i < n; ++i) {
		used[s[i]]++;
	}
	for (int i = 0; i < 11; ++i) {
		if (used[i] > n / 2) {
			return i;
		}
	}
	return -1;
}

void testA() {
	for (int test = 0; test < 100; ++test) {
		int n = 1000;
		int special = (test & 1) ? 0 : n / 2 + 1;
		int* data = initData(n, special);
		int res1 = findMajorByDivide(data, n);
		int res2 = findMajorLinear(data, n);
		int res = checkMajor(data, n);
		CHECK(res == res1);
		CHECK(res == res2);
		free(data);
	}
}

int main() {
	testA();
	return 0;
}
