// 1.4 C allows #include filename to include other files.
// Write a program to read in a file and outputs the file as modified by the include
// statements.

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "logging.h"

static bool readLine(char** ps, int* psize, FILE* fp) {
	int ch;
	int len = 0;
	if (*psize == 0) {
		*ps = (char*)calloc(16, 1);
		if (!*ps) return false;
		*psize = 16;
	}
	while ((ch = fgetc(fp)) != EOF) {
		if (*psize <= len + 1) {
			char* t = (char*)realloc(*ps, *psize * 2);
			if (!t) {
				return false;
			}
			*ps = t;
			*psize *= 2;
		}
		(*ps)[len++] = (char)ch;
		if (ch == '\n') {
			break;
		}
	}
	if (len == 0) {
		return false;
	}
	(*ps)[len] = '\0';
	return true;
}

char* getIncludedFile(const char* s) {
	int begin = 0;
	while (isspace(s[begin])) {
		begin++;
	}
	if (strncmp(&s[begin], "#include", strlen("#include")) == 0) {
		begin += strlen("#include");
	} else {
		return NULL;
	}
	while (isspace(s[begin]) || s[begin] == '\"' || s[begin] == '<') {
		begin++;
	}
	if (!s[begin]) {
		return NULL;
	}
	int end = begin;
	while (s[end] && !isspace(s[end]) && s[end] != '\"' && s[end] != '>') {
		end++;
	}
	int len = end - begin;
	if (len == 0) return NULL;
	char* res = calloc(1, len + 1);
	if (!res) return NULL;
	memcpy(res, s + begin, len);
	res[len] = '\0';
	return res;
}

bool preprocessFile(const char* filename) {
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		return false;
	}
	char* s = NULL;
	int len = 0;
	while (readLine(&s, &len, fp)) {
		char* included_file = getIncludedFile(s);
		if (!included_file || !preprocessFile(included_file)) {
			printf("%s", s);
		}
		if (included_file) {
			free(included_file);
		}
	}
	if (s) {
		free(s);
	}
	fclose(fp);
	return true;
}

int main(int argc, char** argv) {
	const char* filename = "t1_4.c";
	if (argc != 1) {
		filename = argv[1];
	}
	preprocessFile(filename);
	return 0;
}
