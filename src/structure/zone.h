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

#ifndef KAMLLOC_ZONE_H
#define KMALLOC_ZONE_H

#include "boundaries.h"

#include <inttypes.h>

/*!
 * @brief Information about allocation sizes and other metadata needed for a zone.
*/
typedef struct ZoneMetadata_s
{
	const uint32_t	minAllocationSizeInBytes; /*!< -. */
	const uint32_t	maxAllocationSizeInBytes; /*!< -. */
	const uint32_t	zoneSizeInPages; /*!< -. */
	const uint32_t	bitfieldSize; /*!< -. */
} ZoneMetadata;

const extern ZoneMetadata smallAllocationZoneMetadata; /*!< -. */
const extern ZoneMetadata mediumAllocationZoneMetadata; /*!< -. */

/*!
* @brief Contains all metadata needed for a zone.
* 		 [zone1] -> [zone2] -> [zone3]
*/
typedef struct ZoneHeader_s
{
	HeaderBoundaries		start; /*!< Start of header. */
	uint16_t				capacityInPages : 16; /*!< -. */
	uint16_t				sizeInPages : 16; /*!< -. */
	const ZoneMetadata*		metadata; /*!< @brief -. */
	struct ZoneHeader_s*	nextZone; /*!< -. */
	HeaderBoundaries		end; /*!< @brief End of header. */
} ZoneHeader;

#endif
