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

#include "zone.h"

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

const ZoneMetadata smallAllocationZoneMetadata = {
    .minAllocationSizeInBytes	= SMALL_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES,
    .maxAllocationSizeInBytes	= SMALL_ALLOCATION_MAX_ALLOCATION_SIZE_BYTES,
    .zoneSizeInPages			= SMALL_ALLOCATION_ZONE_SIZE_PAGES,
    .bitfieldSize				= (SMALL_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES / BITS_IN_INTEGER)
};

#ifndef MEDIUM_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES
# define MEDIUM_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES 256
#endif

#ifndef MEDIUM_ALLOCATION_MAX_ALLOCATION_SIZE_BYTES
# define MEDIUM_ALLOCATION_MAX_ALLOCATION_SIZE_BYTES 2048
#endif

#ifndef MEDIUM_ALLOCATION_ZONE_SIZE_PAGES
# define MEDIUM_ALLOCATION_ZONE_SIZE_PAGES 8 /* 8192 bytes */
#endif

const ZoneMetadata mediumAllocationZoneMetadata = {
	.minAllocationSizeInBytes	= MEDIUM_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES,
    .maxAllocationSizeInBytes	= MEDIUM_ALLOCATION_MAX_ALLOCATION_SIZE_BYTES,
    .zoneSizeInPages			= MEDIUM_ALLOCATION_ZONE_SIZE_PAGES,
    .bitfieldSize				= (MEDIUM_ALLOCATION_MIN_ALLOCATION_SIZE_BYTES / BITS_IN_INTEGER)
};
