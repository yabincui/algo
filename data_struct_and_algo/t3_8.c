// 3.8 Write a function to take a polynomial f(x), and compute f(x)^n.
// Use a linked list implementation.

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

Polynomial* addValue(Polynomial* l, struct Value v) {
	Polynomial** pp = &l;
	while (*pp && (*pp)->value.power > v.power) {
		pp = &(*pp)->next;
	}
	if (!*pp || (*pp)->value.power < v.power) {
		Polynomial* p = PolynomialAlloc();
		p->value = v;
		p->next = *pp;
		*pp = p;
	} else {
		(*pp)->value.coeff += v.coeff;
	}
	return l;
}

Polynomial* clearPolynomial(Polynomial* a) {
	Polynomial** pp = &a;
	while (*pp) {
		if ((*pp)->value.coeff == 0) {
			Polynomial* tmp = *pp;
			*pp = tmp->next;
			free(tmp);
		} else {
			pp = &(*pp)->next;
		}
	}
	return a;
}

// O(M*N*(M*N))
Polynomial* mulPolynomial(const Polynomial* a, const Polynomial* b) {
	Polynomial* res = NULL;
	for (const Polynomial* i = a; i != NULL; i = i->next) {
		for (const Polynomial* j = b; j != NULL; j = j->next) {
			struct Value v;
			v.coeff = i->value.coeff * j->value.coeff;
			v.power = i->value.power + j->value.power;
			res = addValue(res, v);
		}
	}
	res = clearPolynomial(res);
	return res;
}

int lenPolynomial(const Polynomial* a) {
	int n = 0;
	while (a) {
		n++;
		a = a->next;
	}
	return n;
}

// O(M^2 * N), M is smaller.
Polynomial* mulPolynomial2(const Polynomial* a, const Polynomial* b) {
	Polynomial* res = NULL;
	if (lenPolynomial(a) > lenPolynomial(b)) {
		swapTwo(&a, &b, sizeof(Polynomial*));
	}
	Polynomial* tmp = NULL;
	while (a) {
		Polynomial** pp = &tmp;
		for (const Polynomial* i = b; i != NULL; i = i->next) {
			if (!*pp) {
				*pp = PolynomialAlloc();
			}
			(*pp)->value.coeff = i->value.coeff * a->value.coeff;
			(*pp)->value.power = i->value.power + a->value.power;
			pp = &(*pp)->next;
		}
		res = addPolynomial(res, tmp);
		a = a->next;
	}
	PolynomialFree(tmp);
	return res;
}

// The input a, b can't be used after the function call.
Polynomial* mergePolynomial(Polynomial* a, Polynomial* b) {
	Polynomial* res = NULL;
	Polynomial** pp = &res;
	while (a || b) {
		if (a && (!b || a->value.power > b->value.power)) {
			*pp = a;
			a = a->next;
		} else if (b && (!a || b->value.power > a->value.power)) {
			*pp = b;
			b = b->next;
		} else {
			*pp = b;
			(*pp)->value.coeff += a->value.coeff;
			Polynomial* tmp = a;
			a = a->next;
			b = b->next;
			free(tmp);
		}
		pp = &(*pp)->next;
	}
	return res;
}

// O(M*N*log(MN)), produce M polynomials, and merge the polynomials.
Polynomial* mulPolynomial3(const Polynomial* a, const Polynomial* b) {
	Polynomial* res = NULL;
	int m = lenPolynomial(a);
	int n = lenPolynomial(b);
	if (m > n) {
		swapTwo(&a, &b, sizeof(Polynomial*));
		swapTwo(&m, &n, sizeof(m));
	}
	Polynomial** s = (Polynomial**)calloc(sizeof(Polynomial*), m);
	for (int i = 0; i < m; ++i) {
		Polynomial** pp = &s[i];
		for (const Polynomial* i = b; i != NULL; i = i->next) {
			if (!*pp) {
				*pp = PolynomialAlloc();
			}
			(*pp)->value.coeff = i->value.coeff * a->value.coeff;
			(*pp)->value.power = i->value.power + a->value.power;
			pp = &(*pp)->next;
		}
		a = a->next;
	}
	int count = m;
	while (count > 1) {
		int j = 0;
		for (int i = 0; i < count; i += 2) {
			if (i + 1 < count) {
				s[j++] = mergePolynomial(s[i], s[i+1]);
			} else {
				s[j++] = s[i];
			}
		}
		count = j;
	}
	res = s[0];
	free(s);
	return clearPolynomial(res);
}

Polynomial* mergeSortPolynomial(Polynomial* a, int n) {
	if (n <= 1) {
		return a;
	}
	int left_count = n / 2;
	Polynomial* p = a;
	for (int i = 1; i < left_count; ++i) {
		p = p->next;
	}
	Polynomial* right = p->next;
	p->next = NULL;
	Polynomial* left = mergeSortPolynomial(a, left_count);
	right = mergeSortPolynomial(right, n - left_count);
	return mergePolynomial(left, right);
}

// O(M*N*log(MN)), add each multiply entry into a Polynomial, then
// use merge sort to sort the Polynomial.
Polynomial* mulPolynomial4(const Polynomial* a, const Polynomial* b) {
	Polynomial* res = NULL;
	Polynomial** pp = &res;
	while (a) {
		for (const Polynomial* i = b; i != NULL; i = i->next) {
			*pp = PolynomialAlloc();
			(*pp)->value.coeff = i->value.coeff * a->value.coeff;
			(*pp)->value.power = i->value.power + a->value.power;
			pp = &(*pp)->next;
		}
		a = a->next;
	}
	res = mergeSortPolynomial(res, lenPolynomial(res));
	return clearPolynomial(res);
}

Polynomial* copyPolynomial(const Polynomial* a) {
	Polynomial* res = NULL;
	Polynomial** pp = &res;
	while (a) {
		*pp = PolynomialAlloc();
		(*pp)->value = a->value;
		pp = &(*pp)->next;
		a = a->next;
	}
	return res;
}

Polynomial* powerPolynomial(const Polynomial* x, int n) {
	Polynomial* res = PolynomialAlloc();
	res->value.coeff = 1;
	res->value.power = 0;
	bool first = true;
	while (n) {
		if (n & 1) {
			Polynomial* tmp = mulPolynomial(res, x);
			PolynomialFree(res);
			res = tmp;
		}
		n /= 2;
		if (n) {
			Polynomial* tmp = mulPolynomial(x, x);
			if (first) {
				first = false;
			} else {
				PolynomialFree((Polynomial*)x);
			}
			x = tmp;
		}
	}
	return res;
}


int main() {
	struct Value a[] = {
			{7, 5}, {-2, 3}, {1, 0},
	};
	Polynomial* pa = PolynomialCreateFromArray(a, sizeof(a) / sizeof(a[0]));
	Polynomial* res = powerPolynomial(pa, 3);
	printList(pa);
	printList(res);
	PolynomialFree(pa);
	PolynomialFree(res);
	return 0;
}
