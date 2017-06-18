#ifndef _DS_LIST_H
#define _DS_LIST_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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
    	type_name* tmp = l->next;                         \
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
                                                              \
type_name* type_name##Remove(type_name* l, type_name* node) { \
	CHECK(l && node);                                         \
    if (l == node) return l->next;                            \
    type_name* p;                                             \
    for (p = l; p->next != node && p->next; p = p->next);     \
    if (p->next == node) {                                    \
    	p->next = node->next;	                              \
    }                                                         \
    return l;                                                 \
} \
type_name* type_name##CreateFromArray(data_type s[], int n) { \
	type_name* head = NULL; \
	type_name** p = &head; \
	for (int i = 0; i < n; ++i) { \
		*p = type_name##Alloc(); \
		(*p)->data_name = s[i]; \
		p = &(*p)->next; \
	} \
	return head; \
} \
type_name* type_name##Find(type_name* l, data_type key) { \
    while (l) { \
    	if (memcmp(&(l->data_name), &key, sizeof(data_type)) == 0) return l; \
    	l = l->next; \
    } \
    return NULL; \
} \
\
type_name* type_name##Reverse(type_name* l) { \
	type_name* prev = NULL; \
	type_name* cur = l; \
	while (cur) { \
		type_name* next = cur->next; \
		cur->next = prev; \
		prev = cur; \
		cur = next; \
	} \
	return prev; \
}

#define DEFINE_DOUBLE_LIST_TYPE(type_name, data_type, data_name) \
typedef struct type_name {                                       \
	data_type data_name;                                         \
	struct type_name *prev, *next;                               \
} type_name;                                                     \
                                                                 \
type_name* type_name##Alloc() {                                  \
	type_name* res = calloc(1, sizeof(type_name));               \
	CHECK(res);                                                  \
	return res;                                                  \
}                                                                \
                                                                 \
void type_name##Free(type_name* l) {                             \
	while (l) {                                                  \
		type_name* tmp = l->next;                                \
		free(l);                                                 \
		l = tmp;                                                 \
	}                                                            \
}                                                                \
                                                                 \
type_name* type_name##Add(type_name* l, data_type data) {        \
	type_name* new_item = type_name##Alloc();                    \
	new_item->data_name = data;                                  \
	if (l == NULL) return new_item;                              \
	new_item->next = l->next;                                    \
	new_item->prev = l;                                          \
	l->next = new_item;                                          \
	if (new_item->next) new_item->next->prev = new_item;         \
}                                                                \
                                                                 \
type_name* type_name##Remove(type_name* l, type_name* node) {    \
	if (l == node) {                                             \
		if (l->next) l->next->prev = NULL;                       \
		return l->next;                                          \
	}                                                            \
	node->prev->next = node->next;                               \
	if (node->next) node->next->prev = node->prev;               \
	node->prev = node->next = NULL;                              \
	return l;                                                    \
} \
type_name* type_name##CreateFromArray(data_type s[], int n) { \
	type_name* head = NULL; \
	type_name* prev = NULL; \
	type_name** p = &head; \
	for (int i = 0; i < n; ++i) { \
		*p = type_name##Alloc(); \
		(*p)->data_name = s[i]; \
		(*p)->prev = prev; \
		prev = *p; \
		p = &(*p)->next; \
	} \
	return head; \
} \
type_name* type_name##Find(type_name* l, data_type key) { \
    while (l) { \
    	if (memcmp(&(l->data_name), &key, sizeof(data_type)) == 0) return l; \
    	l = l->next; \
    } \
    return NULL; \
}


#define DEFINE_DOUBLE_CIRCLE_LIST_TYPE(type_name, data_type, data_name) \
typedef struct type_name {                                       \
	data_type data_name;                                         \
	struct type_name *prev, *next;                               \
} type_name;                                                     \
                                                                 \
type_name* type_name##Alloc() {                                  \
	type_name* res = calloc(1, sizeof(type_name));               \
	CHECK(res);                                                  \
	res->prev = res->next = res;                                \
	return res;                                                  \
}                                                                \
                                                                 \
void type_name##Free(type_name* l) {                             \
	if (l) { \
		l->prev->next = NULL; \
	} \
	while (l) {                                                  \
		type_name* tmp = l->next;                                \
		free(l);                                                 \
		l = tmp;                                                 \
	}                                                            \
}                                                                \
 \
type_name* type_name##Add(type_name* l, type_name* node) { \
	CHECK(l);                              \
	node->next = l->next;                                    \
	node->prev = l;                                          \
	l->next = node;                                          \
	node->next->prev = node;         \
	return l; \
} \
                                                                 \
type_name* type_name##AddData(type_name* l, data_type data) {        \
	type_name* new_item = type_name##Alloc();                    \
	new_item->data_name = data;                                  \
	return type_name##Add(l, new_item); \
}                                                                \
                                                                 \
type_name* type_name##Remove(type_name* l, type_name* node) {    \
	CHECK(l != node); \
	node->prev->next = node->next;                               \
	node->next->prev = node->prev;               \
	node->prev = node->next = NULL;                              \
	return l;                                                    \
} \
type_name* type_name##CreateFromArray(data_type s[], int n) { \
	type_name* head = type_name##Alloc(); \
	for (int i = 0; i < n; ++i) { \
		type_name* p = type_name##Alloc(); \
		p->value = s[i]; \
		type_name##Add(head->prev, p); \
	} \
	return head; \
} \
type_name* type_name##Find(type_name* l, data_type key) { \
    for (type_name* i = l->next; i != l; i = i->next) { \
    	if (memcmp(&(i->data_name), &key, sizeof(data_type)) == 0) return i; \
    } \
    return NULL; \
} \
bool type_name##Empty(type_name* l) { \
	return l->next == l; \
}


#endif  // _DS_LIST_H