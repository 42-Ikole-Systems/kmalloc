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

#include "kmalloc/kmalloc.h"
#include "structure/arena.h"
#include "debug_assert.h"

#include <unistd.h>

void* km_malloc(size_t size)
{
	Arena* arena = get_arena();
	return (allocate_in_arena(arena, size));
}



void* km_realloc(void* ptr, size_t size)
{
	(void)ptr;
	(void)size;
	return (NULL);
}

void* km_calloc(size_t count, size_t size)
{
	(void)count;
	(void)size;

	return (NULL);
}
