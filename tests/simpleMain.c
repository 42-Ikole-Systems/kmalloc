
#include <kmalloc/kmalloc.h>
#include "structure/zone.h"

int main()
{
    ZoneHeader* someZone = create_zone(&g_smallAllocationZoneMetadata);
	
    if (someZone == NULL) {
        return 1;
    }
	// First bit is set for zone header.
	// uint32_t bitsSetInBitmap = 1; // 0001

    return 0;
}
