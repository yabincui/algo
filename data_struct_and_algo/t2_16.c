// 2.16 Write the fast exponentiation routine without recursion.

#include <math.h>
#include <limits.h>

#include "logging.h"
#include "utils.h"


double fastExp(double x, int n) {
	double res = 1.0;
	double base = x;
	while (n != 0) {
		if (n & 1) {
			res *= base;
		}
		n >>= 1;
		base *= base;
	}
	return res;
}

int main() {
	double res = fastExp(2.0, 63);
	printf("res = %f\n", res);
	return 0;
}
