#include "exception.h"

#include "logging.h"

struct ExceptionFrame* g_exception_stack;

Exception* NewException(int type, const char* reason) {
	Exception* e = (Exception*) malloc(sizeof(Exception) + strlen(reason) + 1);
	CHECK(e);
	e->type = type;
	strcpy(e->reason, reason);
	return e;
}

void FreeException(Exception* e) {
	free(e);
}

void RaiseException(Exception* e, const char* filename, int line) {
	if (g_exception_stack == NULL) {
		printf("exception type %d, reason %s at %s:%d\n", e->type, e->reason,
				filename, line);
		abort();
	}
	g_exception_stack->exception = e;
	g_exception_stack->filename = filename;
	g_exception_stack->line = line;
	longjmp(g_exception_stack->env, exception_raised);
}
