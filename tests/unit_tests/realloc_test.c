
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

TestSuite(realloc_test, .init=suitesetup, .fini=suiteteardown);

Test(realloc_test, some_test)
{
	someVariable = km_realloc(NULL, 10);
	cr_expect(someVariable == NULL);
}
