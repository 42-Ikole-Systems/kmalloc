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
#include "../debug_assert.h"

#include <pre_preprocessor/generated_kmalloc_metadata.h>
#include <libkm/math.h>

#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__
# include <sys/syscall.h>
# include <sys/types.h>
#endif

Arena* get_arena_by_index(size_t idx)
{
	// No need to initialize since statics will be zero initialised by default.
	static Arena arenas[KMALLOC_NUMBER_OF_CORES];
	
	D_ASSERT(idx < KMALLOC_NUMBER_OF_CORES);
	
	return &arenas[idx];
}

Arena* get_arena()
{
	const size_t arenaIndex = get_thread_arena_index(KMALLOC_NUMBER_OF_CORES);
	return get_arena_by_index(arenaIndex);
}

size_t get_thread_arena_index()
{
#ifdef __linux__
	const pid_t threadId = syscall(SYS_gettid);
#elif defined(__APPLE__)
	uint64_t threadId;
	pthread_threadid_np(NULL, &threadId);
#else
# pramga message "Function not implemented for this platform."
#endif

	return threadId % KMALLOC_NUMBER_OF_CORES;
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
	ZoneHeader* newZone = create_zone(zoneMetadata, get_thread_arena_index());
	if (newZone == NULL) {
		return NULL;
	}
	if (previousZone != NULL) {
		previousZone->nextZone = newZone;
	}
	else {
		D_ASSERT(zoneHead != NULL);
		*zoneHead = newZone;
	}
	return newZone;
}

AllocationData get_allocation_data(ZoneHeader** zoneHead, const size_t allocationSizeInBytes, const ZoneMetadata* zoneMetadata)
{
	D_ASSERT(zoneHead != NULL);
	const uint16_t allocationSizeInBlocks = get_allocation_size_in_blocks(zoneMetadata, allocationSizeInBytes);
	AllocationData allocationData = {NULL, 0, allocationSizeInBlocks};

	ZoneHeader* previousZone = NULL;
	for (ZoneHeader* zone = *zoneHead; zone != NULL; zone = zone->nextZone)
	{
		if (zone->freeBlocks >= allocationSizeInBlocks)
		{
			const size_t firstBlockOfAllocation = get_allocation_block_in_zone(zone, allocationSizeInBlocks);
			if (firstBlockOfAllocation != 0) {
				allocationData.zone = zone;
				allocationData.firstBlockOfAllocation = firstBlockOfAllocation;
				break;
			}
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
			D_ASSERT(firstBlockOfAllocation != 0);

			allocationData.zone = newZone;
			allocationData.firstBlockOfAllocation = firstBlockOfAllocation;
		}
	}
	return allocationData;
}

void* allocate_in_arena(Arena* arena, const size_t allocationSizeInBytes)
{
	D_ASSERT(arena != NULL);
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

void remove_zone_from_arena(Arena* arena, ZoneHeader* zoneToDelete)
{
	ZoneHeader** zonesToSearchHead = NULL;

	if (zoneToDelete->metadata->minAllocationSizeInBytes == g_smallAllocationZoneMetadata.minAllocationSizeInBytes) {
		zonesToSearchHead = &(arena->smallZones);
	}
	else {
		zonesToSearchHead = &(arena->mediumZones);
	}
	
	ZoneHeader* previousZone = NULL;
	for (ZoneHeader* zone = *zonesToSearchHead; zone != NULL; zone = zone->nextZone) {
		if (zone == zoneToDelete) {
			if (previousZone == NULL) {
				*zonesToSearchHead = zone->nextZone;
			}
			else {
				previousZone->nextZone = zone->nextZone;
			}
			if (munmap(zone, zone->metadata->zoneSizeInPages * PAGE_SIZE) == -1) {
				perror("unmapping zone: ");
				abort();
			}
			return;
		}
		previousZone = zone;
	}
	assert("Zone was not deleted..." == NULL);
}
