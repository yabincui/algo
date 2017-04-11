#include "trie.h"

#include <stdlib.h>
#include "logging.h"

struct Trie {
	int count;
	struct Trie* child[26];
};

static struct Trie* newTrie() {
	struct Trie* res = (struct Trie*)calloc(1, sizeof(struct Trie));
	CHECK(res);
	return res;
}


struct Trie* createTrie() {
	return newTrie();
}

void destroyTrie(struct Trie* trie) {
	for (int i = 0; i < 26; ++i) {
		if (trie->child[i]) {
			destroyTrie(trie->child[i]);
		}
	}
	free(trie);
}

bool trieAddWord(struct Trie* trie, const char* s) {
	CHECK(s);
	for (const char* t = s; *t; ++t) {
		if (*t < 'a' || *t > 'z') {
			return false;
		}
	}
	struct Trie** pp = &trie;
	for (const char* t = s; *t; ++t) {
		if (*pp == NULL) {
			*pp = newTrie();
		}
		pp = &((*pp)->child[*t - 'a']);
	}
	if (*pp == NULL) {
		*pp = newTrie();
	}
	(*pp)->count++;
	return true;
}

struct Trie* trieGetSubTrie(struct Trie* trie, char c) {
	if (c < 'a' || c > 'z') {
		return NULL;
	}
	return trie->child[c - 'a'];
}

struct Trie* trieGetSubTrieByString(struct Trie* trie, const char* s) {
	CHECK(s);
	while (*s) {
		trie = trieGetSubTrie(trie, *s);
		if (!trie) {
			return NULL;
		}
		s++;
	}
	return trie;
}

int trieGetCount(struct Trie* trie) {
	return trie->count;
}

void trieDecCount(struct Trie* trie) {
	CHECK(trie->count);
	trie->count--;
}

int trieGetTrieSize(struct Trie* trie) {
	int sum = 1;
	for (int i = 0; i < 26; ++i) {
		if (trie->child[i]) {
			sum += trieGetTrieSize(trie->child[i]);
		}
	}
	return sum;
}
