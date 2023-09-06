/* **************************************************************************** */
/*                                                                              */
/*  __   ___  ___      ___       __      ___      ___        ______    ______   */
/* |/"| /  ")|"  \    /"  |     /""\    |"  |    |"  |      /    " \  /" _  "\  */
/* (: |/   /  \   \  //   |    /    \   ||  |    ||  |     // ____  \(: ( \___) */
/* |    __/   /\\  \/.    |   /' /\  \  |:  |    |:  |    /  /    ) :)\/ \      */
/* (// _  \  |: \.        |  //  __'  \  \  |___  \  |___(: (____/ // //  \ _   */
/* |: | \  \ |.  \    /:  | /   /  \\  \( \_|:  \( \_|:  \\        / (:   _) \  */
/* (__|  \__)|___|\__/|___|(___/    \___)\_______)\_______)\"_____/   \_______) */
/*                                                                              */
/*                    Kingmar  |  https://github.com/K1ngmar                    */
/*                                                                              */
/* **************************************************************************** */

#include "zone.h"

#include <libkm/memory.h>

#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>

#define BITS_IN_BYTE 8
#define BITS_IN_INTEGER sizeof(int) / BITS_IN_BYTE

#ifndef SMALL_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES
# define SMALL_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES 16
#endif

#ifndef SMALL_ALLOCATION_MAX_ALLOCATION_SIZE_BYTES
# define SMALL_ALLOCATION_MAX_ALLOCATION_SIZE_BYTES 256
#endif

#ifndef SMALL_ALLOCATION_ZONE_SIZE_PAGES
# define SMALL_ALLOCATION_ZONE_SIZE_PAGES 4 /* 4096 bytes */
#endif

const ZoneMetadata g_smallAllocationZoneMetadata = {
    .minAllocationSizeInBytes	= SMALL_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES,
    .maxAllocationSizeInBytes	= SMALL_ALLOCATION_MAX_ALLOCATION_SIZE_BYTES,
    .zoneSizeInPages			= SMALL_ALLOCATION_ZONE_SIZE_PAGES,
    .bitfieldSize				= (SMALL_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES / BITS_IN_INTEGER)
};

#ifndef MEDIUM_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES
# define MEDIUM_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES 256
#endif

#ifndef MEDIUM_ALLOCATION_MAX_ALLOCATION_SIZE_BYTES
# define MEDIUM_ALLOCATION_MAX_ALLOCATION_SIZE_BYTES 2048
#endif

#ifndef MEDIUM_ALLOCATION_ZONE_SIZE_PAGES
# define MEDIUM_ALLOCATION_ZONE_SIZE_PAGES 8 /* 8192 bytes */
#endif

const ZoneMetadata g_mediumAllocationZoneMetadata = {
	.minAllocationSizeInBytes	= MEDIUM_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES,
    .maxAllocationSizeInBytes	= MEDIUM_ALLOCATION_MAX_ALLOCATION_SIZE_BYTES,
    .zoneSizeInPages			= MEDIUM_ALLOCATION_ZONE_SIZE_PAGES,
    .bitfieldSize				= (MEDIUM_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES / BITS_IN_INTEGER)
};

// Temproary define in future will be gotten from kernel.
#define PAGE_SIZE 1024

ZoneHeader* create_zone(const ZoneMetadata* zoneMetadata)
{
	void* data = mmap(NULL, zoneMetadata->zoneSizeInPages * PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (data == (void*)-1) {
		return NULL;
	}

	ZoneHeader* header = data;
	header->start = header_boundary_zone_start;
	header->metadata = zoneMetadata;
	header->nextZone = NULL;
	header->end = header_boundary_zone_end;

	return data;
}
