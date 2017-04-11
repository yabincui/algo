#include "trie.h"

#include "logging.h"

int main() {
	struct Trie* trie = createTrie();
	CHECK(trie);

	CHECK(trieAddWord(trie, "hello"));
	CHECK(trieAddWord(trie, "hello"));
	struct Trie* a;
	CHECK(a = trieGetSubTrieByString(trie, "hello"));
	CHECK(2 == trieGetCount(a));
	CHECK(a = trieGetSubTrieByString(trie, "hell"));
	CHECK(0 == trieGetCount(a));
	CHECK(!trieGetSubTrieByString(trie, "hellowe"));

	destroyTrie(trie);
	return 0;
}
