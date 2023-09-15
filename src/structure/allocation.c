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

#include "allocation.h"

#include <libkm/memory.h>

#include <stdlib.h>

void* set_allocation_header(void* address, uint16_t sizeInBlocks)
{
	AllocationHeader* header = (AllocationHeader*)address;
	header->start = header_boundary_allocation_start;
	header->sizeInBlocks = sizeInBlocks;
	header->end = header_boundary_allocation_end;

	return (address + sizeof(AllocationHeader));
}

AllocationHeader* get_allocation_header(void* allocationAddress)
{
	AllocationHeader* header = (AllocationHeader*)(allocationAddress - sizeof(AllocationHeader));

	if (header->start != header_boundary_allocation_start || header->end != header_boundary_allocation_end) {
		return NULL;
	}
	return header;
}

void remove_allocation_header(AllocationHeader* header)
{
	km_bzero(header, sizeof(AllocationHeader));
}
