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
#include <sys/mman.h>

zone_header* create_new_zone()
{
    void* zonePageRun = mmap(NULL, KMALLOC_ZONE_SIZE, PROT_READ | PROT_WRITE, MAP_ANON, 0, 0);
    if (zonePageRun == NULL) {
        return NULL;
    }

    zone_header* header = (zone_header*)zonePageRun;
    header->__header_start = zone_header_start;
    header->capacityInPages = KMALLOC_ZONE_SIZE / KMALLOC_PAGE_SIZE;
    header->freePages = header->capacityInPages;
    header->nextFreePage = zonePageRun;
    header->smallSlabs = NULL;
    header->largeSlabs = NULL;
    header->nextZone = NULL;
    header->__header_end = zone_header_end;
    return header;
}

/*!
 * @brief
 * @param zone
 * @param size in bytes of object attempting to allocate
*/
static slab_header* get_slab(zone_header* zone, size_t size)
{
    assert(size < KMALLOC_LARGE_SLAB_ALLOCATION_MAX_SIZE);

    slab_header* slab = NULL;
    size_t slabMinAllocationSize = 0;
    if (size <= KMALLOC_SMALL_SLAB_ALLOCATION_MAX_SIZE) {
        slab = zone->smallSlabs;
        slabMinAllocationSize = KMALLOC_SMALL_SLAB_ALLOCATION_MIN_SIZE;
    }
    else {
        slab = zone->largeSlabs;
        slabMinAllocationSize = KMALLOC_LARGE_SLAB_ALLOCATION_MIN_SIZE;
    }

    size_t actualSizeInBytes = ((size_t)km_ceil((double)size / slabMinAllocationSize)) * slabMinAllocationSize;
    while (slab != NULL)
    {
        if (slab->)

        slab = slab->nextSlab;
    }

    return slab;
}

void* zone_allocate(zone_header* zone, size_t size)
{
    // check slab destination
    // check slab availability
    // possibly create new zone
}
