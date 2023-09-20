
#include "kmalloc/kmalloc.h"

#include "structure/zone.h"

#include <criterion/criterion.h>

#define TEST_SIZE 1024

ZoneHeader* someZone;

void suitesetup(void)
{
	someZone = NULL;
}

void suiteteardown(void)
{
	someZone = NULL;
}

TestSuite(zone_test, .init=suitesetup, .fini=suiteteardown);

Test(zone_test, create_zone)
{
	// small zone
	someZone = create_zone(&g_smallAllocationZoneMetadata);
	cr_expect_neq(someZone, NULL);
	
	cr_expect_eq(someZone->start, header_boundary_zone_start);
	cr_expect_eq(someZone->metadata, &g_smallAllocationZoneMetadata);
	cr_expect_eq(someZone->nextZone, NULL);
	cr_expect_eq(someZone->end, header_boundary_zone_end);

	// medium zone
	someZone = create_zone(&g_mediumAllocationZoneMetadata);
	cr_expect(someZone != NULL);

	cr_expect_eq(someZone->start, header_boundary_zone_start);
	cr_expect_eq(someZone->metadata, &g_mediumAllocationZoneMetadata);
	cr_expect_eq(someZone->nextZone, NULL);
	cr_expect_eq(someZone->end, header_boundary_zone_end);
}

Test(zone_test, destroy_zone, .signal=SIGSEGV)
{
	someZone = create_zone(&g_smallAllocationZoneMetadata);
	cr_expect_neq(someZone, NULL);
	
	destroy_zone(someZone);
	cr_expect_eq(someZone->start, 0); // should crash here
	cr_expect_eq(someZone->metadata, NULL);
	cr_expect_eq(someZone->nextZone, NULL);
	cr_expect_eq(someZone->end, 0);
}

Test(zone_test, zone_bitmap_size)
{
	// small zone
	cr_expect_eq(g_smallAllocationZoneMetadata.bitmapSize, 128);

	// medium zone
	cr_expect_eq(g_mediumAllocationZoneMetadata.bitmapSize, 64);
}

Test(zone_test, zone_allocation)
{
	someZone = create_zone(&g_smallAllocationZoneMetadata);
	cr_expect_neq(someZone, NULL);
	
	// First bit is set for zone header.
	uint32_t bitsSetInBitmap = 1; // 0001
	cr_expect_eq(someZone->blockBitmap[0] & bitsSetInBitmap, bitsSetInBitmap);
	
	allocate_in_zone(someZone, 19);
	bitsSetInBitmap = 7; // 0111
	cr_expect_eq(someZone->blockBitmap[0] & bitsSetInBitmap, bitsSetInBitmap);

	allocate_in_zone(someZone, 5);
	bitsSetInBitmap = 15; // 1111
	cr_expect_eq(someZone->blockBitmap[0] & bitsSetInBitmap, bitsSetInBitmap);

	allocate_in_zone(someZone, 128); // [0] 0000-0000 0000-0000 0001-1111 1111-1111
	bitsSetInBitmap = 0x00001FFF;
	cr_expect_eq(someZone->blockBitmap[0] & bitsSetInBitmap, bitsSetInBitmap);

	allocate_in_zone(someZone, 128); // [0] 0000-0000 0011-1111 1111-1111 1111-1111
	bitsSetInBitmap = 0x002FFFFF;
	cr_expect_eq(someZone->blockBitmap[0] & bitsSetInBitmap, bitsSetInBitmap);

	allocate_in_zone(someZone, 128); // [0] 0111-1111 1111-1111 1111-1111 1111-1111
	bitsSetInBitmap = 0x7FFFFFFF;
	cr_expect_eq(someZone->blockBitmap[0] & bitsSetInBitmap, bitsSetInBitmap);

	allocate_in_zone(someZone, 128); // [0] 1111-1111 1111-1111 1111-1111 1111-1111 [1] 1111-1111
	bitsSetInBitmap = 0xFFFFFFFF;
	cr_expect_eq(someZone->blockBitmap[0] & bitsSetInBitmap, bitsSetInBitmap);
	bitsSetInBitmap = 0x000000FF;
	cr_expect_eq(someZone->blockBitmap[1] & bitsSetInBitmap, bitsSetInBitmap);

}
