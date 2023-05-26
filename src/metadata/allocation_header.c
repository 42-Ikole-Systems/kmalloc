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

#include "kmalloc/metadata/allocation_headers.h"

#include <libkm/memory.h>

#include <assert.h>

void set_allocation_header(void* restrict addr, uint16_t size)
{
	allocation_header header = {
		.__header_start = allocation_header_start,
		.size = size,
		.__header_end = allocation_header_end
	};
	km_memcpy(addr, &header, sizeof(header));
}

allocation_header get_allocation_header(void* restrict addr)
{
	allocation_header header = *(allocation_header*)addr;

	assert(header.__header_start == allocation_header_start);
	assert(header.__header_end == allocation_header_end);
	assert(header.size > 0);

	return header;
}
