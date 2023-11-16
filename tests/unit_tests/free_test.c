
#include "kmalloc/kmalloc.h"
#include "structure/allocation.h"

#include <criterion/criterion.h>

#include <signal.h>

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

Test(free_test, basic_free)
{
	someVariable = km_malloc(12);
	km_free(someVariable);
	cr_expect(get_allocation_header(someVariable) == NULL);
}

Test(free_test, double_free, .signal=SIGSEGV)
{
	someVariable = km_malloc(16);
	km_free(someVariable);
	km_free(someVariable);
}

Test(free_test, not_allocated, .signal=SIGSEGV)
{
	km_free(someVariable);
}
