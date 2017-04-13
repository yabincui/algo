// 3.1 Write a program to print out the elements of a singly linked list.

#include "list.h"
#include "logging.h"

DEFINE_LIST_TYPE(IntList, int, value);

static void printList(IntList* head) {
	head = head->next;
	while (head != NULL) {
		printf("%d\n", head->value);
		head = head->next;
	}
}

int main() {
	IntList* head = IntListAlloc();
	for (int i = 0; i < 10; ++i) {
		head = IntListAdd(head, i);
	}
	printList(head);
	return 0;
}
