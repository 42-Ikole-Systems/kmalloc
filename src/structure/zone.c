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
#include <libkm/math.h>

#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

const ZoneMetadata g_smallAllocationZoneMetadata = {
    .minAllocationSizeInBytes	= SMALL_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES,
    .maxAllocationSizeInBytes	= SMALL_ALLOCATION_MAX_ALLOCATION_SIZE_BYTES,
    .zoneSizeInPages			= SMALL_ALLOCATION_ZONE_SIZE_PAGES,
    .bitmapSize					= SMALL_BLOCK_BITMAP_SIZE_BITS
};

const ZoneMetadata g_mediumAllocationZoneMetadata = {
	.minAllocationSizeInBytes	= MEDIUM_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES,
    .maxAllocationSizeInBytes	= MEDIUM_ALLOCATION_MAX_ALLOCATION_SIZE_BYTES,
    .zoneSizeInPages			= MEDIUM_ALLOCATION_ZONE_SIZE_PAGES,
    .bitmapSize					= MEDIUM_BLOCK_BITMAP_SIZE_BITS
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
	// do something with allocation header.

	const uint16_t allocationSizeInBlocks = km_ceil((float)allocationSizeInBytes / (float)(zone->metadata->minAllocationSizeInBytes));
	const size_t bitmapSize = zone->metadata->bitmapSize;
	
	size_t consecutiveFreeBlocks = 0;
	for (size_t i = 0; i < bitmapSize; i++)
	{
		// can be optimised by checking whole bytes or 4bytes at a time if they are full.
		const uint32_t bitToCheck = 1 << (i % bitmapSize);
		if (zone->blockBitmap[i / bitmapSize] & bitToCheck) {
			consecutiveFreeBlocks++;
		}
		else {
			consecutiveFreeBlocks = 0;
		}
		
		if (consecutiveFreeBlocks == allocationSizeInBlocks)
		{
			const size_t firstBlockOfAllocation = i + 1 - allocationSizeInBlocks;
			set_bitmap_occupied(zone, firstBlockOfAllocation)
			// actually create allocation here.
			return (void*)zone + sizeof(ZoneHeader) + (firstBlockOfAllocation * (zone->metadata->minAllocationSizeInBytes));
		}
	}

	return NULL;
}
