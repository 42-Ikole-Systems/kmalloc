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

#ifndef KMALLOC_ARENA_H
# define KMALLOC_ARENA_H

#include "zone.h"
#include "allocation.h"

#include <pre_preprocessor/generated_kmalloc_metadata.h>
#include <inttypes.h>

/*!
 * @brief Struct containing information about a single arena.
*/
typedef struct Arena_s
{
	ZoneHeader*		smallZones; /*!< Points to the first zone, Null if no zones exist yet. */
	ZoneHeader*		mediumZones; /*!< Points to the first zone, Null if no zones exist yet. */
	void*			huge_allocations; /*!< (binary tree?) of huge allocations (not contained in zones). */
	uint64_t		capacity; /*!< Capacity in bytes. */
	uint64_t		size; /*!< Used size in bytes. */    
} Arena;

/*!
 * @brief -.
 * @param idx
 * @return
*/
Arena* get_arena_by_index(size_t idx);

/*!
 * @brief Gets the arena assigned to the thread.
 * @return
*/
Arena* get_arena();

/*!
 * @brief Gets the index for the arena assigned to the thread.
 * @returns
*/
size_t get_thread_arena_index();

/*!
 * @brief Creates an allocation within arena.
 * @param arena
 * @param numberOfBytes
 * @return address of allocation.
*/
void* allocate_in_arena(Arena* arena, const size_t numberOfBytes);

/*!
 * @brief Retrieves all information needed in orderd to create an allocation.
 * @param zoneHead
 * @param allocationSizeInBytes
 * @param zoneMetadata
 * @return
*/
AllocationData get_allocation_data(ZoneHeader** zoneHead, const size_t allocationSizeInBytes, const ZoneMetadata* zoneMetadata);

/*!
 * @brief Deallocates and removes zone from arena.
 * @param arena
 * @param zoneToDelete
*/
void remove_zone_from_arena(Arena* arena, ZoneHeader* zoneToDelete);

#endif
