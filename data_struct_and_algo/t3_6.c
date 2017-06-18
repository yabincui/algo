// 3.6 Write a function to add two polynomials. Use a linked list implementation.

#include <stdbool.h>

#include "list.h"
#include "logging.h"
#include "utils.h"

struct Value {
	int coeff;
	int power;
};

DEFINE_LIST_TYPE(Polynomial, struct Value, value);


void printList(Polynomial* a) {
	printf("[");
	bool first = true;
	while (a) {
		if (first) {
			first = false;
		} else {
			printf(" + ");
		}
		printf("%d * x^(%d)", a->value.coeff, a->value.power);
		a = a->next;
	}
	printf("]\n");
}

Polynomial* addPolynomial(const Polynomial* a, const Polynomial* b) {
	Polynomial* res = NULL;
	Polynomial** pp = &res;
	while (a || b) {
		struct Value v;
		if (a && (!b || a->value.power > b->value.power)) {
			v = a->value;
			a = a->next;
		} else if (b && (!a || b->value.power > a->value.power)) {
			v = b->value;
			b = b->next;
		} else {
			v = a->value;
			v.coeff += b->value.coeff;
			a = a->next;
			b = b->next;
		}
		if (v.coeff != 0) {
			(*pp) = PolynomialAlloc();
			(*pp)->value = v;
			pp = &(*pp)->next;
		}
	}
	return res;
}

int main() {
	struct Value a[] = {
			{7, 5}, {-2, 3}, {1, 0},
	};
	struct Value b[] = {
			{3, 6}, {8, 3}, {2, 2}, {4, 0},
	};
	Polynomial* pa = PolynomialCreateFromArray(a, sizeof(a) / sizeof(a[0]));
	Polynomial* pb = PolynomialCreateFromArray(b, sizeof(b) / sizeof(b[0]));
	Polynomial* res = addPolynomial(pa, pb);
	printList(pa);
	printList(pb);
	printList(res);
	PolynomialFree(pa);
	PolynomialFree(pb);
	PolynomialFree(res);
	return 0;
}
