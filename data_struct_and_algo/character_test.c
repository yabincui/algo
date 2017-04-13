#include <stdio.h>

int main() {
	for (int i = 0; i < 256; ++i) {
		printf("%d %c\n", i, (char)i);
	}
	printf("\u263A\n");
	printf("\u2764\n");
	return 0;
}
