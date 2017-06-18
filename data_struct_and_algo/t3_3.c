// 3.3 Swap two adjacent elements by adjusting only the pointers (and not the data),
// using a. single linked list, b. double linked list.

#include "list.h"
#include "logging.h"
#include "utils.h"

DEFINE_LIST_TYPE(IntList, int, value);
DEFINE_DOUBLE_LIST_TYPE(IntDoubleList, int, value);

void checkIntList(IntList* l, int s[], int n) {
	//printf("[");
	int i = 0;
	while (l) {
		//printf("%d ", l->value);
		CHECK(i < n);
		CHECK(s[i++] == l->value);
		l = l->next;
	}
	//printf("]\n");
	CHECK(i == n);
}

IntList* swapIntList(IntList* l, IntList* a, IntList* b) {
	if (a == b) return l;
	if (b->next == a) {
		swapTwo(&a, &b, sizeof(IntList*));
	}
	IntList** pp = &l;
	IntList** pa = NULL;
	IntList** pb = NULL;
	while (*pp) {
		if (*pp == a) pa = pp;
		if (*pp == b) pb = pp;
		pp = &(*pp)->next;
	}
	CHECK(pa && pb);
	if (a->next == b) {
		IntList* next = b->next;
		*pa = b;
		b->next = a;
		a->next = next;
	} else {
		IntList* next_of_a = a->next;
		IntList* next_of_b = b->next;
		*pa = b;
		b->next = next_of_a;
		*pb = a;
		a->next = next_of_b;
	}
	return l;
}

void checkIntDoubleList(IntDoubleList* l, int s[], int n) {
	//printf("[");
	int i = 0;
	while (l) {
		//printf("%d ", l->value);
		CHECK(i < n);
		CHECK(s[i++] == l->value);
		IntDoubleList* next = l->next;
		if (next) {
			CHECK(next->prev == l);
		}
		l = next;
	}
	//printf("]\n");
	CHECK(i == n);
}

IntDoubleList* swapIntDoubleList(IntDoubleList* l, IntDoubleList* a, IntDoubleList* b) {
	if (a == b) return l;
	if (b->next == a) {
		swapTwo(&a, &b, sizeof(IntDoubleList*));
	}
	if (l == b) l = a;
	else if (l == a) l = b;

	if (a->next == b) {
		IntDoubleList* pa = a->prev;
		IntDoubleList* nb = b->next;
		a->next = nb;
		a->prev = b;
		b->next = a;
		b->prev = pa;
		if (pa) pa->next = b;
		if (nb) nb->prev = a;
	} else {
		IntDoubleList* pa = a->prev;
		IntDoubleList* na = a->next;
		a->prev = b->prev;
		a->next = b->next;
		b->prev = pa;
		b->next = na;
		if (a->prev) a->prev->next = a;
		if (a->next) a->next->prev = a;
		if (b->prev) b->prev->next = b;
		if (b->next) b->next->prev = b;
	}
	return l;
}

int main() {
	for (int i = 1; i <= 6; ++i) {
		for (int j = 1; j <= 6; ++j) {
			int s[] = {1, 2, 3, 4, 5, 6};
			IntList* p = IntListCreateFromArray(s, sizeof(s)/sizeof(s[0]));
			IntList* a = IntListFind(p, i);
			IntList* b = IntListFind(p, j);
			p = swapIntList(p, a, b);
			swapTwo(&s[i-1], &s[j-1], sizeof(s[i-1]));
			checkIntList(p, s, sizeof(s) / sizeof(s[0]));
			IntListFree(p);
		}
	}
	for (int i = 1; i <= 6; ++i) {
		for (int j = 1; j <= 6; ++j) {
			int s[] = {1, 2, 3, 4, 5, 6};
			IntDoubleList* p = IntDoubleListCreateFromArray(s, sizeof(s)/sizeof(s[0]));
			IntDoubleList* a = IntDoubleListFind(p, i);
			IntDoubleList* b = IntDoubleListFind(p, j);
			p = swapIntDoubleList(p, a, b);
			swapTwo(&s[i-1], &s[j-1], sizeof(s[i-1]));
			checkIntDoubleList(p, s, sizeof(s) / sizeof(s[0]));
			IntDoubleListFree(p);
		}
	}
	return 0;
}
