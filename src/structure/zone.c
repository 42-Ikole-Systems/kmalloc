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
#include "allocation.h"
#include "../debug_assert.h"

#include <libkm/memory.h>
#include <libkm/math.h>

#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>

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

uint16_t get_allocation_size_in_blocks(const ZoneMetadata* zoneMetadata, size_t allocationSizeInBytes)
{
	return km_ceil((float)(allocationSizeInBytes + sizeof(AllocationHeader)) / (float)(zoneMetadata->minAllocationSizeInBytes));
}

/*!
 * @brief Sets blocks as used in bitmap.
 * @param zone
 * @param startingBlock index of the block the occupation starts.
 * @param allocationSizeInBlocks
*/
static void set_bitmap_occupied(ZoneHeader* zone, size_t startingBlock, size_t allocationSizeInBlocks)
{
	const size_t bitmapSize = zone->metadata->bitmapSize;

	for (size_t i = startingBlock; i < bitmapSize && i < startingBlock + allocationSizeInBlocks; i++)
	{
		const size_t bitToMark = 1 << (i % BITS_IN_INTEGER);
		zone->blockBitmap[i / BITS_IN_INTEGER] |= bitToMark;
	}
	zone->freeBlocks -= allocationSizeInBlocks;
}

ZoneHeader* create_zone(const ZoneMetadata* zoneMetadata, size_t arenaIndex)
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
	header->freeBlocks = zoneMetadata->bitmapSize; // size of bitmap has a 1:1 correlation with the amount of blocks in a zone. 
	header->arenaIndex = arenaIndex;
	header->nextZone = NULL;
	header->end = header_boundary_zone_end;

	// Mark blocks containing zoneHeader as occupied in bitmap.
	set_bitmap_occupied(header, 0, get_allocation_size_in_blocks(zoneMetadata, sizeof(ZoneHeader)));

	return header;
}

ZoneHeader* get_zone_header(AllocationHeader* allocation)
{
	ZoneHeader* header = (ZoneHeader*)((void*)allocation - (allocation->zoneOffsetper16Bytes * 16));

	if (header->start != header_boundary_zone_start || header->end != header_boundary_zone_end) {
		return NULL;
	}
	return header;
}

void destroy_zone(ZoneHeader* zone)
{
	const ZoneMetadata* zoneData = zone->metadata;
	zone->start = 0;
	zone->metadata = NULL;
	zone->nextZone = NULL;
	zone->end = 0;

	const int ret = munmap(zone, zoneData->zoneSizeInPages * PAGE_SIZE);
	D_ASSERT(ret != -1); // Can only fail if zone is an invalid address.
	(void)ret;
}

size_t get_allocation_block_in_zone(const ZoneHeader* zone, size_t allocationSizeInBlocks)
{
	size_t firstBlockOfAllocation = 0;
	size_t consecutiveFreeBlocks = 0;
	// we can start looking after the first block because it will always be taken for the ZoneHeader.
	for (size_t i = 1; i < zone->metadata->bitmapSize; i++)
	{
		// can be optimised by checking sizeof(int) bytes at a time if they are full.
		const size_t bitToCheck = 1 << (i % BITS_IN_INTEGER);
		if (!(zone->blockBitmap[i / BITS_IN_INTEGER] & bitToCheck)) {
			consecutiveFreeBlocks++;
		}
		else {
			consecutiveFreeBlocks = 0;
		}
		
		if (consecutiveFreeBlocks == allocationSizeInBlocks) {
			firstBlockOfAllocation = i + 1 - allocationSizeInBlocks;
			break;
		}
	}
	return firstBlockOfAllocation;
}

void* allocate_in_zone(const AllocationData allocation)
{
	set_bitmap_occupied(allocation.zone, allocation.firstBlockOfAllocation, allocation.allocationSizeInBlocks);

	void* allocationAddress = (void*)allocation.zone + (allocation.firstBlockOfAllocation * (allocation.zone->metadata->minAllocationSizeInBytes));
	const uint16_t stepsToZoneHead = ((intptr_t)allocationAddress - (intptr_t)allocation.zone) >> 4; // divides by 16.
	set_allocation_header(allocationAddress, allocation.allocationSizeInBlocks, stepsToZoneHead);

	return allocationAddress + sizeof(AllocationHeader);
}

/*!
 * @brief -.
 * @param zone
 * @param allocation
*/
static void clear_allocation_from_bitmap(ZoneHeader* zone, AllocationHeader* allocation)
{
	const size_t bitmapSize = zone->metadata->bitmapSize;
	const size_t startingBlock = ((intptr_t)allocation - (intptr_t)zone) / zone->metadata->minAllocationSizeInBytes;

	for (size_t bitToClear = startingBlock; bitToClear < bitmapSize && bitToClear < startingBlock + allocation->sizeInBlocks; bitToClear++)
	{
		zone->blockBitmap[bitToClear / BITS_IN_INTEGER] &= ~(1 << bitToClear);
	}
	zone->freeBlocks += allocation->sizeInBlocks;
}

void free_from_zone(ZoneHeader* header, AllocationHeader* allocation)
{
	clear_allocation_from_bitmap(header, allocation);
	km_memset(allocation, 0, sizeof(AllocationHeader));
}

bool zone_is_empty(const ZoneHeader* zone)
{
	// Check if there is either nothing or only the zoneHeader is allocated.
	return (zone->freeBlocks <= ((zone->metadata->zoneSizeInPages * PAGE_SIZE) / zone->metadata->minAllocationSizeInBytes) - get_allocation_size_in_blocks(zone->metadata, sizeof(ZoneHeader)));
}
