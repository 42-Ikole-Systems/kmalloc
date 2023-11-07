
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

TestSuite(malloc_test, .init=suitesetup, .fini=suiteteardown);

void TestAllocation(size_t size)
{
	someVariable = km_malloc(sizeof(char) * size);
	cr_expect(someVariable != NULL);

	for (size_t i = 0; i < size; i++) {
		someVariable[i] = (char)i;
	}
}

Test(malloc_test, basic_allocation_test)
{
	TestAllocation(16);
	TestAllocation(32);
	TestAllocation(64);
	TestAllocation(128);
	TestAllocation(256);
	TestAllocation(512);
}

Test(malloc_test, multiple_allocation_test)
{
	for (size_t i = 0; i < 256; i++) {
		TestAllocation(256);
	}
}
