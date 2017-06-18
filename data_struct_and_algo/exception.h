#ifndef ALGO_EXCEPTION_H
#define ALGO_EXCEPTION_H

#include <setjmp.h>

typedef struct Exception {
	int type;
	char reason[0];
} Exception;

struct ExceptionFrame {
	jmp_buf env;	
	Exception* exception;
	const char* filename;
	int line;
	struct ExceptionFrame* prev;
};

enum {
	exception_entered = 0, exception_raised, exception_handled
};

// It is not thread safe!!!
extern struct ExceptionFrame* g_exception_stack;

Exception* NewException(int type, const char* reason);
void FreeException(Exception* e);
void RaiseException(Exception* e, const char* filename, int line);

#define TRY \
	{																					\
		struct ExceptionFrame frame;													\
		frame.prev = g_exception_stack;													\
		g_exception_stack = &frame;														\
		int exception_flag = setjmp(frame.env);											\
		if (exception_flag == exception_entered) { 										\

#define CATCH(exception_type, exception_variable) \
	 } else if (exception_flag == exception_raised && frame.exception->type == exception_type) {	\
			exception_flag = exception_handled;													\
			g_exception_stack = frame.prev;														\
			Exception* exception_variable = frame.exception;


#define FINALLY \
		} {

#define ENDTRY \
		}																				\
		if (exception_flag != exception_handled) {										\
			g_exception_stack = frame.prev;												\
		} else {																		\
			FreeException(frame.exception);												\
		}																				\
		if (exception_flag == exception_raised) {										\
			RaiseException(frame.exception, frame.filename, frame.line);				\
		}																				\
	}																					\

#define RAISE(type, reason) \
	RaiseException(NewException(type, reason), __FILE__, __LINE__)

#define RERAISE \
	RaiseException(frame.exception, frame.filename, frame.line)

#endif // ALGO_EXCEPTION_H