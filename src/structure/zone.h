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
typedef struct zone_metadata
{
	const uint32_t	minAllocationSizeInBytes; /*!< -. */
	const uint32_t	maxAllocationSizeInBytes; /*!< -. */
	const uint32_t	zoneSizeInPages; /*!< -. */
	const uint32_t	bitfieldSize; /*!< -. */
} zone_metadata;

const extern zone_metadata smallAllocationZoneMetadata; /*!< -. */
const extern zone_metadata mediumAllocationZoneMetadata; /*!< -. */

/*!
* @brief Contains all metadata needed for a zone.
*/
typedef struct zone_header_s
{
	header_boundaries		start; /*!< Start of header. */
	uint16_t				capacityInPages : 16; /*!< -. */
	uint16_t				sizeInPages : 16; /*!< -. */
	const zone_metadata*	metadata; /*!< @brief -. */
	struct zone_header_s*	nextZone; /*!< -. */
	header_boundaries		end; /*!< @brief End of header. */
} zone_header;

#endif
