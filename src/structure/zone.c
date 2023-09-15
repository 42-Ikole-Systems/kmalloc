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
#include <assert.h>

const ZoneMetadata g_smallAllocationZoneMetadata = {
    .minAllocationSizeInBytes	= SMALL_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES,
    .maxAllocationSizeInBytes	= SMALL_ALLOCATION_MAX_ALLOCATION_SIZE_BYTES,
    .zoneSizeInPages			= SMALL_ALLOCATION_ZONE_SIZE_PAGES,
    .bitfieldSize				= ((SMALL_ALLOCATION_ZONE_SIZE_PAGES * PAGE_SIZE) / SMALL_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES)
};

const ZoneMetadata g_mediumAllocationZoneMetadata = {
	.minAllocationSizeInBytes	= MEDIUM_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES,
    .maxAllocationSizeInBytes	= MEDIUM_ALLOCATION_MAX_ALLOCATION_SIZE_BYTES,
    .zoneSizeInPages			= MEDIUM_ALLOCATION_ZONE_SIZE_PAGES,
    .bitfieldSize				= ((MEDIUM_ALLOCATION_ZONE_SIZE_PAGES * PAGE_SIZE) / MEDIUM_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES)
};

ZoneHeader* create_zone(const ZoneMetadata* zoneMetadata)
{
	// MAP_ANON so its not associated with any file.
	// MAP_PRIVATE so a forked process will not share the memory but gets a dupliate.
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

void destroy_zone(ZoneHeader* zone)
{
	const ZoneMetadata* zoneData = zone->metadata;
	zone->start = 0;
	zone->metadata = NULL;
	zone->nextZone = NULL;
	zone->end = 0;

	int ret = munmap(zone, zoneData->zoneSizeInPages * PAGE_SIZE);
	assert(ret != -1); // Can only fail if zone is an invalid address.
}

void* allocate_in_zone(ZoneHeader* zone, size_t allocationSizeInBytes)
{
	const uint16_t allocationSizeInBlocks = ceil((float)allocationSizeInBytes / (float)zone->metadata->minAllocationSizeInBytes);


}
