// 1.8 What is 2^100 % 5.

#include "logging.h"

int main() {
	int value = 1;
	for (int i = 0; i < 100; ++i) {
		value *= 2;
		value %= 5;
	}
	printf("2^100 %% 5 = %d\n", value);
	return 0;
}
