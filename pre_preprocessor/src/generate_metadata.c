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

/*!
 * Running this program will generate `metadata.hpp` which contains macros for
 * kmalloc so that it wont have to waste resources getting values that will be the
 * same throughout the program but depend on the system it is compiled on.
*/

#ifndef KM_GENERATED_FILENAME
#pragma message "Using default generated filename."
# define KM_GENERATED_FILENAME "__generated_kmalloc_metadata__.h"
#endif

#include "libkm/io/printf.h"
#include "libkm/string.h"

#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>

static char* get_warning_message()
{
	return km_strdup("/*\n * THIS IS AN AUTOMATICALLY GENERATED FILE, PLEASE DO NOT CHANGE VALUES DIRECTLY.\n"
		" * IF YOU WISH TO USE DIFFERENT SETTINGS, PLEASE USE SETTINGS FILE INSTEAD.\n*/\n\n");
}

/*!
 * @brief Returns the macro string of cores currently available on the system (there can be more configured).
*/
static char* get_number_of_cores_macro()
{
	const long numberOfCoresAvailable = sysconf(_SC_NPROCESSORS_CONF);
	char* numberOfCoresDefineString = NULL;
	assert(km_sprintf(&numberOfCoresDefineString, "#define KMALLOC_NUMBER_OF_CORES %d\n", numberOfCoresAvailable) != -1);
	return numberOfCoresDefineString;
}

static char* get_page_size_macro()
{
	const long pageSizeInBytes = sysconf(_SC_PAGE_SIZE);
	char* pageSizeDefineString = NULL;
	assert(km_sprintf(&pageSizeDefineString, "#define PAGE_SIZE %d\n", pageSizeInBytes) != -1);
	return pageSizeDefineString;
}

/*!
 * @brief
 * @param fd
 * @param get_macro_string Function pointer to function that returns an allocated macro string.
*/
static void write_macro_to_file(int fd, char*(*get_macro_string)(void))
{
	char* macro = get_macro_string();
	assert(macro != NULL);
	assert(write(fd, macro, km_strlen(macro)) != -1);
	free(macro);
}

int main()
{
	const int fd = open(KM_GENERATED_FILENAME, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	assert(fd != -1);

	write_macro_to_file(fd, get_warning_message);
	write_macro_to_file(fd, get_number_of_cores_macro);
	write_macro_to_file(fd, get_page_size_macro);
	close(fd);
}
