#ifndef __ISO_GRAPHICS_TYPES_H__
#define __ISO_GRAPHICS_TYPES_H__

#include "iso_util/iso_includes.h"
#include "iso_util/iso_defines.h"
#include "iso_util/iso_log.h"
#include "iso_util/iso_str.h"


/*
 * @brief Enum of the graphics data type
 */

typedef enum {
	ISO_GRAPHICS_FLOAT,
	ISO_GRAPHICS_INT,
	ISO_GRAPHICS_UNSIGNED_INT,
	ISO_GRAPHICS_UNSIGNED_BYTE
} iso_graphics_data_type;


/*
 * @brief Enum for the texture format
 */

typedef enum {
	ISO_RGB,
	ISO_RGBA
} iso_graphics_texture_fmt;


/*
 * @brief Function to get size of iso graphics data types
 * @param type = iso graphics data type
 * @return Returns the size of data type
 */

static size_t iso_graphics_get_type_size(iso_graphics_data_type type) {
	size_t sz = 0;
	switch (type) {
		case ISO_GRAPHICS_FLOAT        : sz = sizeof(f32); break;
		case ISO_GRAPHICS_INT          : sz = sizeof(i32); break;
		case ISO_GRAPHICS_UNSIGNED_INT : sz = sizeof(u32); break;
		case ISO_GRAPHICS_UNSIGNED_BYTE: sz = sizeof(u32); break;
		default: iso_assert(0, "Unknown ISO_GRAPHICS_DATA_TYPE: %d\n", type); break;
	}
	return sz;
}


/*
 * @brief Enum of the buffer usage types.
 */

typedef enum {
	ISO_GRAPHICS_STREAM,
	ISO_GRAPHICS_STATIC,
	ISO_GRAPHICS_DYNAMIC
} iso_graphics_buffer_usage_type;


/*====================
 * Vertex buffer
 *==================*/


/*
 * @brief Struct that holds the data to update the buffer
 * @mem start_sz = Starting size of the data
 * @mem end_sz   = Ending size of the data
 * @mem data     = Pointer to the data
 */

typedef struct {
	size_t start_sz, end_sz;
	void* data;
} iso_graphics_buffer_update_def;


/*
 * @brief Structure that holds definition for vertex buffer and index buffer
 * @mem data  = Pointer to the data
 * @mem size  = Size of the data
 * @mem usage = Buffer usage type
 */

typedef struct {
	iso_str name;
	void*  data;
	size_t size;
	iso_graphics_buffer_usage_type usage;
} iso_graphics_vertex_buffer_def;


/*
 * @brief Vertex Buffer object
 * @mem id   = Id of the buffer
 * @mem name = Name of the buffer
 */

typedef struct {
	iso_str name;
	u32 id;
} iso_graphics_vertex_buffer;


/*====================
 * Index buffer
 *==================*/

typedef iso_graphics_vertex_buffer_def iso_graphics_index_buffer_def;
typedef iso_graphics_vertex_buffer     iso_graphics_index_buffer;


/*====================
 * Shader
 *==================*/

typedef iso_graphics_vertex_buffer iso_graphics_shader;


/*
 * @brief Enum for the creation of shader.
 */

typedef enum {
	ISO_GRAPHICS_SHADER_FROM_STR,
	ISO_GRAPHICS_SHADER_FROM_FILE
} iso_shader_source_type;


/*
 * @brief Shader defination structure
 * @mem name        = Name of the shader
 * @mem v_src       = Vertex shader string / Vertex shader file location
 * @mem f_src       = Fragment shader string / Fragment shader file location
 * @mem source_type = Shader creation process type
 */

typedef struct {
	iso_str name;
	iso_str v_src;
	iso_str f_src;
	iso_shader_source_type source_type;
} iso_graphics_shader_def;


/*
 * @brief Enum for all the supported uniform type
 */

typedef enum {
	ISO_GRAPHICS_UNIFORM_INT,
	ISO_GRAPHICS_UNIFORM_FLOAT,
	ISO_GRAPHICS_UNIFORM_VEC2,
	ISO_GRAPHICS_UNIFORM_VEC3,
	ISO_GRAPHICS_UNIFORM_VEC4,
	ISO_GRAPHICS_UNIFORM_MAT4,
	ISO_GRAPHICS_UNIFORM_SAMPLER2D
} iso_graphics_uniform_type;


/*
 * @brief Struct that defines the shader sampler.
 * @mem count    = Total number of samplers
 * @mem samplers = Array of samplers
 */

typedef struct {
	i32 count;
	i32* samplers;
} iso_graphics_sampler_def;


/*
 * @brief Struct to define uniform
 * @mem name   = Name of the uniform in shader
 * @mem shader = Pointer to the iso_graphics_shader
 * @mem data   = Pointer to the uniform data
 * @mem type   = Type of uniform
 */

typedef struct {
	iso_str name;
	iso_graphics_shader* shader;
	void* data;
	iso_graphics_uniform_type type;
} iso_graphics_uniform_def;


/*====================
 * Texture
 *==================*/

/*
 * @brief Enum for supported texture filters.
 */

typedef enum {
	ISO_GRAPHICS_FILTER_LINEAR,
	ISO_GRAPHICS_FILTER_NEAREST
} iso_graphics_filter;

/*
 * @brief Enum to define multiple texture types
 */

typedef enum {
	ISO_GRAPHICS_TEXTURE_FROM_FILE,
	ISO_GRAPHICS_TEXTURE_FROM_DATA
} iso_graphics_texture_type;

/*
 * @brief Structure that defines parameter to load texture from file
 * @mem file_path = Path to the image
 */

typedef struct {
	iso_str file_path;
} iso_graphics_texture_from_file_param;

/*
 * @brief Structure that defines parameter to load texture from raw data
 * @mem pixels = Pixel buffer
 * @mem format = Pixel format
 * @mem width  = Width of the texture
 * @mem height = Height of the texture
 */

typedef struct {
	void* pixels;
	iso_graphics_texture_fmt format;
	u32 width, height;
} iso_graphics_texture_from_data_param;

/*
 * @brief Structure that holds the definition of update texture
 * @mem pixels = Pointer to the pixel buffers
 * @mem lod    = Level of details
 * @mem x_offset = X offset in buffer
 * @mem y_offset = Y offset in buffer
 * @mem width    = Width of the image
 * @mem height   = Height of the image
 * @mem format   = Pixel format
 * @mem type     = iso_graphics_data_type
 */

typedef struct {
	void* pixels;
	u32   lod;
	u32   x_offset, y_offset;
	u32   width, height;
	iso_graphics_texture_fmt format;
	iso_graphics_data_type   type;
} iso_graphics_texture_update_def;

/*
 * @brief Struct to define texture
 * @mem name      = Name of the texture
 * @mem type      = Type of mode to create a texture from (file/data)
 * @mem param     = Param required to create texture. Varies according to the type.
 * @mem filter    = Struct to hold min and mag filters
 */

typedef struct {
	iso_str name;
	iso_graphics_texture_type type;

	union {
		iso_graphics_texture_from_file_param file_param;
		iso_graphics_texture_from_data_param data_param;
	} param;

	struct {
		iso_graphics_filter min, mag;
	} filter;
} iso_graphics_texture_def;

/*
 * @brief Struct to hold texture
 * @mem id     = Id of the texture in gpu
 * @mem width  = Width of the texture
 * @mem height = Height of the texture
 * @mem name   = Name of the texture
 */

typedef struct {
	u32 id;
	u32 width, height;
	iso_str name;
} iso_graphics_texture;


/*====================
 * Render pipeline
 *==================*/

/*
 * @brief Defination to define the vertex layout in GPU
 * @mem amt  = Amount of data for a single type
 * @mem type = Data type
 */

typedef struct {
	i32 amt;
	iso_graphics_data_type type;
} iso_graphics_vertex_layout_def;

/*
 * @brief Render Pipeline defination
 * @mem name   = Name of the pipeline
 * @mem vbo    = Pointer to vertex buffer
 * @mem ibo    = Pointer to index buffer
 * @mem shader = Pointer to shader
 * @mem layout = Array of the vertex layouts
 * @mem amt    = Amount of layout
 */

typedef struct {
	iso_str name;

	struct {
		iso_graphics_vertex_buffer* vbo;
		iso_graphics_index_buffer* ibo;
		iso_graphics_shader* shader;
	} buffers;

	iso_graphics_vertex_layout_def* layout;
	i32 amt;
} iso_graphics_render_pipeline_def;

/*
 * @brief Render Pipeline Struct
 * @mem id     = Id of the buffer
 * @mem name   = Name of the pipeline
 * @mem vbo    = Pointer to vertex buffer
 * @mem ibo    = Pointer to index buffer
 * @mem shader = Pointer to shader
 */

typedef struct {
	u32 id;
	iso_str name;

	struct {
		iso_graphics_vertex_buffer* vbo;
		iso_graphics_index_buffer* ibo;
		iso_graphics_shader* shader;
	} buffers;
} iso_graphics_render_pipeline;


/*====================
 * Frame Buffers
 *==================*/

/*
 * @brief Frame buffer definiton struct
 * @mem name   = Name of the frame buffer
 * @mem width  = Width of the frame buffer
 * @mem height = Height of the frame buffer
 * @mem format = frame buffer texture format
 */

typedef struct {
	iso_str name;
	u32 width, height;
	iso_graphics_texture_fmt format;
} iso_graphics_frame_buffer_def;

/*
 * @brief Frame buffer struct
 * @mem name           = Name of the frame buffer
 * @mem id             = Frame buffer id
 * @mem texture_id     = Frame buffer texture id
 * @mem width, height  = Width and height of the frame buffer
 */

typedef struct {
	iso_str name;
	u32 id;
	u32 texture_id;
	u32 width, height;
} iso_graphics_frame_buffer;

#endif // __ISO_GRAPHICS_TYPES_H__
