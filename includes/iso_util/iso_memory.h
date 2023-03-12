#ifndef __ISO_MEMORY_H__
#define __ISO_MEMORY_H__

#include "iso_includes.h"
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
extern iso_memory *memory;
extern i32        *memory_size;
extern i32        *memory_cap;


/*
 * Its a trick to make global variables in header only library
 * For this ISO_IMPLEMENTATION should be defined in a single translation unit
 * before including the file
 */

#ifdef ISO_IMPLEMENTATION
iso_memory *memory;
i32        *memory_size;
i32        *memory_cap;
#endif // ISO_IMPLEMENTATION

/*
 * @Brief Initialize the memory stack for memories
 */

static void iso_memory_init() {
	memory      = (iso_memory*) malloc(sizeof(iso_memory) * ISO_MEMORY_CAP);
	memory_size = (i32*) malloc(sizeof(i32));
	memory_cap  = (i32*) malloc(sizeof(i32));

	*memory_size = 0;
	*memory_cap  = ISO_MEMORY_CAP;
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

static void __iso_check_memory_bounds() {
	if (*memory_size >= *memory_cap) {
		// Create temp memory and copy the data
		iso_memory* tmp_mem = (iso_memory*) malloc(sizeof(iso_memory) * (*memory_cap));
		memcpy(tmp_mem, memory, sizeof(iso_memory) * (*memory_size));
		free(memory);

		// Delete old memory and create a new extended one and copy the data again
		*memory_cap += ISO_MEMORY_CAP;
		memory = (iso_memory*) malloc(sizeof(iso_memory) * (*memory_cap));
		memcpy(memory, tmp_mem, sizeof(iso_memory) * (*memory_size));
		free(tmp_mem);
	}
}

static void* __iso_alloc(size_t size, const char* file, i32 line) {
	iso_memory mem = {
		malloc(size),
		size,
		file,
		line 
	};
	memset(mem.ptr, 0, size);

	//iso_assert(*memory_size < ISO_MEMORY_CAP, "Alloc buffer ran out of memory at %s:%d\n", file, line);
	__iso_check_memory_bounds();

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
