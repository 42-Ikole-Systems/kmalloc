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

#ifndef KMALLOC_ARENA_HEADER_H
# define KMALLOC_ARENA_HEADER_H

# include "kmalloc/metadata/zone.h"

# include <inttypes.h>
# include <stddef.h>

/*!
 * @brief header containing metadata of an arena
*/
typedef struct
{
	zone_header* zones; /*! @brief points to the first zone, Null if not allocated. the zone will contain a pointer to the next zone */
	void* hugeAllocations; /*! @brief (map?) of huge allocations (not contained in a slab, has its own zone) */
	uint64_t capacity; /*! @brief capacity in bytes*/
	uint64_t size; /*! @brief used size in bytes */
} kmalloc_arena;

/*!
 * @brief intialises an empty arena
 * @param arena
*/
void initialise_arena(kmalloc_arena* arena);

/*!
 * @brief
 * @param arena
 * @param size
 * @return
*/
void* arena_allocate(kmalloc_arena* arena, size_t size);

#endif
