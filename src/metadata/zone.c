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
 * @brief gets the slab where an allocation of sizeWithHeader will fit (creates new slab when necessary)
 * @param zone
 * @param sizeWithHeader in bytes of object attempting to allocate
 * @return
*/
static slab_header* get_slab(zone_header* zone, size_t sizeWithHeader)
{
    assert(sizeWithHeader <= KMALLOC_LARGE_SLAB_ALLOCATION_MAX_SIZE);

    const slab_metadata slabMetadata = get_slab_metadata(sizeWithHeader);
    slab_header** slabHead = get_slab_head_from_zone(zone, sizeWithHeader);
    
    if (slabHead == NULL) {
        return NULL; // should never happen
    }

    slab_header* slab = *slabHead;
    slab_header* prev = NULL;
    void* region;
    while (slab != NULL)
    {
        region = get_region_in_slab(slab, sizeWithHeader, slabMetadata);
        if (region != NULL) {
            return slab;
        }

        prev = slab;
        slab = slab->nextSlab;
    }

    // no slab of this type exists yet.
    if (slab == NULL && prev == NULL)
    {
        *slabHead = create_new_slab(zone, slabMetadata);
        if (*slabHead == NULL) {
            return NULL;
        }
        slab = *slabHead;
    }
    else if (slab == NULL)
    {
        prev->nextSlab = create_new_slab(zone, slabMetadata);
        if (prev->nextSlab == NULL) {
            return NULL;
        }
        slab = prev->nextSlab;
    }

    return slab;
}

void* zone_allocate(zone_header* zone, size_t sizeWithHeader)
{
    slab_header* slab = NULL;
    while (zone)
    {
        slab = get_slab(zone, sizeWithHeader);
        if (slab != NULL) {
            break ;
        }

        if (zone->nextZone == NULL) {
            zone->nextZone = create_new_zone();
        }
        zone = zone->nextZone;
    }
    if (slab == NULL) {
        return NULL;
    }

    return slab_allocate(slab, sizeWithHeader);
}

bool can_store_allocation_in_zone(size_t sizeWithHeader)
{
    return (sizeWithHeader <= KMALLOC_LARGE_SLAB_ALLOCATION_MAX_SIZE);
}
