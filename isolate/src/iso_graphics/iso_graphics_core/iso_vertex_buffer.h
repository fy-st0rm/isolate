#ifndef __ISO_VERTEX_BUFFER_H__
#define __ISO_VERTEX_BUFFER_H__

#include "iso_graphics_util.h"

/*
 * @brief Structure that holds definition for vertex buffer
 * @mem name  = Name of the vertex buffer
 * @mem data  = Pointer to the data
 * @mem size  = Size of the data
 * @mem usage = Buffer usage type
 */

typedef struct {
	iso_str name;
	void*  data;
	size_t size;
	iso_buffer_usage_type usage;
} iso_vertex_buffer_def;


/*
 * @brief Vertex Buffer object
 * @mem id   = Id of the buffer
 * @mem name = Name of the buffer
 * @mem name = Name of the buffer
 */

typedef struct {
	u32 id;
	iso_str name;
} iso_vertex_buffer;


/*
 * @brief Function to create vertex buffer
 * @param def = iso_vertex_buffer_def struct
 * @return Returns pointer to iso_vertex_buffer
 */

ISO_API iso_vertex_buffer* iso_vertex_buffer_new(iso_vertex_buffer_def def);

/*
 * @brief Function to delete vertex buffer
 * @param vbo = Pointer to iso_vertex_buffer struct
 */

ISO_API void iso_vertex_buffer_delete(iso_vertex_buffer* vbo);

/*
 * @brief Function to update vertex buffer
 * @param vbo = Pointer to iso_vertex_buffer struct
 * @param def = iso_buffer_update_def struct
 */

ISO_API void iso_vertex_buffer_update(iso_vertex_buffer* vbo, iso_buffer_update_def def);

/*
 * @brief Function to bind vertex buffer
 * @param vbo = Pointer to iso_vertex_buffer
 */

ISO_API void iso_vertex_buffer_bind(iso_vertex_buffer* vbo);

/*
 * @brief Function to unbind vertex buffer
 */

ISO_API void iso_vertex_buffer_unbind();

#endif // __ISO_VERTEX_BUFFER_H__
