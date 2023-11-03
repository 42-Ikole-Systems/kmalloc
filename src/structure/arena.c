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

#include <libkm/math.h>

#include <unistd.h>
#include <pthread.h>
#include <assert.h>

#ifdef __linux__
# include <sys/syscall.h>
# include <sys/types.h>
#endif

size_t get_thread_arena_index(const size_t amountOfArenas)
{
#ifdef __linux__
	const pid_t threadId = syscall(SYS_gettid);
#elif defined(__APPLE__)
	uint64_t threadId;
	pthread_threadid_np(NULL, &threadId);
#else
# pramga message "Function not implemented for this platform."
#endif

	return threadId % amountOfArenas;
}

/*!
 * @brief Adds a new zone after (if supplied) previous zone, else stored in zoneHead;
 * @param zoneHead
 * @param previousZone
 * @param zoneMetadata
 * @return Zone just allocated.
*/
static ZoneHeader* add_new_zone(ZoneHeader** zoneHead, ZoneHeader* previousZone, const ZoneMetadata* zoneMetadata)
{
	ZoneHeader* newZone = create_zone(zoneMetadata);
	if (newZone == NULL) {
		return NULL;
	}
	if (previousZone != NULL) {
		previousZone->nextZone = newZone;
	}
	else {
		assert(zoneHead != NULL);
		*zoneHead = newZone;
	}
	return newZone;
}

AllocationData get_allocation_data(ZoneHeader** zoneHead, const size_t allocationSizeInBytes, const ZoneMetadata* zoneMetadata)
{
	assert(zoneHead != NULL);
	const uint16_t allocationSizeInBlocks = get_allocation_size_in_blocks(zoneMetadata, allocationSizeInBytes);
	AllocationData allocationData = {NULL, 0, allocationSizeInBlocks};

	ZoneHeader* previousZone = NULL;
	for (ZoneHeader* zone = *zoneHead; zone != NULL; zone = zone->nextZone)
	{
		const size_t firstBlockOfAllocation = get_allocation_block_in_zone(zone, allocationSizeInBlocks);
		if (firstBlockOfAllocation != 0) {
			allocationData.zone = zone;
			allocationData.firstBlockOfAllocation = firstBlockOfAllocation;
			break;
		}
		previousZone = zone;
	}

	if (allocationData.zone == NULL)
	{
		// No valid zone was found, we need to allocate a new one.
		ZoneHeader* newZone = add_new_zone(zoneHead, previousZone, zoneMetadata);
		if (newZone != NULL)
		{
			const size_t firstBlockOfAllocation = get_allocation_block_in_zone(newZone, allocationSizeInBlocks);
			assert(firstBlockOfAllocation != 0);

			allocationData.zone = newZone;
			allocationData.firstBlockOfAllocation = firstBlockOfAllocation;
		}
	}
	return allocationData;
}

void* allocate_in_arena(Arena* arena, const size_t allocationSizeInBytes)
{
	assert(arena != NULL);
	AllocationData allocationData = {NULL, 0, 0};

	if (allocationSizeInBytes < g_smallAllocationZoneMetadata.maxAllocationSizeInBytes) {
		allocationData = get_allocation_data(&(arena->smallZones), allocationSizeInBytes, &g_smallAllocationZoneMetadata);
	}
	else if (allocationSizeInBytes < g_mediumAllocationZoneMetadata.maxAllocationSizeInBytes) {
		allocationData = get_allocation_data(&(arena->mediumZones), allocationSizeInBytes, &g_mediumAllocationZoneMetadata);
	}
	else {
		// Create large allocation.
	}

	void* allocation = allocate_in_zone(allocationData);

	if (allocation != NULL) {
		arena->size += allocationSizeInBytes;
		arena->capacity -= allocationSizeInBytes;
	}
	return allocation;
}
