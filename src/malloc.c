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

#include <unistd.h>
#include <assert.h>

void* km_malloc(size_t size)
{
	// No need to initialize since statics will be zero initialised by default.
	static Arena arenas[KMALLOC_NUMBER_OF_CORES];

	const size_t arenaIndex = get_thread_arena_index(KMALLOC_NUMBER_OF_CORES);
	assert(arenaIndex < KMALLOC_NUMBER_OF_CORES);

	return (allocate_in_arena(&arenas[arenaIndex], size));
}
