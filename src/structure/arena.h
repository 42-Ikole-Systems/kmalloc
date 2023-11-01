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

#ifndef KMALLOC_ARENA_H
# define KMALLOC_ARENA_H

#include "zone.h"

#include <inttypes.h>

/*!
 * @brief -.
*/
typedef struct Arena_s
{
	ZoneHeader*		smallZones; /*!< @brief Points to the first zone, Null if no zones exist yet. */
	ZoneHeader*		mediumZones; /*!< @brief Points to the first zone, Null if no zones exist yet. */
	void*			huge_allocations; /*!< @brief (binary tree?) of huge allocations (not contained in zones). */
	uint64_t		capacity; /*!< @brief Capacity in bytes. */
	uint64_t		size; /*!< @brief Used size in bytes. */    
} Arena;

/*!
 * @brief Gets the index for the arena assigned to the thread.
 * @param amountOfArenas -.
 * @returns
*/
size_t get_thread_arena_index(const size_t amountOfArenas);

#endif
