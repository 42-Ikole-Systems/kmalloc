
#ifndef KMALLOC_H
# define KMALLOC_H

# include <stdbool.h>
# include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief allocates size bytes of memory and returns a pointer to the allocated memory.
 * @param size
 * @return
*/
void* km_malloc(size_t size);

/*!
 * @brief	contiguously allocates enough space for count objects that are size bytes of memory each and returns a pointer to the allocated memory.
 *			The allocated memory is filled with bytes of value zero.
 * @param count
 * @param size
 * @return
*/
void* km_calloc(size_t count, size_t size);

/*!
 * @brief	tries to change the size of the allocation pointed to by ptr to size, and returns ptr.
 *			If there is not enough room to enlarge the memory allocation pointed to by ptr, realloc() creates a new allocation,
 *			copies as much of the old data pointed to by ptr as will fit to the new allocation,
 *			frees the old allocation, and returns a pointer to the allocated memory.
 *			If ptr is NULL, realloc() is identical to a call to malloc() for size bytes.
 *			If size is zero and ptr is not NULL, a new, minimum sized object is allocated and the original object is freed.
 *			When extending a region allocated with calloc(3), realloc(3) does not guarantee that the additional memory is also zero-filled.
 * @param ptr
 * @param size
 * @return
*/
void* km_realloc(void* ptr, size_t size);

/*!
 * @brief deallocates the memory allocation pointed to by ptr. If ptr is a NULL pointer, no operation is performed.
 * @param
 * @return
*/
void km_free(void* ptr);

#ifdef __cplusplus
}
#endif

#endif
