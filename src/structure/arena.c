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

#include "arena.h"
#include "allocation.h"

#include <unistd.h>
#include <pthread.h>
#include <assert.h>

size_t get_thread_arena_index(const size_t amountOfArenas)
{
#ifdef __linux__
	const pthread_id_np_t threadId = pthread_getthreadid_np();
#elif defined(__APPLE__)
	uint64_t threadId;
	pthread_threadid_np(NULL, &threadId);
#endif

	return threadId % amountOfArenas;
}

/*!
 * @brief Calculates the amount of blocks allocationSize will take up in the zone.
 * @param zone
 * @param allocationSizeInBytes
 * @return
*/
static uint16_t get_allocation_size_in_blocks(const ZoneHeader* zone, size_t allocationSizeInBytes)
{
	return km_ceil((float)(allocationSizeInBytes + sizeof(AllocationHeader)) / (float)(zone->metadata->minAllocationSizeInBytes));
}

AllocationData get_zone_for_allocation(const ZoneHeader* zoneHead, const size_t allocationSizeInBytes, const ZoneMetadata* zoneMetadata)
{
	const uint16_t allocationSizeInBlocks = get_allocation_size_in_blocks(zoneMetadata, allocationSizeInBytes);
	AllocationData allocationData = {NULL, 0, allocationSizeInBlocks};

	for (const ZoneHeader* zone = zoneHead; zone != NULL; zone = zone->nextZone)
	{
		const size_t firstBlockOfAllocation = get_allocation_block_in_zone(zone, allocationSizeInBlocks);
		if (firstBlockOfAllocation != 0) {
			allocationData.zone = zone;
			allocationData.firstBlockOfAllocation = firstBlockOfAllocation;
			break;
		}
	}
	if (allocationData.zone == NULL) {
		create_zone(zoneHead->metadata);
	}
	return allocationData;
}

void* allocate_in_arena(Arena* arena, const size_t allocationSizeInBytes)
{
	assert(arena != NULL);
	AllocationData* allocationData = {NULL, 0, 0};

	if (allocationSizeInBytes < g_smallAllocationZoneMetadata.maxAllocationSizeInBytes) {
		allocationData = get_zone_for_allocation(arena->smallZones, allocationSizeInBytes, &g_smallAllocationZoneMetadata);
	}
	else if (allocationSizeInBytes < g_mediumAllocationZoneMetadata.maxAllocationSizeInBytes) {
		allocationData = get_zone_for_allocation(arena->mediumZones, allocationSizeInBytes, &g_mediumAllocationZoneMetadata);
	}
	else {
		// Create large allocation.
	}

	const void* allocation = allocate_in_zone(allocationData.zone, allocationSizeInBytes);

	if (allocation != NULL) {
		arena->size += allocationSizeInBytes;
		arena->capacity -= allocationSizeInBytes;
	}
	return allocation;
}
