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

# include <inttypes.h>

/*!
 * @brief malloc metadata
*/
typedef struct
{
    const uint16_t pageSizeInBytes : 16;
    const uint16_t chunkSizeInPages : 16;
    uint64_t totalAllocations : 64;
    uint64_t totalBytesAllocated : 64; /*! @brief amount of bytes allocted by process */
    uint64_t totalBytesCapacity : 64; /*! @brief amount of bytes given to process by the kernel (includes preallocated bytes and internal/external-fragmentation) */
} kmalloc_metadata;

#endif
