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
 * @param sizeWithHeader in bytes of object attempting to allocate
*/
static slab_header* get_slab(zone_header* zone, size_t sizeWithHeader)
{
    assert(sizeWithHeader <= KMALLOC_LARGE_SLAB_ALLOCATION_MAX_SIZE);

    const slab_metadata slabMetadata = get_slab_metadata(sizeWithHeader);
    slab_header* slab = NULL;
    
    // get either large or small slab
    while (slab != NULL)
    {

        if (slab_has_space(slab, sizeWithHeader)) {
            break ;
        }

        slab = slab->nextSlab;
    }

    // maybe store previous so no looping twice..
    if (slab == NULL) {
        slab = create_new_slab();
    }

    return slab;
}

void* zone_allocate(zone_header* zone, size_t sizeWithHeader)
{
    // check slab destination
    // check slab availability
    // possibly create new zone
}

bool can_store_allocation_in_zone(size_t sizeWithHeader)
{
    return (sizeWithHeader <= KMALLOC_LARGE_SLAB_ALLOCATION_MAX_SIZE);
}
