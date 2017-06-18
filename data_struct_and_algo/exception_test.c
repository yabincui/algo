#include "exception.h"

#include <stdio.h>

void test1() {
	TRY
		{
			printf("hello, world!\n");
			}ENDTRY
}

void test2() {
	TRY
			{
				RAISE(1, "reason one");
			}
		CATCH(1, e)
			{
				printf("catched exception 1\n");
			}
		ENDTRY
}

void test3() {
	TRY
			{
				RAISE(1, "reason one");
			}ENDTRY
}

void test4() {
	TRY
			{
				printf("hello, world!\n");
			}CATCH(1, e)
			{
				printf("catch 1\n");

			}FINALLY
			{
				printf("finally!\n");
			}ENDTRY
}

void test5() {
	TRY
			{
				RAISE(1, "reason one");
			}FINALLY
			{
				printf("finally\n");
			}ENDTRY
}

void test6() {
	TRY
			{
				RAISE(2, "reason two");
			}CATCH(1, e)
			{
				printf("catch 1\n");
			}CATCH(2, e)
			{
				printf("catch 2: %s\n", e->reason);
			}FINALLY
			{
				printf("finally\n");
			}ENDTRY
}

int main() {
	test1();
	test2();
	//test3();
	test4();
	//test5();
	test6();
	return 0;
}
