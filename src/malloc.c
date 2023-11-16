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
#include "pre_preprocessor/generated_kmalloc_metadata.h"
#include "debug_assert.h"

#include <unistd.h>

Arena arenas[KMALLOC_NUMBER_OF_CORES];

void* km_malloc(size_t size)
{
	// No need to initialize since statics will be zero initialised by default.

	const size_t arenaIndex = get_thread_arena_index(KMALLOC_NUMBER_OF_CORES);
	D_ASSERT(arenaIndex < KMALLOC_NUMBER_OF_CORES);

	return (allocate_in_arena(&arenas[arenaIndex], size));
}

void km_free(void* ptr)
{
	(void)ptr;
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
