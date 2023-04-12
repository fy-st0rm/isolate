#include "iso_memory.h"

/*
 * @brief Memory structure for the custom allocator
 * @mem ptr  = Pointer to the data
 * @mem size = Size of the pointer in bytes
 * @mem file = File name where the pointer was created
 * @mem line = Line number where the pointer was created
 */

typedef struct {
	void* ptr;
	size_t size;
	const char* file;
	i32 line;
} iso_memory_block;


/*
 * @brief Memory manager that holds the allocated memory and tracks them.
 * @mem memory      = Array of memory blocks
 * @mem memory_cap  = Total memory capacitance
 * @mem memory_cnt  = No of memory allocated
 * @mem memory_size = Amount of memory allocated in bytes
 */

#define ISO_MEMORY_CAP 1024
typedef struct {
	iso_memory_block* memory;
	i32 memory_cap;
	i32 memory_cnt;
	i32 memory_size;
} iso_memory_manager;

iso_memory_manager* manager;


void iso_memory_init() {
	manager = (iso_memory_manager*) malloc(sizeof(iso_memory_manager));

	manager->memory      = (iso_memory_block*) malloc(sizeof(iso_memory_block) * ISO_MEMORY_CAP);
	manager->memory_cap  = 0;
	manager->memory_cnt  = 0;
	manager->memory_size = 0;
}

void iso_print_mem(iso_memory_block* mem) {
	printf("%p at %s:%d of %zu bytes\n", mem->ptr, mem->file, mem->line, mem->size);
}

void iso_memory_alert() {
	if (!manager->memory_cnt) return;

	printf("\n---------Unfreed memories---------\n");
	i32 i = 0;
	for (i = 0; i < manager->memory_cnt; i++) {
		iso_print_mem(&manager->memory[i]);
	}
	printf("\nTotal unfreed memories = %d\n", i+1);
	printf("---------Unfreed memories---------\n\n");
}

void iso_print_mem_buffer() {
	if (!manager->memory_cnt) return;

	printf("\n---------Memory Buffer---------\n");
		for (i32 i = 0; i < manager->memory_cnt; i++) {
		iso_print_mem(&manager->memory[i]);
	}
	printf("---------Memory Buffer---------\n\n");
}


void __iso_check_memory_bounds() {
	if (manager->memory_size >= manager->memory_cap) {
		// Create temp memory and copy the data
		iso_memory_block* tmp_mem = (iso_memory_block*) malloc(sizeof(iso_memory_block) * (manager->memory_cap));
		memcpy(tmp_mem, manager->memory, sizeof(iso_memory_block) * (manager->memory_size));
		free(manager->memory);

		// Delete old memory and create a new extended one and copy the data again
		manager->memory_cap += ISO_MEMORY_CAP;
		manager->memory = (iso_memory_block*) malloc(sizeof(iso_memory_block) * (manager->memory_cap));
		memcpy(manager->memory, tmp_mem, sizeof(iso_memory_block) * (manager->memory_size));
		free(tmp_mem);
	}
}

void* __iso_alloc(size_t size, const char* file, i32 line) {
	iso_memory_block mem = {
		malloc(size),
		size,
		file,
		line 
	};
	memset(mem.ptr, 0, size);

	__iso_check_memory_bounds();

	manager->memory[(manager->memory_cnt)++] = mem;
	manager->memory_size += size;
	return mem.ptr;
}

void  __iso_free(void* ptr) {
	for (i32 i = 0; i < manager->memory_cnt; i++) {
		if (manager->memory[i].ptr == ptr) {
			manager->memory_size -= manager->memory[i].size;

			memmove(&manager->memory[i], &manager->memory[i+1], (manager->memory_cnt - i - 1) * sizeof(iso_memory_block));

			manager->memory_cnt--;

			free(ptr);
			break;
		}
	}
}
