
#include "kmalloc/kmalloc.h"

#include "structure/zone.h"

#include <criterion/criterion.h>

#define TEST_SIZE 1024

char* somePointer;

void suitesetup(void)
{
	somePointer = NULL;
}

void suiteteardown(void)
{
	somePointer = NULL;
}

TestSuite(malloc_test, .init=suitesetup, .fini=suiteteardown);

Test(malloc_test, some_test)
{
	somePointer = km_malloc(10);
	cr_expect(somePointer == NULL);
}

Test(malloc_test, create_zone)
{
	// small zone
	somePointer = (void*)create_zone(&g_smallAllocationZoneMetadata);
	cr_expect(somePointer != NULL);
	
	ZoneHeader* zoneHeader = (ZoneHeader*)somePointer;
	cr_expect_eq(zoneHeader->start, header_boundary_zone_start);
	cr_expect_eq(zoneHeader->metadata, &g_smallAllocationZoneMetadata);
	cr_expect_eq(zoneHeader->nextZone, NULL);
	cr_expect_eq(zoneHeader->end, header_boundary_zone_end);

	// medium zone
	somePointer = (void*)create_zone(&g_mediumAllocationZoneMetadata);
	cr_expect(somePointer != NULL);

	zoneHeader = (ZoneHeader*)somePointer;
	cr_expect_eq(zoneHeader->start, header_boundary_zone_start);
	cr_expect_eq(zoneHeader->metadata, &g_mediumAllocationZoneMetadata);
	cr_expect_eq(zoneHeader->nextZone, NULL);
	cr_expect_eq(zoneHeader->end, header_boundary_zone_end);
}
