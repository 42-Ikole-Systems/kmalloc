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

#include "kmalloc/kmalloc.h"
#include "structure/zone.h"
#include "structure/arena.h"
#include "structure/allocation.h"
#include "debug_assert.h"

#include <unistd.h>
#include <stdlib.h>

void free_small_or_large(AllocationHeader* allocation)
{
	ZoneHeader* zone = get_zone_header(allocation);
	if (zone == NULL) {
		abort(); // No zone header was found for allocation.
	}
	Arena* arena = get_arena_by_index(zone->arenaIndex);
	// lock mutex;

	free_from_zone(zone, allocation);
	
	if (zone_is_empty(zone)) {
		remove_zone_from_arena(arena, zone);
	}
}

void km_free(void* ptr)
{
	if (ptr == NULL)
	{
		return;
	}

	AllocationHeader* allocation = get_allocation_header(ptr);
	if (allocation != NULL) {
		free_small_or_large(allocation);
	}
	// maybe a different header for large allocations.
	
	// abort(); // Not a valid allocation header.
}
