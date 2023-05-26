
#include "kmalloc/kmalloc.h"

#include <criterion/criterion.h>

#define TEST_SIZE 1024

char* someVariable;

void suitesetup(void)
{
	someVariable = NULL;
}

void suiteteardown(void)
{
	someVariable = NULL;
}

TestSuite(free_test, .init=suitesetup, .fini=suiteteardown);

Test(free_test, some_test)
{
	free(someVariable);
	cr_expect(someVariable == NULL);
}
