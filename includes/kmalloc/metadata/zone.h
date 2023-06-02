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

#ifndef KMALLOC_ZONE_H
# define KMALLOC_ZONE_H

# include "kmalloc/metadata/slab.h"

# include <inttypes.h>
# include <stdbool.h>

/*!
 * @brief in bytes
*/
# define MiB KiB * KiB

/*!
 * @brief in bytes
*/
# define KMALLOC_ZONE_SIZE (2 * MiB)

/*!
 * @brief temporary define!!!!!
*/
# define KMALLOC_PAGE_SIZE 4096

/*!
 * @brief -.
 */
typedef enum
{
	zone_header_start = 0x01,
	zone_header_end = 0x02
} zone_header_boundries;

/*!
 * @brief header containing metadata of a zone
*/
typedef struct s_zone_header
{
	uint8_t __header_start : 8;
	uint16_t capacityInPages : 16;
    uint16_t freePages : 16; /*! @brief amount of pages not assigned to a slab */
    void* nextFreePage;
    slab_header* smallSlabs; /*! @brief header of first slab, NULL if none exist. Will contain pointer to next slab */
    slab_header* largeSlabs; /*! @brief header of first slab, NULL if none exist. Will contain pointer to next slab */
	struct s_zone_header* nextZone;
	uint8_t __header_end : 8;
} zone_header;

/*!
 * @brief allocates space for a zone and intialises its header
 * @return
*/
zone_header* create_new_zone();

/*!
 * @brief creates an allocation within a zone
 * @param zone
 * @param sizeWithHeader in bytes
 * @return
 * 
 * @note allocates a new zone if no space is available in the current
*/
void* zone_allocate(zone_header* zone, size_t sizeWithHeader);

/*!
 * @brief checks whether the allocation can be stored within a preallocated zone
 * @param sizeWithHeader
 * @return
*/
bool can_store_allocation_in_zone(size_t sizeWithHeader);

#endif
