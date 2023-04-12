#ifndef __ISO_FILE_H__
#define __ISO_FILE_H__

#include "iso_defines.h"
#include "iso_includes.h"
#include "iso_memory.h"

/*
 * @brief Struct that holds the data of the file.
 * @mem data = Content of the file
 * @mem size = Size of the file
 */

typedef struct {
	char*  data;
	size_t size;
} iso_file;

/*
 * @brief Function to calculate size of the file
 * @param file = FILE pointer
 * @return Returns the size of file
 */

static size_t __iso_file_get_size(FILE* file) {
	fseek(file, 0, SEEK_END);
	size_t size = ftell(file);
	fseek(file, 0, SEEK_SET);
	return size;
}

/*
 * @brief Function to read the file
 * @param path = Path of the file to read
 * @return Returns pointer to the iso_file
 */

static iso_file* iso_file_read(char* path) {
	iso_file* file = iso_alloc(sizeof(iso_file));

	FILE* f = fopen(path, "r");
	if (!f) {
		iso_assert(false, "Failed to open file: %s\n[Reason]: %s\n", path, strerror(errno));
	}

	file->size = __iso_file_get_size(f);
	file->data = iso_alloc(file->size);
	fread(file->data, file->size, 1, f);
	file->data[file->size-1] = '\0';

	fclose(f);
	return file;
}

/*
 * @brief Function to write content of iso_file into file
 * @param path = Path of the file
 * @param file = Pointer to the iso_file
 */

static void iso_file_write(char* path, iso_file* file) {
	FILE* f = fopen(path, "w");
	if (!f) {
		iso_assert(false, "Failed to open file: %s\n[Reason]: %s\n", path, strerror(errno));
	}

	fwrite(file->data, file->size, 1, f);
	fclose(f);
}

/*
 * @brief Function to close iso_file
 * @param file = Pointer to iso_file
 */

static void iso_file_close(iso_file* file) {
	iso_free(file->data);
	iso_free(file);
}

#endif // __ISO_FILE_H__
