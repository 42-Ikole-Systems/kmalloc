
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

TestSuite(calloc_test, .init=suitesetup, .fini=suiteteardown);

Test(calloc_test, some_test)
{
	someVariable = km_calloc(2, 5);
	cr_expect(someVariable == NULL);
}
