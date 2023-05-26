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

#ifndef KMALLOC_METADATA_H
# define KMALLOC_METADATA_H

#include <inttypes.h>

/*!
 * @brief malloc metadata
*/
typedef struct
{
    uint16_t pageSizeInBytes : 16;
    uint16_t chunkSizeInPages : 16;
    uint16_t smallAllocationSizeLimit : 16; /*! @brief allocation size in bytes */
    uint16_t largeAllocaitonSizeLimit : 16; /*! @brief allocaiton size in bytes */
    uint64_t totalAllocations;
    uint64_t totalBytesAllocated;
} kmalloc_metadata;


#endif
