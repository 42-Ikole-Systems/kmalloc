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

# include <inttypes.h>

# define KiB 1024
# define MiB (KiB * KiB)

/*!
 * @brief small allocation minimum size in bytes
*/
# define SMALL_SLAB_ALLOCATION_MIN_SIZE 16

/*!
 * @brief small allocation maximum size in bytes
*/
# define SMALL_SLAB_ALLOCATION_MAX_SIZE (1 * KiB)

/*!
 * @brief size in bytes
*/
# define SMALL_SLAB_SIZE (16 * KiB)

/*!
 * @brief large allocation minimum size in bytes
*/
# define LARGE_SLAB_ALLOCATION_MIN_SIZE (1 * KiB)

/*!
 * @brief large allocation maximum size in bytes
*/
# define LARGE_SLAB_ALLOCATION_MAX_SIZE (64 * KiB)

/*!
 * @brief size in bytes
*/
# define LARGE_SLAB_SIZE (1 * MiB)

/*!
 * @brief -.
*/
# define SLAB_MAX_ALLOCATIONS 1024

/*!
 * @brief sizeof of bitfield in bytes
*/
# define __SLAB_MAX_ALLOCATIONS_BITFIELD_SIZE__ (SLAB_MAX_ALLOCATIONS / 8)

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
	uint8_t __header_start : 8;
	uint16_t sizeInPages : 16;
    uint8_t allocations[__SLAB_MAX_ALLOCATIONS_BITFIELD_SIZE__]; /*! @brief bitfield specifying what regions are allcoated */
    void* nextSlab;
	uint8_t __header_end : 8;
} slab_header;

# undef __SLAB_MAX_ALLOCATIONS_BITFIELD_SIZE__

#endif
