
#include "kmalloc/kmalloc.h"

#include "structure/zone.h"
#include "structure/arena.h"

#include <criterion/criterion.h>

#include <signal.h>

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
	someZone = create_zone(&g_smallAllocationZoneMetadata, 0);
	cr_expect_neq(someZone, NULL);
	
	cr_expect_eq(someZone->start, header_boundary_zone_start);
	cr_expect_eq(someZone->metadata, &g_smallAllocationZoneMetadata);
	cr_expect_eq(someZone->nextZone, NULL);
	cr_expect_eq(someZone->end, header_boundary_zone_end);

	// medium zone
	someZone = create_zone(&g_mediumAllocationZoneMetadata, 0);
	cr_expect(someZone != NULL);

	cr_expect_eq(someZone->start, header_boundary_zone_start);
	cr_expect_eq(someZone->metadata, &g_mediumAllocationZoneMetadata);
	cr_expect_eq(someZone->nextZone, NULL);
	cr_expect_eq(someZone->end, header_boundary_zone_end);
}

Test(zone_test, destroy_zone, .signal=SIGSEGV)
{
	someZone = create_zone(&g_smallAllocationZoneMetadata, 0);
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
	// ==========================================================================================
	// THESE FOLLOWING TESTS ARE BASED ON DEFAULT SETTINGS, IF THEY ARE CHANGED TESTS MIGHT FAIL.
	// ==========================================================================================

	someZone = create_zone(&g_smallAllocationZoneMetadata, 0);
	cr_expect_neq(someZone, NULL);

	// First bits is set to accomodate for zone header.
	uint32_t bitsSetInBitmap = 0x0F; // 1111
	cr_expect_eq(someZone->blockBitmap[0], bitsSetInBitmap);

	AllocationData allocationData = get_allocation_data(&someZone, 19, &g_smallAllocationZoneMetadata);
	void* ret;
	ret = allocate_in_zone(allocationData);
	cr_expect_neq(ret, NULL);
	bitsSetInBitmap = 0x3F; // 0011-1111
	cr_expect_eq(someZone->blockBitmap[0], bitsSetInBitmap);

	allocationData = get_allocation_data(&someZone, 5, &g_smallAllocationZoneMetadata);
	ret = allocate_in_zone(allocationData);
	cr_expect_neq(ret, NULL);
	bitsSetInBitmap = 0x7F; // 0111-1111
	cr_expect_eq(someZone->blockBitmap[0], bitsSetInBitmap);

	allocationData = get_allocation_data(&someZone, 128, &g_smallAllocationZoneMetadata);
	ret = allocate_in_zone(allocationData); // [0] 0000-0000 0000-0000 1111-1111 1111-1111
	cr_expect_neq(ret, NULL);
	bitsSetInBitmap = 0x0000FFFF;
	cr_expect_eq(someZone->blockBitmap[0], bitsSetInBitmap);

	allocationData = get_allocation_data(&someZone, 128, &g_smallAllocationZoneMetadata);
	ret = allocate_in_zone(allocationData); // [0] 0000-0001 1111-1111 1111-1111 1111-1111
	cr_expect_neq(ret, NULL);
	bitsSetInBitmap = 0x01FFFFFF;
	cr_expect_eq(someZone->blockBitmap[0], bitsSetInBitmap);

	allocationData = get_allocation_data(&someZone, 128, &g_smallAllocationZoneMetadata);
	ret = allocate_in_zone(allocationData); // [0] 1111-1111 1111-1111 1111-1111 1111-1111 [1] 0000-0011
	cr_expect_neq(ret, NULL);
	bitsSetInBitmap = 0xFFFFFFFF;
	cr_expect_eq(someZone->blockBitmap[0], bitsSetInBitmap);
	bitsSetInBitmap = 0x00000003;
	cr_expect_eq(someZone->blockBitmap[1], bitsSetInBitmap);

	allocationData = get_allocation_data(&someZone, 128, &g_smallAllocationZoneMetadata);
	ret = allocate_in_zone(allocationData); // [0] 1111-1111 1111-1111 1111-1111 1111-1111 [1] 0000-0111 1111-1111
	cr_expect_neq(ret, NULL);
	bitsSetInBitmap = 0xFFFFFFFF;
	cr_expect_eq(someZone->blockBitmap[0], bitsSetInBitmap);
	bitsSetInBitmap = 0x000007FF;
	cr_expect_eq(someZone->blockBitmap[1], bitsSetInBitmap);

}

Test(zone_test, free_single_allocation)
{
	// ==========================================================================================
	// THESE FOLLOWING TESTS ARE BASED ON DEFAULT SETTINGS, IF THEY ARE CHANGED TESTS MIGHT FAIL.
	// ==========================================================================================

	someZone = create_zone(&g_smallAllocationZoneMetadata, 0);
	cr_expect_neq(someZone, NULL);

	AllocationData allocationData = get_allocation_data(&someZone, 19, &g_smallAllocationZoneMetadata);
	void* ret;
	ret = allocate_in_zone(allocationData);
	cr_expect_neq(ret, NULL);

	AllocationHeader* allocation = get_allocation_header(ret);
	cr_expect_neq(allocation, NULL);

	free_from_zone(someZone, allocation);
	uint32_t bitsSetInBitmap = 0x0F; // 1111
	cr_expect_eq(someZone->blockBitmap[0] & bitsSetInBitmap, bitsSetInBitmap);
}

Test(zone_test, free_multiple)
{
	// ==========================================================================================
	// THESE FOLLOWING TESTS ARE BASED ON DEFAULT SETTINGS, IF THEY ARE CHANGED TESTS MIGHT FAIL.
	// ==========================================================================================

// create some allocations
	someZone = create_zone(&g_smallAllocationZoneMetadata, 0);
	cr_expect_neq(someZone, NULL);

	AllocationData allocationData = get_allocation_data(&someZone, 19, &g_smallAllocationZoneMetadata);
	void* alloc1 = allocate_in_zone(allocationData);
	cr_expect_neq(alloc1, NULL);

	allocationData = get_allocation_data(&someZone, 5, &g_smallAllocationZoneMetadata);
	void* alloc2 = allocate_in_zone(allocationData);
	cr_expect_neq(alloc2, NULL);

	allocationData = get_allocation_data(&someZone, 128, &g_smallAllocationZoneMetadata);
	void* alloc3 = allocate_in_zone(allocationData); // [0] 0000-0000 0000-0000 1111-1111 1111-1111
	cr_expect_neq(alloc3, NULL);

	allocationData = get_allocation_data(&someZone, 128, &g_smallAllocationZoneMetadata);
	void* alloc4 = allocate_in_zone(allocationData); // [0] 0000-0001 1111-1111 1111-1111 1111-1111
	cr_expect_neq(alloc4, NULL);

	allocationData = get_allocation_data(&someZone, 128, &g_smallAllocationZoneMetadata);
	void* alloc5 = allocate_in_zone(allocationData); // [0] 1111-1111 1111-1111 1111-1111 1111-1111 [1] 0000-0011
	cr_expect_neq(alloc5, NULL);

	allocationData = get_allocation_data(&someZone, 128, &g_smallAllocationZoneMetadata);
	void* alloc6 = allocate_in_zone(allocationData); // [0] 1111-1111 1111-1111 1111-1111 1111-1111 [1] 0000-0111 1111-1111
	cr_expect_neq(alloc6, NULL);

// start freeing
	free_from_zone(someZone, get_allocation_header(alloc1)); // [0] 1111-1111 1111-1111 1111-1111 1100-1111 [1] 0000-0111 1111-1111
	uint32_t bitsSetInBitmap = 0xFFFFFFCF;
	cr_expect_eq(someZone->blockBitmap[0], bitsSetInBitmap);
	bitsSetInBitmap = 0x000007FF;
	cr_expect_eq(someZone->blockBitmap[1], bitsSetInBitmap);

	free_from_zone(someZone, get_allocation_header(alloc6)); // [0] 1111-1111 1111-1111 1111-1111 1100-1111 [1] 0000-0011
	bitsSetInBitmap = 0xFFFFFFCF;
	cr_expect_eq(someZone->blockBitmap[0], bitsSetInBitmap);
	bitsSetInBitmap = 0x00000003;
	cr_expect_eq(someZone->blockBitmap[1], bitsSetInBitmap);

	free_from_zone(someZone, get_allocation_header(alloc4)); // [0] 1111-1110 0000-0000 1111-1111 1100-1111 [1] 0000-0011
	bitsSetInBitmap = 0xFE00FFCF;
	cr_expect_eq(someZone->blockBitmap[0], bitsSetInBitmap);
	bitsSetInBitmap = 0x00000003;
	cr_expect_eq(someZone->blockBitmap[1], bitsSetInBitmap);
	
	free_from_zone(someZone, get_allocation_header(alloc3)); // [0] 1111-1110 0000-0000 0000-0000 0100-1111 [1] 0000-0011
	bitsSetInBitmap = 0xFE00004F;
	cr_expect_eq(someZone->blockBitmap[0], bitsSetInBitmap);
	bitsSetInBitmap = 0x00000003;
	cr_expect_eq(someZone->blockBitmap[1], bitsSetInBitmap);

	free_from_zone(someZone, get_allocation_header(alloc5)); // [0] 0000-0000 0000-0000 0000-0000 0100-1111 [1] 0000-0000
	bitsSetInBitmap = 0x0000004F;
	cr_expect_eq(someZone->blockBitmap[0], bitsSetInBitmap);
	bitsSetInBitmap = 0x00000000;
	cr_expect_eq(someZone->blockBitmap[1], bitsSetInBitmap);
	
	free_from_zone(someZone, get_allocation_header(alloc2)); // 0000-1111 (only zone header is left)
	bitsSetInBitmap = 0x0000000F;
	cr_expect_eq(someZone->blockBitmap[0], bitsSetInBitmap);
}
