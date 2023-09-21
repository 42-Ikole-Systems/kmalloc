
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

    return 0;
}
