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

#ifndef KMALLOC_ARENA_HEADER_H
# define KMALLOC_ARENA_HEADER_H

# include "kmalloc/metadata/metadata.h"
# include "kmalloc/metadata/slab.h"

# include <inttypes.h>

/*!
 * @brief -.
 */
typedef enum
{
	arena_header_start = 0x01,
	arena_header_end = 0x02
} arena_header_boundries;

/*!
 * @brief header containing metadata of an arena
*/
typedef struct
{
	uint8_t __header_start : 8;
	kmalloc_metadata* metadata;
	slab_header* smallSlabs; /*! @brief points to the first slab for small allocations, its header will contain a pointer to the next slab */
	slab_header* largeSlabs; /*! @brief piont to the first slab for large allocations, its header will contain a pointer to the next slab */
	void* hugeAllocations; /*! @brief (map?) of huge allocations (not contained in a slab, has its own zone) */
	uint8_t __header_end : 8;
} arena_header;

#endif
