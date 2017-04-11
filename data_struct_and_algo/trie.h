#ifndef _DS_TRIE_H
#define _DS_TRIE_H

#include <stdbool.h>

// The trie only accepts lower 26 characters, 'a' - 'z'.
struct Trie;

struct Trie* createTrie();

void destroyTrie(struct Trie* trie);

bool trieAddWord(struct Trie* trie, const char* s);

struct Trie* trieGetSubTrie(struct Trie* trie, char c);
struct Trie* trieGetSubTrieByString(struct Trie* trie, const char* s);

int trieGetCount(struct Trie* trie);

void trieDecCount(struct Trie* trie);

int trieGetTrieSize(struct Trie* trie);

#endif  // _DS_TRIE_H