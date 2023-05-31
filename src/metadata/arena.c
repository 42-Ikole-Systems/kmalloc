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

#include "kmalloc/metadata/arena.h"

#include <stddef.h>
#include <assert.h>

void initialise_arena(kmalloc_arena* arena)
{
    arena->zones = NULL;
    arena->hugeAllocations = NULL;
    arena->capacity = 0;
    arena->size = 0;
}

void* arena_allocate(kmalloc_arena* arena, size_t size)
{
    void* allocation = NULL;
    if (size <= KMALLOC_LARGE_SLAB_ALLOCATION_MAX_SIZE)
    {
        if (arena->zones == NULL)
        {
            arena->zones = create_new_zone();
            if (arena->zones == NULL) {
                return NULL;
            }
        }
        allocation = zone_allocate(arena->zones);
    }
    else
    {
        // its a huge allocation
    }
    if (allocation) {
        // how to do the capacity?
        arena->size += size;
    }
    return allocation;
}
