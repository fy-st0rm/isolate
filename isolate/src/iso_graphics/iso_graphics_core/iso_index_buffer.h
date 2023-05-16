#ifndef __ISO_index_BUFFER_H__
#define __ISO_index_BUFFER_H__

#include "iso_graphics_util.h"

/*
 * @brief Structure that holds definition for index buffer
 * @mem name  = Name of the index buffer
 * @mem data  = Pointer to the data
 * @mem size  = Size of the data
 * @mem usage = Buffer usage type
 */

typedef struct {
	iso_str name;
	void*  data;
	size_t size;
	iso_buffer_usage_type usage;
} iso_index_buffer_def;


/*
 * @brief index Buffer object
 * @mem id   = Id of the buffer
 * @mem name = Name of the buffer
 * @mem name = Name of the buffer
 */

typedef struct {
	u32 id;
	iso_str name;
} iso_index_buffer;


/*
 * @brief Function to create index buffer
 * @param def = iso_index_buffer_def structure
 * @return Returns pointer to iso_index_buffer
 */

ISO_API iso_index_buffer* iso_index_buffer_new(iso_index_buffer_def def);

/*
 * @brief Function to delete index buffer
 * @param ibo = Pointer to iso_index_buffer struct
 */

ISO_API void iso_index_buffer_delete(iso_index_buffer* ibo);

/*
 * @brief Function to update index buffer data
 * @param ibo = Pointer to iso_index_buffer
 * @param def = iso_buffer_update_def struct
 */

ISO_API void iso_index_buffer_update(iso_index_buffer* ibo, iso_buffer_update_def def);

/*
 * @brief Function to bind index buffer
 * @param ibo = Pointer to iso_index_buffer struct
 */

ISO_API void iso_index_buffer_bind(iso_index_buffer* ibo);

/*
 * @brief Function to unbind index buffer
 */

ISO_API void iso_index_buffer_unbind();

#endif // __ISO_index_BUFFER_H__
