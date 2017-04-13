#ifndef _DS_LIST_H
#define _DS_LIST_H

#include <stdlib.h>

#define DEFINE_LIST_TYPE(type_name, data_type, data_name) \
typedef struct type_name {                                \
	data_type data_name;                                  \
	struct type_name* next;                               \
} type_name;                                              \
                                                          \
type_name* type_name##Alloc() {                           \
    type_name* res = calloc(1, sizeof(type_name));        \
    CHECK(res);                                           \
    return res;                                           \
}                                                         \
                                                          \
void type_name##Free(type_name* l) {                      \
    while (l) {                                           \
    	type_name* tmp = l;                               \
    	free(l);                                          \
    	l = tmp;                                          \
    }                                                     \
}                                                         \
                                                          \
type_name* type_name##Add(type_name* l, data_type data) { \
    type_name* new_item = type_name##Alloc();             \
    new_item->data_name = data;                           \
    if (l == NULL) return new_item;                       \
    new_item->next = l->next;                             \
    l->next = new_item;                                   \
    return l;                                             \
}                                                         \



#endif  // _DS_LIST_H