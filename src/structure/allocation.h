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

#ifndef KMALLOC_ALLOCATION_H
# define KMALLOC_ALLOCATION_H

# include "boundaries.h"

# include <inttypes.h>

/*!
 * @brief Header stored in front of each allocation.
*/
typedef struct AllocationHeader_s
{
    HeaderBoundaries start : 3; /*!< -. */
    uint16_t		 sizeInBlocks : 13; /*!< -. */
	uint16_t		 zoneOffsetper16Bytes : 13; /*!< Amount of 16byte steps it takes to get back to the zone header. */
    HeaderBoundaries end : 3; /*!< -. */
} AllocationHeader;

/*!
 * @brief Sets allocation header at address.
 * @param address
 * @param sizeInBlocks Size of the allocation.
 * @param zoneOffsetper16Bytes Amount of 16byte steps it takes to get back to the zone header.
*/
void set_allocation_header(void* address, uint16_t sizeInBlocks, uint16_t zoneOffsetper16Bytes);

/*!
 * @brief Gets allocation header.
 * @param allocationAddress
 * @return Null if it is not an allocation.
*/
AllocationHeader* get_allocation_header(void* allocationAddress);

/*!
 * @brief Invalidates allocation header.
 * @param header
*/
void remove_allocation_header(AllocationHeader* header);

#endif
