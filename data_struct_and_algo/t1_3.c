// 1.3 Write a procedure to output an arbitrary real number (which might be negative)
// using only print_digit for IO.

#include <stdio.h>

static void print_digit(int i) {
	fputc(i + '0', stdout);
}

static void printNumber(double value) {
	if (value < 0) {
		fputc('-', stdout);
		value = -value;
	}
	double tmp = 1;
	while (tmp * 10 < value) {
		tmp *= 10;
	}
	do {
		if (tmp == 0.1) {
			fputc('.', stdout);
		}
		int t = (int)(value / tmp);
		print_digit(t);
		value -= t * tmp;
		tmp /= 10;
	} while (value > 1e-6);
	fputc('\n', stdout);
}

int main() {
	printNumber(-123.45);
	printNumber(0);
	printNumber(32675.7645);
	return 0;
}
