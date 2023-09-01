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

#ifndef KMALLOC_SLAB_H
# define KMALLOC_SLAB_H

# include "kmalloc/metadata/metadata.h"
# include "kmalloc/metadata/zone.h"

# include <inttypes.h>
# include <stddef.h>
# include <stdbool.h>

/*!
 * @brief in bytes
*/
# define KiB 1024

/*!
 * @brief small allocation minimum size in bytes
*/
# define KMALLOC_SMALL_SLAB_ALLOCATION_MIN_SIZE 16

/*!
 * @brief small allocation maximum size in bytes
*/
# define KMALLOC_SMALL_SLAB_ALLOCATION_MAX_SIZE (1 * KiB)

/*!
 * @brief size in bytes
*/
# define KMALLOC_SMALL_SLAB_SIZE (4 * KiB)

/*!
 * @brief large allocation minimum size in bytes
*/
# define KMALLOC_LARGE_SLAB_ALLOCATION_MIN_SIZE (1 * KiB)

/*!
 * @brief large allocation maximum size in bytes
*/
# define KMALLOC_LARGE_SLAB_ALLOCATION_MAX_SIZE (64 * KiB)

/*!
 * @brief size in bytes
*/
# define KMALLOC_LARGE_SLAB_SIZE (256 * KiB)

/*!
 * @brief -.
*/
# define KMALLOC_SLAB_MAX_ALLOCATIONS 256

/*!
 * @brief sizeof of bitfield in bytes
*/
# define __SLAB_MAX_ALLOCATIONS_BITFIELD_SIZE__ (KMALLOC_SLAB_MAX_ALLOCATIONS / 32)

/*!
 * @brief
*/
typedef struct
{
	uint64_t minAllocationSize;
	uint64_t maxAllocaitonSize;
	uint64_t sizeInBytes;
} slab_metadata;

/*!
 * @brief -.
 * @return
*/
slab_metadata get_small_slab_metadata();

/*!
 * @brief -.
 * @return
*/
slab_metadata get_large_slab_metadata();

/*!
 * @brief gets the address of the right slab_header that where an allocation of sizeWithHeader will be stored
 * @param zone
 * @param sizeWithHeader
 * @return
*/
slab_header** get_slab_head_from_zone(zone_header* zone, size_t sizeWithHeader);

/*!
 * @brief gets the size the allocation will take up in memory (with padding and header)
 * @param sizeWithHeader
 * @return
*/
size_t get_true_allocation_size(size_t sizeWithHeader, slab_metadata slabMetadata);

/*!
 * @brief -.
 */
typedef enum
{
	slab_header_start = 0x01,
	slab_header_end = 0x02
} slab_header_boundries;

/*!
 * @brief header containing metadata of a slab
*/
typedef struct
{
	uint8_t			__header_start : 8;
	uint16_t		sizeInPages : 16;
    uint32_t		allocations[__SLAB_MAX_ALLOCATIONS_BITFIELD_SIZE__]; /*! @brief bitfield specifying what regions are allcoated */
    slab_metadata	metadata;
	void*			nextSlab;
	uint8_t			__header_end : 8;
} slab_header;

/*!
 * @brief creates a new slab within zone
 * @param zone
 * @param metadata
 * @return
*/
slab_header* create_slab(zone_header* zone, slab_metadata metadata);

/*!
 * @brief creates an allocation in slab
 * @param slab
 * @param sizeWithHeader
 * @return
*/
void* slab_allocate(slab_header* slab, size_t sizeWithHeader);

/*!
 * @brief gets the region in the slab with space for sizeWithHeader
 * @param slab
 * @param sizeWithHeader
 * @return pointer to region where the allocation will fit, if not possible return NULL
*/
void* get_region_in_slab(const slab_header* slab, size_t sizeWithHeader, slab_metadata slabMetadata);

# undef __SLAB_MAX_ALLOCATIONS_BITFIELD_SIZE__

#endif
