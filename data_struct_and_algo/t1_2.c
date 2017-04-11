// 1.2 Write a program to solve the word puzzle problem.
// Find all words in a maze.

// The idea is to use dfs to visit all directions, and use hash table or trie to verify the string.
// How to generate the dictionary?

#include <ctype.h>
#include <string.h>

#include "logging.h"
#include "trie.h"

static struct Trie* loadDictionary() {
	char s[100];
	FILE* fp = fopen("words.txt", "r");
	if (fp == NULL) {
		return NULL;
	}
	struct Trie* trie = createTrie();
	if (trie == NULL) {
		return trie;
	}
	int str_count = 0;
	while (fgets(s, sizeof(s), fp)) {
		int n = strlen(s);
		while (n > 0 && isspace(s[n - 1])) {
			n--;
		}
		if (n == 0) {
			continue;
		}
		s[n] = '\0';
		int i;
		for (i = 0; i < n; ++i) {
			if (s[i] < 'a' || s[i] > 'z') {
				break;
			}
		}
		if (i != n) continue;
		if (!trieAddWord(trie, s)) {
			destroyTrie(trie);
			return NULL;
		}
		str_count++;
	}
	printf("trie_count = %d, str_count = %d\n", trieGetTrieSize(trie), str_count);

	fclose(fp);
	return trie;
}

static void createDict(char m[16][16]) {
	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 16; ++j) {
			m[i][j] = rand() % 26 + 'a';
		}
	}
}

static int dr[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
static int dc[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

static void recur(int r, int c, int dir, char m[16][16], struct Trie* trie,
		char path[17], int path_len) {
	while (true) {
		trie = trieGetSubTrie(trie, m[r][c]);
		if (!trie) return;
		path[path_len++] = m[r][c];
		if (trieGetCount(trie) > 0) {
			path[path_len] = '\0';
			printf("%s\n", path);
			trieDecCount(trie);
		}
		r = r + dr[dir];
		c = c + dc[dir];
		if (r < 0 || r >= 16 || c < 0 || c >= 16) return;
	}
}

static void solve(char m[16][16], struct Trie* trie) {
	char path[17];
	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 16; ++j) {
			for (int d = 0; d < 8; ++d) {
				recur(i, j, d, m, trie, path, 0);
			}
		}
	}
}

int main() {
	struct Trie* trie = loadDictionary();
	if (trie == NULL) {
		return 1;
	}
	char m[16][16];
	createDict(m);
	solve(m, trie);

	destroyTrie(trie);
	return 0;
}
