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

#ifndef KMALLOC_ALLOCATION_H
# define KMALLOC_ALLOCATION_H

# include <inttypes.h>

/*!
 * @brief -.
 */
typedef enum
{
	allocation_header_start = 0x01,
	allocation_header_end = 0x02
} allocation_header_boundries;

/*!
 * @brief header containing metadata of an allocation
*/
typedef struct
{
	uint8_t __header_start : 8;
	uint16_t sizeInBytes : 16;
	uint8_t __header_end : 8;
} allocation_header;

/*!
 * @brief -.
 * @param addr
 * @param size
*/
void set_allocation_header(void* restrict addr, uint16_t size);

/*!
 * @brief -.
 * @param addr
 * @return
*/
allocation_header get_allocation_header(void* restrict addr);

#endif
