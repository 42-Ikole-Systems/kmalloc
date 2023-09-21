
#include <kmalloc/kmalloc.h>
#include "structure/zone.h"

#include <libkm/io/printf.h>

int main()
{
    km_printf("small bitmapSize bits: %d, sizeInIntegers: %d\n", SMALL_BLOCK_BITMAP_SIZE_BITS, BLOCK_BITMAP_SIZE_INTEGERS);
    ZoneHeader* someZone = create_zone(&g_smallAllocationZoneMetadata);
    if (someZone == NULL) {
        return 1;
    }

    uint32_t bitsSetInBitmap = 0x0F; // 1111
	km_printf("block_bitmap: %d, expected to be set, %d\n", someZone->blockBitmap[0], bitsSetInBitmap);
	
	allocate_in_zone(someZone, 19);
	bitsSetInBitmap = 0x3F; // 0011-1111
	km_printf("block_bitmap: %X, expected to be set, %X\n", someZone->blockBitmap[0], bitsSetInBitmap);

    return 0;
}
