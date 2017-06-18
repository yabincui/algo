// 3.2 Given a linked list L and P. implement print_lots(L, P). P contains a list of
// increasing numbers, which are positions to print in P.

#include "list.h"
#include "logging.h"

DEFINE_LIST_TYPE(IntList, int, value);

IntList* createIntList(int s[], int n) {
	IntList* head = NULL;
	IntList** p = &head;
	for (int i = 0; i < n; ++i) {
		*p = IntListAlloc();
		(*p)->value = s[i];
		p = &(*p)->next;
	}
	return head;
}

void print_lots(IntList* L, IntList* P) {
	int cur = 1;
	while (P && L) {
		if (cur == P->value) {
			printf("%d\n", L->value);
			P = P->next;
		}
		L = L->next;
		cur++;
	}
}

int main() {
	int l[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int p[] = {1, 3, 4, 6};
	IntList* L = createIntList(l, sizeof(l) / sizeof(l[0]));
	IntList* P = createIntList(p, sizeof(p) / sizeof(p[0]));
	print_lots(L, P);
	IntListFree(L);
	IntListFree(P);
	return 0;
}
