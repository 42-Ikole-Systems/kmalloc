
#include <kmalloc/kmalloc.h>
#include "structure/zone.h"
#include <assert.h>

#include <libkm/io/printf.h>

void simpleZoneTest()
{
	km_printf("small bitmapSize bits: %d, sizeInIntegers: %d\n", SMALL_BLOCK_BITMAP_SIZE_BITS, BLOCK_BITMAP_SIZE_INTEGERS);
    ZoneHeader* someZone = create_zone(&g_smallAllocationZoneMetadata);
    if (someZone == NULL) {
        return ;
    }

    uint32_t bitsSetInBitmap = 0x0F; // 1111
	km_printf("block_bitmap: %d, expected to be set, %d\n", someZone->blockBitmap[0], bitsSetInBitmap);
	
	// allocate_in_zone(someZone, 19);
	// bitsSetInBitmap = 0x3F; // 0011-1111
	// km_printf("block_bitmap: %X, expected to be set, %X\n", someZone->blockBitmap[0], bitsSetInBitmap);

	// allocate_in_zone(someZone, 5);
	// bitsSetInBitmap = 0x7F; // 0111-1111
	// km_printf("block_bitmap: %X, expected to be set, %X\n", someZone->blockBitmap[0], bitsSetInBitmap);

	// allocate_in_zone(someZone, 128); // [0] 0000-0000 0000-0000 1111-1111 1111-1111
	// bitsSetInBitmap = 0x0000FFFF;
	// km_printf("block_bitmap: %X, expected to be set, %X\n", someZone->blockBitmap[0], bitsSetInBitmap);

	// allocate_in_zone(someZone, 128); // [0] 0000-0001 1111-1111 1111-1111 1111-1111
	// bitsSetInBitmap = 0x01FFFFFF;
	// km_printf("block_bitmap: %X, expected to be set, %X\n", someZone->blockBitmap[0], bitsSetInBitmap);

	// assert(allocate_in_zone(someZone, 128) != NULL); // [0] 1111-1111 1111-1111 1111-1111 1111-1111 [1] 0000-0011
	// bitsSetInBitmap = 0xFFFFFFFF;
	// km_printf("block_bitmap[0]: %X, expected to be set, %X\n", someZone->blockBitmap[0], bitsSetInBitmap);
	// bitsSetInBitmap = 0x00000003;
	// km_printf("block_bitmap[1]: %X, expected to be set, %X\n", someZone->blockBitmap[1], bitsSetInBitmap);

	// assert(allocate_in_zone(someZone, 128) != NULL); // [0] 1111-1111 1111-1111 1111-1111 1111-1111 [1] 0000-0111 1111-1111
	// bitsSetInBitmap = 0xFFFFFFFF;
	// km_printf("block_bitmap[0]: %X, expected to be set, %X\n", someZone->blockBitmap[0], bitsSetInBitmap);
	// bitsSetInBitmap = 0x000007FF;
	// km_printf("block_bitmap[1]: %X, expected to be set, %X\n", someZone->blockBitmap[1], bitsSetInBitmap);
}

int main()
{
    // simpleZoneTest();
	void* var = km_malloc(16);
	km_free(var);


	return 0;
}
