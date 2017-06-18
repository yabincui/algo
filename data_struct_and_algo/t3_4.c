// 3.4 Given two sorted lists, L1 and L2, write a procedure to compute L1 and L2.

#include "list.h"
#include "logging.h"
#include "utils.h"

DEFINE_LIST_TYPE(IntList, int, value);

IntList* FindCommon(IntList* a, IntList* b) {
	IntList* res = NULL;
	IntList** pp = &res;
	while (a && b) {
		if (a->value < b->value) {
			a = a->next;
		} else if (a->value > b->value) {
			b = b->next;
		} else {
			(*pp) = IntListAlloc();
			(*pp)->value = a->value;
			pp = &(*pp)->next;
			a = a->next;
			b = b->next;
		}
	}
	return res;
}

void printList(IntList* a) {
	printf("[");
	while (a) {
		printf("%d ", a->value);
		a = a->next;
	}
	printf("]\n");
}

int main() {
	int a[] = {2, 4, 6, 8, 10, 12};
	int b[] = {3, 6, 9, 12};
	IntList* pa = IntListCreateFromArray(a, sizeof(a) / sizeof(a[0]));
	IntList* pb = IntListCreateFromArray(b, sizeof(b) / sizeof(b[0]));
	IntList* res = FindCommon(pa, pb);
	printList(res);
	IntListFree(pa);
	IntListFree(pb);
	IntListFree(res);
	return 0;
}
