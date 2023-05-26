
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

TestSuite(TestGroup, .init=suitesetup, .fini=suiteteardown);

Test(TestGroup, some_test)
{
	someVariable = km_calloc(2, 5);
	cr_expect(someVariable == NULL);
}