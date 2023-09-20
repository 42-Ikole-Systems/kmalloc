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

#ifndef KMALLOC_SIZE_INFO_H
# define KMALLOC_SIZE_INFO_H

#include "boundaries.h"

// Temproary define in future will be gotten from kernel.
#define PAGE_SIZE 1024

#define BITS_IN_BYTE 8
#define BITS_IN_INTEGER sizeof(int) * BITS_IN_BYTE

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


#define SMALL_BLOCK_BITMAP_SIZE_BITS ((SMALL_ALLOCATION_ZONE_SIZE_PAGES * PAGE_SIZE) / SMALL_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES)
#define MEDIUM_BLOCK_BITMAP_SIZE_BITS ((MEDIUM_ALLOCATION_ZONE_SIZE_PAGES * PAGE_SIZE) / MEDIUM_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES)

// calculate how big the bitmap needs to be (so zone can be abstract).
#if SMALL_BLOCK_BITMAP_SIZE_BITS < MEDIUM_BLOCK_BITMAP_SIZE_BITS
# define BLOCK_BITMAP_SIZE_INTEGERS SMALL_BLOCK_BITMAP_SIZE_BITS / BITS_IN_INTEGER + (SMALL_BLOCK_BITMAP_SIZE_BITS % BITS_IN_INTEGER != 0)
#else
# define BLOCK_BITMAP_SIZE_INTEGERS MEDIUM_BLOCK_BITMAP_SIZE_BITS / BITS_IN_INTEGER + (MEDIUM_BLOCK_BITMAP_SIZE_BITS % BITS_IN_INTEGER != 0)
#endif

#endif
