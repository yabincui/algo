// 2.12
//  a. Find the minimum subsequence sum
//  b. Find the minimum positive subsequence sum
//  c. Find the maximum subsequence product

#include <limits.h>
#include <math.h>
#include <stdlib.h>

#include "logging.h"
#include "utils.h"

static int* randomArray(int n) {
	int* s = (int*)calloc(sizeof(int), n);
	CHECK(s);
	for (int i = 0; i < n; ++i) {
		s[i] = randInt(-10, 10);
	}
	return s;
}

int findMinSubseqSum(int* s, int n) {
	int min_sum = 0;
	int sum = 0;
	for (int i = 0; i < n; ++i) {
		if (sum > 0) {
			sum = 0;
		}
		sum += s[i];
		if (sum < min_sum) {
			min_sum = sum;
		}
	}
	return min_sum;
}

int checkMinSubseqSum(int* s, int n) {
	int min_sum = 0;
	for (int i = 0; i < n; ++i) {
		int sum = 0;
		for (int j = i; j < n; ++j) {
			sum += s[j];
			if (sum < min_sum) {
				min_sum = sum;
			}
		}
	}
	return min_sum;
}

void testA() {
	for (int test = 0; test < 100; ++test) {
		int n = 1000;
		int* s = randomArray(n);
		int sum = findMinSubseqSum(s, n);
		int check_sum = checkMinSubseqSum(s, n);
		CHECK(sum == check_sum);
		free(s);
	}
}

int findMinPositiveSubseqSum(int* s, int n) {
	int min_sum = INT_MAX;
	if (n == 0) {
		return min_sum;
	}
	if (n == 1) {
		if (s[0] > 0) {
			min_sum = s[0];
		}
		return min_sum;
	}
	int mid = n / 2;
	static int tmp[1000000];
	int size = n - mid - 1;
	int sum = 0;
	for (int i = 0; i < size; ++i) {
		sum += s[i + mid + 1];
		tmp[i] = sum;
	}
	if (size > 0) {
		qsort(tmp, size, sizeof(int), cmpInt);
	}
	int left_sum = 0;
	for (int i = mid; i >= 0; --i) {
		left_sum += s[i];
		if (left_sum > 0 && left_sum < min_sum) {
			min_sum = left_sum;
		}
		int ask = -left_sum + 1;
		int* p = (int*)bsearchLowBound(tmp, size, sizeof(int), &ask, cmpInt);
		if (p) {
			int sum = *p + left_sum;
			if (sum > 0 && sum < min_sum) {
				min_sum = sum;
			}
		}
	}
	int left_min = findMinPositiveSubseqSum(s, mid);
	int right_min = findMinPositiveSubseqSum(s + mid + 1, size);
	if (left_min != INT_MAX && left_min < min_sum) {
		min_sum = left_min;
	}
	if (right_min != INT_MAX && right_min < min_sum) {
		min_sum = right_min;
	}
	return min_sum;
}

int checkMinPositiveSubseqSum(int* s, int n) {
	int min_sum = INT_MAX;
	for (int i = 0; i < n; ++i) {
		int sum = 0;
		for (int j = i; j < n; ++j) {
			sum += s[j];
			if (sum < min_sum && sum > 0) {
				min_sum = sum;
			}
		}
	}
	return min_sum;
}

void testB() {
	for (int test = 0; test < 100; ++test) {
		int n = 1000;
		int* s = randomArray(n);
		int sum = findMinPositiveSubseqSum(s, n);
		int check_sum = checkMinPositiveSubseqSum(s, n);
		CHECK(sum == check_sum);
		free(s);
	}
}

double findMaxSubseqProduct(int* s, int n) {
	double res = 0.0;
	double max_pro = 1;
	double min_pro = 1;
	for (int i = 0; i < n; ++i) {
		double t1 = max_pro * s[i];
		double t2 = min_pro * s[i];
		double t3 = s[i];
		max_pro = max(max(t1, t2), t3);
		min_pro = min(min(t1, t2), t3);
		if (res < max_pro) {
			res = max_pro;
		}
	}
	return res;
}

double checkMaxSubseqProduct(int* s, int n) {
	double res = 0.0;
	for (int i = 0; i < n; ++i) {
		double pro = 1.0;
		for (int j = i; j < n; ++j) {
			pro *= s[j];
			if (pro > res) {
				res = pro;
			}
		}
	}
	return res;
}

void testC() {
	for (int test = 0; test < 100; ++test) {
		int n = 100;
		int* s = randomArray(n);
		double sum = findMaxSubseqProduct(s, n);
		double check_sum = checkMaxSubseqProduct(s, n);
		CHECK(fabs(sum - check_sum) < 1e-6);
		free(s);
	}
}


int main() {
	testA();
	testB();
	testC();

	return 0;
}
