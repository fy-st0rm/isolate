#ifndef __ISO_MEMORY_H__
#define __ISO_MEMORY_H__


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "iso_defines.h"
#include "iso_log.h"

/*
 * @brief Memory structure for the custom allocator
 * @member ptr  = Pointer to the data
 * @member size = Size of the pointer in bytes
 * @member file = File name where the pointer was created
 * @member line = Line number where the pointer was created
 */

typedef struct {
	void* ptr;
	size_t size;
	const char* file;
	i32 line;
} iso_memory;

/*
 * @brief Storage for memory.
 */

#define ISO_MEMORY_CAP 1024
static iso_memory *memory;
static i32        *memory_size;

/*
 * @Brief Initialize the memory stack for memories
 */

void iso_memory_init() {
	memory = malloc(sizeof(iso_memory) * ISO_MEMORY_CAP);
	memory_size = malloc(sizeof(i32));
	*memory_size = 0;
}

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
 * @brief Function to print a memory
 * @param mem = Iso Memory struct
 */

static void iso_print_mem(iso_memory* mem) {
	printf("%p at %s:%d of %zu bytes\n", mem->ptr, mem->file, mem->line, mem->size);
}

/*
 * @brief Function that alerts about the unfreed memory
 */

static void iso_memory_alert() {
	if (!*memory_size) return;

	printf("\n---------Unfreed memories---------\n");
	i32 i = 0;
	for (i = 0; i < *memory_size; i++) {
		iso_print_mem(&memory[i]);
	}
	printf("\nTotal unfreed memories = %d\n", i+1);
	printf("---------Unfreed memories---------\n\n");
}

/*
 * @brief Function to print all the memory buffer
 */

static void iso_print_mem_buffer() {
	if (!*memory_size) return;

	printf("\n---------Memory Buffer---------\n");
		for (i32 i = 0; i < *memory_size; i++) {
		iso_print_mem(&memory[i]);
	}
	printf("---------Memory Buffer---------\n\n");
}



/* 
 * Backend implementation
 */

static void* __iso_alloc(size_t size, const char* file, i32 line) {
	iso_memory mem = {
		malloc(size),
		size,
		file,
		line 
	};
	memset(mem.ptr, 0, size);

	iso_assert(*memory_size < ISO_MEMORY_CAP, "Alloc buffer ran out of memory at %s:%d\n", file, line);

	memory[(*memory_size)++] = mem;
	return mem.ptr;
}

static void  __iso_free(void* ptr) {
	for (i32 i = 0; i < *memory_size; i++) {
		if (memory[i].ptr == ptr) {
			memmove(&memory[i], &memory[i+1], (*memory_size - i - 1) * sizeof(iso_memory));
			free(ptr);
			--(*memory_size);
			break;
		}
	}
}

#endif // __ISO_MEMORY_H__
