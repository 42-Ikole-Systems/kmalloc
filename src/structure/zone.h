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

#include <libkm/math.h>

#include <inttypes.h>
#include <stdbool.h>

// Temproary define in future will be gotten from kernel.
#define PAGE_SIZE 1024

#define BITS_IN_BYTE 8
#define BITS_IN_INTEGER sizeof(int) / BITS_IN_BYTE

#ifndef SMALL_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES
# define SMALL_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES 16
#endif

#ifndef SMALL_ALLOCATION_MAX_ALLOCATION_SIZE_BYTES
# define SMALL_ALLOCATION_MAX_ALLOCATION_SIZE_BYTES 256
#endif

#ifndef SMALL_ALLOCATION_ZONE_SIZE_PAGES
# define SMALL_ALLOCATION_ZONE_SIZE_PAGES 4 /* 4096 bytes */
#endif

#ifndef MEDIUM_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES
# define MEDIUM_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES 256
#endif

#ifndef MEDIUM_ALLOCATION_MAX_ALLOCATION_SIZE_BYTES
# define MEDIUM_ALLOCATION_MAX_ALLOCATION_SIZE_BYTES 2048
#endif

#ifndef MEDIUM_ALLOCATION_ZONE_SIZE_PAGES
# define MEDIUM_ALLOCATION_ZONE_SIZE_PAGES 8 /* 8192 bytes */
#endif

// Size of block bitmap in integers.
#define __SMALL_BLOCK_BITMAP_SIZE_INT__ ((float)(SMALL_ALLOCATION_ZONE_SIZE_PAGES * PAGE_SIZE) / (float)SMALL_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES) / (float)BITS_IN_INTEGER)
#define __MEDIUM_BLOCK_BITMAP_SIZE_INT__ ((float)(MEDIUM_ALLOCATION_ZONE_SIZE_PAGES * PAGE_SIZE) / (float)MEDIUM_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES) / (float)BITS_IN_INTEGER)

#if __SMALL_BLOCK_BITMAP_SIZE_INT__ < __MEDIUM_BLOCK_BITMAP_SIZE_INT__
# define BLOCK_BITMAP_SIZE_INTEGERS (int)__SMALL_BLOCK_BITMAP_SIZE_INT__
// ceil at compiletime
# if BLOCK_BITMAP_SIZE_INTEGERS < __SMALL_BLOCK_BITMAP_SIZE_INT__
#   define BLOCK_BITMAP_SIZE_INTEGERS (int)__SMALL_BLOCK_BITMAP_SIZE_INT__ + 1
# endif
#else
# define BLOCK_BITMAP_SIZE_INTEGERS (int)__MEDIUM_BLOCK_BITMAP_SIZE_INT__
# if BLOCK_BITMAP_SIZE_INTEGERS < __MEDIUM_BLOCK_BITMAP_SIZE_INT__
#   define BLOCK_BITMAP_SIZE_INTEGERS (int)__MEDIUM_BLOCK_BITMAP_SIZE_INT__ + 1
# endif
#endif

/*!
 * @brief Information about allocation sizes and other metadata needed for a zone.
*/
typedef struct ZoneMetadata_s
{
	const uint32_t	minAllocationSizeInBytes; /*!< -. */
	const uint32_t	maxAllocationSizeInBytes; /*!< -. */
	const uint32_t	zoneSizeInPages; /*!< -. */
	const uint32_t	bitfieldSize; /*!< Size of bitfield in bits. */
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
	int						blockBitmap	[BLOCK_BITMAP_SIZE_INTEGERS]; /*!< @brief bitmap to store blocks are occupied */
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

#endif
