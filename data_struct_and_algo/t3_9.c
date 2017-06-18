// 3.9 Write a function to support arbitrary precision integer arithmetic,
// calculate the distribution of 0 - 9 in 2^4000.

#include <stdbool.h>

#include "list.h"
#include "logging.h"
#include "utils.h"

struct Value {
	int digit;
	int level;
};

DEFINE_LIST_TYPE(BigInteger, struct Value, value);


void printBigInteger(BigInteger* a) {
	BigInteger* head = BigIntegerReverse(a);
	a = head;
	int prev_level = -1;
	while (a) {
		if (prev_level > a->value.level + 1) {
			while (prev_level > a->value.level + 1) {
				printf("0");
				prev_level--;
			}
		}
		printf("%d", a->value.digit);
		prev_level = a->value.level;
		a = a->next;
	}
	printf("\n");
	BigIntegerReverse(head);
}

int lenBigInteger(BigInteger* a) {
	int n = 0;
	while (a) {
		n++;
		a = a->next;
	}
	return n;
}

BigInteger* cleanBigInteger(BigInteger* a) {
	BigInteger** pp = &a;
	int add = 0;
	int level = 0;
	while (*pp || add) {
		if (*pp) {
			int cur_level = (*pp)->value.level;
			int cur_digit = (*pp)->value.digit;
			while ((*pp)->next && (*pp)->next->value.level == cur_level) {
				cur_digit += (*pp)->next->value.digit;
				BigInteger* tmp = (*pp)->next;
				(*pp)->next = tmp->next;
				free(tmp);
			}
			(*pp)->value.digit = cur_digit;
		}
		if (add && (!*pp || level < (*pp)->value.level)) {
			if (add % 10 != 0) {
				BigInteger* tmp = BigIntegerAlloc();
				tmp->value.digit = add % 10;
				tmp->value.level = level;
				if (*pp) {
					tmp->next = *pp;
				}
				*pp = tmp;
				pp = &(tmp->next);
			}
			add /= 10;
			level++;
		} else {
			int sum = (*pp)->value.digit + add;
			(*pp)->value.digit = sum % 10;
			add = sum / 10;
			level = (*pp)->value.level + 1;
			pp = &(*pp)->next;
		}
	}
	return a;
}

BigInteger* copyBigInteger(BigInteger* a) {
	BigInteger* res = NULL;
	BigInteger** pp = &res;
	while (a) {
		*pp = BigIntegerAlloc();
		(*pp)->value = a->value;
		pp = &(*pp)->next;
		a = a->next;
	}
	return res;
}

BigInteger* mergeBigInteger(BigInteger* a, BigInteger* b) {
	BigInteger* res = NULL;
	BigInteger** pp = &res;
	BigInteger* sa = copyBigInteger(a);
	BigInteger* sb = copyBigInteger(b);
	while (a || b) {
		if (a && (!b || a->value.level < b->value.level)) {
			*pp = a;
			pp = &a->next;
			a = a->next;
		} else if (b && (!a || b->value.level < a->value.level)) {
			*pp = b;
			pp = &b->next;
			b = b->next;
		} else {
			*pp = b;
			pp = &b->next;
			b->value.digit += a->value.digit;
			b = b->next;
			BigInteger* tmp = a;
			a = a->next;
			free(tmp);
		}
	}
	/*
	printf("a = ");
	printBigInteger(sa);
	printf("b = ");
	printBigInteger(sb);
	printf("merge(a, b) = ");
	printBigInteger(res);
	res = cleanBigInteger(res);
	printf("clean res = ");
	printBigInteger(res);
	*/
	BigIntegerFree(sa);
	BigIntegerFree(sb);
	return res;
}

BigInteger* mergeSortBigInteger(BigInteger* a, int n) {
	if (n == 0) {
		return NULL;
	}
	if (n == 1) {
		return a;
	}
	int left_count = n / 2;
	BigInteger* left = a;
	BigInteger* p = left;
	for (int i = 1; i < left_count; ++i) {
		p = p->next;
	}
	BigInteger* right = p->next;
	p->next = NULL;
	left = mergeSortBigInteger(left, left_count);
	right = mergeSortBigInteger(right, n - left_count);
	return mergeBigInteger(left, right);
}

BigInteger* mulBigInteger(const BigInteger* a, const BigInteger* b) {
	BigInteger* res = NULL;
	BigInteger** pp = &res;
	const BigInteger* sa = a;
	while (a) {
		for (const BigInteger* i = b; i; i = i->next) {
			*pp = BigIntegerAlloc();
			(*pp)->value.digit = a->value.digit * i->value.digit;
			(*pp)->value.level = a->value.level + i->value.level;
			pp = &(*pp)->next;
		}
		a = a->next;
	}
	/*
	printf("a = ");
	printBigInteger(sa);
	printf("b = ");
	printBigInteger(b);
	printf("a * b = ");
	printBigInteger(res);
	*/
	res = mergeSortBigInteger(res, lenBigInteger(res));
	/*
	printf("a = ");
	printBigInteger(sa);
	printf("b = ");
	printBigInteger(b);
	printf("a * b = ");
	printBigInteger(res);
	*/
	return cleanBigInteger(res);
}

BigInteger* powerBigInteger(const BigInteger* a, int n) {
	BigInteger* res = BigIntegerAlloc();
	res->value.digit = 1;
	res->value.level = 0;
	BigInteger* base = mulBigInteger(res, a);
	while (n) {
		if (n & 1) {
			BigInteger* tmp = mulBigInteger(res, base);
			BigIntegerFree(res);
			res = tmp;
		}
		n /= 2;
		if (n) {
			BigInteger* tmp = mulBigInteger(base, base);
			BigIntegerFree(base);
			base = tmp;
		}
	}
	return res;
}

void calculateNum(BigInteger* a) {
	int digits[10];
	memset(digits, 0, sizeof(digits));
	int prev_level = -1;
	while (a) {
		if (prev_level > a->value.level + 1) {
			digits[0] += prev_level - 1 - a->value.level;
		}
		digits[a->value.digit]++;
		prev_level = a->value.level;
		a = a->next;
	}
	for (int i = 0; i < 10; ++i) {
		printf("digit %d : count %d\n", i, digits[i]);
	}
}


int main() {
	struct Value a[] = {
			{2, 0},
	};
	BigInteger* pa = BigIntegerCreateFromArray(a, sizeof(a) / sizeof(a[0]));
	BigInteger* pb = BigIntegerCreateFromArray(a, sizeof(a) / sizeof(a[0]));
	printBigInteger(pa);
	BigInteger* res = powerBigInteger(pa, 4000);
	printf("2**4000 = ");
	printBigInteger(res);
	calculateNum(res);
	BigIntegerFree(pa);
	BigIntegerFree(pb);
	BigIntegerFree(res);
	return 0;
}
