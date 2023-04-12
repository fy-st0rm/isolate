#ifndef __ISO_MEMORY_H__
#define __ISO_MEMORY_H__

#include "iso_includes.h"
#include "iso_defines.h"
#include "iso_log.h"

/*
 * @Brief Initialize the memory stack for memory allocations
 *        Must initialize before doing any memory allocations!!
 */

ISO_API void iso_memory_init();

/*
 * @brief Function to allocate `x` amount of bytes.
 * @param x = sizeof the bytes needed to be allocated
 */

#define iso_alloc(x) __iso_alloc(x, __FILE__, __LINE__)

/*
 * @brief Function to free the allocated pointer
 * @param ptr = pointer to be freed
 */

#define iso_free(ptr) __iso_free(ptr)

/*
 * @brief Function that alerts about the unfreed memory.
 *        Should be called at the end of program to notify all the
 *        unfreed memories and its location.
 */

ISO_API void iso_memory_alert();

/*
 * @brief Function to print all the memory buffer
 */

ISO_API void iso_print_mem_buffer();


/*
 * @brief Internal memory functions
 */

ISO_API void* __iso_alloc(size_t size, const char* file, i32 line);
ISO_API void  __iso_free(void* ptr);


#endif // __ISO_MEMORY_H__
