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

#include "boundaries.h"
#include "sizeInfo.h"

#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>

/*!
 * @brief Information about allocation sizes and other metadata needed for a zone.
*/
typedef struct ZoneMetadata_s
{
	const uint32_t	minAllocationSizeInBytes; /*!< -. */
	const uint32_t	maxAllocationSizeInBytes; /*!< -. */
	const uint32_t	zoneSizeInPages; /*!< -. */
	const uint32_t	bitmapSize; /*!< Size of bitmap in bits. */
} ZoneMetadata;

const extern ZoneMetadata g_smallAllocationZoneMetadata; /*!< -. */
const extern ZoneMetadata g_mediumAllocationZoneMetadata; /*!< -. */

/*!
* @brief Contains all metadata needed for a zone.
* 		 Zones are stored as linked list [zone1] -> [zone2] -> [zone3].
*/
typedef struct ZoneHeader_s
{
	HeaderBoundaries		start; /*!< Start of header. */
	const ZoneMetadata*		metadata; /*!< -. */
	int						blockBitmap[BLOCK_BITMAP_SIZE_INTEGERS]; /*!< @brief bitmap to store which blocks are occupied */
	struct ZoneHeader_s*	nextZone; /*!< -. */
	HeaderBoundaries		end; /*!< @brief End of header. */
} ZoneHeader;

/*!
 * @brief Allocates memory from kernel and initialises the zoneheader.
 * @param zoneMetadata
 * @return
*/
ZoneHeader* create_zone(const ZoneMetadata* zoneMetadata);

/*!
 * @brief Deallocates and clears a zone.
 * @param zone
*/
void destroy_zone(ZoneHeader* zone);

/*!
 * @brief creates an allocation in zone.
 * @param zone
 * @param allocationSizeInBytes
 * @return Address of allocation, NULL if failed.
*/
void* allocate_in_zone(ZoneHeader* zone, size_t allocationSizeInBytes);

#endif
