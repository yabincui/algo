// 3.10 The josephus proble is: n people, numbered 1-n, are sitting in a circle.
// Starting at person 1, a handgun is passed. After m passes, the person holding
// the gun commits suicide, the body is removed, and the game continues with the
// person next to the suicide person. Ask for the order.

#include "list.h"
#include "logging.h"
#include "utils.h"

DEFINE_DOUBLE_CIRCLE_LIST_TYPE(IntList, int, value);

IntList* createIntList(int n) {
	IntList* head = IntListAlloc();
	for (int i = 1; i <= n; ++i) {
		IntListAddData(head->prev, i);
	}
	return head;
}

void printIntList(IntList* a) {
	for (IntList* i = a->next; i != a; i = i->next) {
		printf("%d ", i->value);
	}
	printf("\n");
}

int lenIntList(IntList* a) {
	int len = 0;
	for (IntList* i = a->next; i != a; i = i->next) {
		len++;
	}
	return len;
}

// method 1: O(N * min(M, N)).
IntList* getSuicideList(IntList* p, int m) {
	IntList* res = IntListAlloc();
	int n = lenIntList(p);
	if (n == 0) return res;
	CHECK(m > 0);
	m--;
	IntList* cur = p->next;
	while (!IntListEmpty(p)) {
		int k = m % n;
		for (int step = 0; step < k; ++step) {
			if (cur == p) {
				cur = cur->next;
			}
			cur = cur->next;
		}
		if (cur == p) {
			cur = cur->next;
		}
		IntList* next = cur->next;
		IntListRemove(p, cur);
		IntListAdd(res->prev, cur);
		cur = next;
		n--;
	}
	return res;
}

// method 2: use binary index tree to calculate the position of next m people.
IntList* getSuicideList2(IntList* p, int m) {
	IntList* res = IntListAlloc();
	int n = lenIntList(p);
	if (n == 0) return res;
	CHECK(m > 0);
	m--;
	int* s = (int*)calloc(n, sizeof(int));
	IntList* cur = p->next;
	for (int i = 0; i < n; ++i) {
		s[i] = cur->value;
		cur = cur->next;
	}
	int* bit = (int*)calloc(n, sizeof(int));
	CHECK(bit);



	free(bit);
	free(s);
	return res;
}

int main() {
	IntList* a = createIntList(3);
	printIntList(a);
	IntList* res = getSuicideList(a, 4);
	printIntList(res);
	IntListFree(a);
	IntListFree(res);
	return 0;
}
