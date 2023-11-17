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
#include "structure/allocation.h"
#include "debug_assert.h"

#include <unistd.h>
#include <stdlib.h>

void km_free(void* ptr)
{
	if (ptr == NULL)
	{
		return;
	}

	// maybe a different header for large allocati`ons.
	AllocationHeader* allocation = get_allocation_header(ptr);
	if (allocation == NULL) {
		abort(); // Not a valid allocation header.
	}

	ZoneHeader* zone = get_zone_header(allocation);
	if (zone == NULL) {
		abort(); // No zone header was found for allocation.
	}
	free_from_zone(zone, allocation);
	// remove zone if empty.
}
