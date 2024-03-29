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
#include "allocation.h"
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

extern const ZoneMetadata g_smallAllocationZoneMetadata; /*!< -. */
extern const ZoneMetadata g_mediumAllocationZoneMetadata; /*!< -. */

/*!
* @brief Contains all metadata needed for a zone.
* 		 Zones are stored as linked list [zone1] -> [zone2] -> [zone3].
*/
typedef struct ZoneHeader_s
{
	HeaderBoundaries		start; /*!< Start of header. */
	const ZoneMetadata*		metadata; /*!< -. */
	uint16_t				freeBlocks; /*!< -. */
	uint16_t				arenaIndex; /*!< Index of the arena the zone is contained in. */
	int						blockBitmap[BLOCK_BITMAP_SIZE_INTEGERS]; /*!< @brief bitmap to store which blocks are occupied */
	struct ZoneHeader_s*	nextZone; /*!< -. */
	HeaderBoundaries		end; /*!< @brief End of header. */
} ZoneHeader;

/*!
 * @brief -.
*/
typedef struct AllocationData_s
{
	ZoneHeader* zone; /*!< -. */
	size_t      firstBlockOfAllocation; /*!< Index inside the zone of the block where the allocation will start. */
	size_t      allocationSizeInBlocks; /*!< -. */
} AllocationData;

/*!
 * @brief Allocates memory from kernel and initialises the zoneheader.
 * @param zoneMetadata
 * @param arenaIndex
 * @return
*/
ZoneHeader* create_zone(const ZoneMetadata* zoneMetadata, size_t arenaIndex);

/*!
 * @brief Gets the ZoneHeader of the zone the allocaiton is contained in.
 * @param allocation
 * @return If ZoneHeader is NULL the allocaiton is not contained inside a zone.
*/
ZoneHeader* get_zone_header(AllocationHeader* allocation);

/*!
 * @brief Deallocates and clears a zone.
 * @param zone
*/
void destroy_zone(ZoneHeader* zone);

/*!
 * @brief Returns the index of the first block of where the allocation can fit.
 * @param zone
 * @param allocationSizeInBlocks
 * @return 0 if allocation does not fit.
*/
size_t get_allocation_block_in_zone(const ZoneHeader* zone, size_t allocationSizeInBlocks);

/*!
 * @brief creates an allocation in zone.
 * @param allocation to be created.
 * @return Address of allocation, NULL if failed.
*/
void* allocate_in_zone(const AllocationData allocation);

/*!
 * @brief Calculates the amount of blocks allocationSize will take up in the zone.
 * @param zone
 * @param allocationSizeInBytes
 * @return
*/
uint16_t get_allocation_size_in_blocks(const ZoneMetadata* zoneMetadata, size_t allocationSizeInBytes);

/*!
 * @brief -.
 * @param header
 * @param allocation
*/
void free_from_zone(ZoneHeader* header, AllocationHeader* allocation);

/*!
 * @brief -.
 * @param zone
 * @return
*/
bool zone_is_empty(const ZoneHeader* zone);

#endif
