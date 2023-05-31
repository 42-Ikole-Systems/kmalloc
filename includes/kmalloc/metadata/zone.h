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

/*!
 * @brief in bytes
*/
# define MiB KiB * KiB

/*!
 * @brief in bytes
*/
# define KMALLOC_ZONE_SIZE (2 * MiB)

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
typedef struct
{
	uint8_t __header_start : 8;
	uint16_t capacityInPages : 16;
    uint16_t freePages : 16; /*! @brief amount of pages not assigned to a slab */
    void* nextFreePage;
    slab_header* smallSlabs; /*! @brief header of first slab, NULL if none exist. Will contain pointer to next slab */
    slab_header* largeSlabs; /*! @brief header of first slab, NULL if none exist. Will contain pointer to next slab */
	uint8_t __header_end : 8;
} zone_header;

/*!
 * @brief -.
 * @param addr
 * @param capacityInPages
*/
void set_zone_header(void* restrict addr, uint16_t capacityInPages);

/*!
 * @brief -.
 * @param addr
 * @return
*/
zone_header* get_zone_header(void* restrict addr);

#endif
