#ifndef __ISO_GRAPHICS_H__
#define __ISO_GRAPHICS_H__

#include "iso_util/iso_includes.h"
#include "iso_util/iso_defines.h"
#include "iso_util/iso_memory.h"
#include "iso_util/iso_hash_map.h"
#include "iso_window/iso_window.h"
#include "iso_math.h"

/*
 * @brief Enum of the graphics data type
 */

typedef enum {
	ISO_GRAPHICS_FLOAT,
	ISO_GRAPHICS_INT,
	ISO_GRAPHICS_UNSIGNED_INT
} iso_graphics_data_type;

/*
 * @brief Function to get size of iso graphics data types
 * @param type = iso graphics data type
 * @return Returns the size of data type
 */

static size_t iso_graphics_get_type_size(iso_graphics_data_type type) {
	size_t sz = 0;
	switch (type) {
		case ISO_GRAPHICS_FLOAT       : sz = sizeof(f32); break;
		case ISO_GRAPHICS_INT         : sz = sizeof(i32); break;
		case ISO_GRAPHICS_UNSIGNED_INT: sz = sizeof(u32); break;
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
	char   name[256];
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
	u32 id; char* name;
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
	ISO_SHADER_FROM_STR,
	ISO_SHADER_FROM_FILE
} iso_shader_source_type;

/*
 * @brief Shader defination structure
 * @mem name        = Name of the shader
 * @mem v_src       = Vertex shader string / Vertex shader file location
 * @mem f_src       = Fragment shader string / Fragment shader file location
 * @mem source_type = Shader creation process type
 */

typedef struct {
	char name[256];
	char* v_src;
	char* f_src;
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
	char name[256];
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
	char* file_path;
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
	u32 format;
	u32 width, height;
} iso_graphics_texture_from_data_param;

/*
 * @brief Struct to define texture
 * @mem name      = Name of the texture
 * @mem type      = Type of mode to create a texture from (file/data)
 * @mem param     = Param required to create texture. Varies according to the type.
 * @mem filter    = Struct to hold min and mag filters
 */

typedef struct {
	char name[256];
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
	char* name;
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
	char name[256];

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
	char* name;

	struct {
		iso_graphics_vertex_buffer* vbo;
		iso_graphics_index_buffer* ibo;
		iso_graphics_shader* shader;
	} buffers;
} iso_graphics_render_pipeline;


/*====================
 * Graphics api
 *==================*/

/*
 * @brief Enum that holds supported graphics api
 */

typedef enum {
	ISO_OPENGL_API = SDL_WINDOW_OPENGL,
} iso_graphics_api;

/*
 * @brief Graphics defination structure. Defines on how the graphics should be.
 * @mem api          = Graphics api to use.
 */

typedef struct {
	iso_graphics_api api;
} iso_graphics_def;

/*
 * @brief Graphics structure
 * @mem vertex_buffers   = Hashmap to store vertex buffers struct pointer.
 * @mem index_buffers    = Hashmap to store index buffers struct pointer.
 * @mem shaders          = Hashmap to store shader struct pointer.
 * @mem render_pipelines = Hashmap to store render pipeline struct pointer.
 * @mem textures         = Hashmap to store texture struct pointer.
 * @mem api              = A structure that holds function pointers to the specified graphics api.
 * @mem memory           = A structure that holds function pointers to the memory getters and setters.
 */

#define ISO_GRAPHICS_MEM_SIZE 100

typedef struct iso_graphics iso_graphics;
struct iso_graphics {

	// Internal memory
	iso_hmap_def(char*, iso_graphics_vertex_buffer*,   ISO_GRAPHICS_MEM_SIZE) vertex_buffers;
	iso_hmap_def(char*, iso_graphics_index_buffer*,    ISO_GRAPHICS_MEM_SIZE) index_buffers;
	iso_hmap_def(char*, iso_graphics_shader*,          ISO_GRAPHICS_MEM_SIZE) shaders;
	iso_hmap_def(char*, iso_graphics_render_pipeline*, ISO_GRAPHICS_MEM_SIZE) render_pipelines;
	iso_hmap_def(char*, iso_graphics_texture*,         ISO_GRAPHICS_MEM_SIZE) textures;

	struct {
		void (*init)         (iso_window* window);                   // Initializes graphics api
		void (*update)       (iso_window* window);                   // Updates the window
		void (*clear_window) (iso_window* window, iso_color color);  // Clears the window with specified color
		
		// Construction
		iso_graphics_vertex_buffer*   (*vertex_buffer_new)   (iso_graphics* graphics, iso_graphics_vertex_buffer_def def);    // Function to create vertex buffer
		iso_graphics_index_buffer*    (*index_buffer_new)    (iso_graphics* graphics, iso_graphics_index_buffer_def  def);    // Function to create index buffer
		iso_graphics_shader*          (*shader_new)          (iso_graphics* graphics, iso_graphics_shader_def def);           // Function to create shader
		iso_graphics_render_pipeline* (*render_pipeline_new) (iso_graphics* graphics, iso_graphics_render_pipeline_def def);  // Function to create render pipeline
		iso_graphics_texture*         (*texture_new)         (iso_graphics* graphics, iso_graphics_texture_def def);          // Function to create texture

		// Updates
		void (*vertex_buffer_update)  (iso_graphics* graphics, char* name, iso_graphics_buffer_update_def def);    // Function to update the vertex buffer data
		void (*uniform_set)           (iso_graphics* graphics, iso_graphics_uniform_def def);                      // Function to set the uniform
		void (*render_pipeline_begin) (iso_graphics* graphics, char* name);                                        // Function to bind all the buffers to prepare for rendering
		void (*render_pipeline_end)   (iso_graphics* graphics, char* name, i32 indices_cnt);                       // Function to do a draw call for render pipeline

		// Binds
		void (*vertex_buffer_bind) (iso_graphics* graphics, char* name);    // Function to bind vertex buffer
		void (*index_buffer_bind)  (iso_graphics* graphics, char* name);    // Function to bind index buffer
		void (*shader_bind)        (iso_graphics* graphics, char* name);    // Function to bind shader
		void (*texture_bind)       (iso_graphics* graphics, char* name);    // Function to bind texture

		// Destruction
		void (*vertex_buffer_delete)   (iso_graphics* graphics, char* name);   // Function to delete vertex buffer
		void (*index_buffer_delete)    (iso_graphics* graphics, char* name);   // Function to delete index buffer
		void (*shader_delete)          (iso_graphics* graphics, char* name);   // Function to delete shader
		void (*render_pipeline_delete) (iso_graphics* graphics, char* name);   // Function to delete render pipeline
		void (*texture_delete)         (iso_graphics* graphics, char* name);   // Function to delete texture
	} api;

	struct {
		iso_graphics_vertex_buffer*   (*get_vertex_buffer)   (iso_graphics* graphics, char* name);
		iso_graphics_index_buffer*    (*get_index_buffer)    (iso_graphics* graphics, char* name);
		iso_graphics_shader*          (*get_shader)          (iso_graphics* graphics, char* name);
		iso_graphics_render_pipeline* (*get_render_pipeline) (iso_graphics* graphics, char* name);
		iso_graphics_texture*         (*get_texture)         (iso_graphics* graphics, char* name);
	} memory;
};

/*
 * @brief Function to get saved vertex buffer
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the vertex buffer
 * @return Returns the vertex buffer pointer
 */

static iso_graphics_vertex_buffer* __iso_graphics_get_vertex_buffer(iso_graphics* graphics, char* name) {
	iso_graphics_vertex_buffer* vbo;
	iso_hmap_get(graphics->vertex_buffers, name, vbo);
	return vbo;
}

/*
 * @brief Function to get saved index buffer
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the index buffer
 * @return Returns the index buffer pointer
 */

static iso_graphics_index_buffer* __iso_graphics_get_index_buffer(iso_graphics* graphics, char* name) {
	iso_graphics_index_buffer* ibo;
	iso_hmap_get(graphics->index_buffers, name, ibo);
	return ibo;
}

/*
 * @brief Function to get saved shader
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the shader
 * @return Returns the shader pointer
 */

static iso_graphics_shader* __iso_graphics_get_shader(iso_graphics* graphics, char* name) {
	iso_graphics_shader* shader;
	iso_hmap_get(graphics->shaders, name, shader);
	return shader;
}

/*
 * @brief Function to get saved render_pipeline
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the render_pipeline
 * @return Returns the render_pipeline pointer
 */

static iso_graphics_render_pipeline* __iso_graphics_get_render_pipeline(iso_graphics* graphics, char* name) {
	iso_graphics_render_pipeline* render_pipeline;
	iso_hmap_get(graphics->render_pipelines, name, render_pipeline);
	return render_pipeline;
}

/*
 * @brief Function to get saved texture
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the texture
 * @return Returns the texture pointer
 */

static iso_graphics_texture* __iso_graphics_get_texture(iso_graphics* graphics, char* name) {
	iso_graphics_texture* texture;
	iso_hmap_get(graphics->textures, name, texture);
	return texture;
}

/*
 * @brief Function to load the memory functions
 * @param graphics = Pointer to the iso_graphics
 */

static void __iso_load_memory_functions(iso_graphics* graphics) {
	graphics->memory.get_vertex_buffer   = __iso_graphics_get_vertex_buffer;
	graphics->memory.get_index_buffer    = __iso_graphics_get_index_buffer;
	graphics->memory.get_shader          = __iso_graphics_get_shader;
	graphics->memory.get_render_pipeline = __iso_graphics_get_render_pipeline;
	graphics->memory.get_texture         = __iso_graphics_get_texture;
}

/*
 * Including the backend implimentation
 */

#include "iso_opengl_backend/iso_opengl_backend.h"

/*
 * @brief Function that loads opengl backend functions into graphics api
 * @param graphics = Pointer to the iso_graphics
 */

static void __iso_load_opengl_functions(iso_graphics* graphics) {
	graphics->api.init         = iso_gl_init;
	graphics->api.update       = iso_gl_update;
	graphics->api.clear_window = iso_gl_clear_window;

	// Construction
	graphics->api.vertex_buffer_new   = iso_gl_vertex_buffer_new;
	graphics->api.index_buffer_new    = iso_gl_index_buffer_new;
	graphics->api.shader_new          = iso_gl_shader_new;
	graphics->api.render_pipeline_new = iso_gl_render_pipeline_new;
	graphics->api.texture_new         = iso_gl_texture_new;

	// Updates
	graphics->api.vertex_buffer_update  = iso_gl_vertex_buffer_update;
	graphics->api.uniform_set           = iso_gl_uniform_set;
	graphics->api.render_pipeline_begin = iso_gl_render_pipeline_begin;
	graphics->api.render_pipeline_end   = iso_gl_render_pipeline_end;

	// Binds
	graphics->api.vertex_buffer_bind = iso_gl_vertex_buffer_bind;
	graphics->api.index_buffer_bind  = iso_gl_index_buffer_bind;
	graphics->api.shader_bind        = iso_gl_shader_bind;
	graphics->api.texture_bind       = iso_gl_texture_bind;

	// Destructions
	graphics->api.vertex_buffer_delete   = iso_gl_vertex_buffer_delete;
	graphics->api.index_buffer_delete    = iso_gl_index_buffer_delete;
	graphics->api.shader_delete          = iso_gl_shader_delete;
	graphics->api.render_pipeline_delete = iso_gl_render_pipeline_delete;
	graphics->api.texture_delete         = iso_gl_texture_delete;
}

/*
 * @brief  Function to construct iso_graphics
 * @param  graphics_def = Graphics defination structure
 * @return Returns pointer to the iso_graphics
 */

static iso_graphics* iso_graphics_new(iso_graphics_def graphics_def) {
	iso_graphics* graphics = iso_alloc(sizeof(iso_graphics));

	__iso_load_memory_functions(graphics);

	switch (graphics_def.api) {
		case ISO_OPENGL_API:
			__iso_load_opengl_functions(graphics);
			break;
		default:
			iso_assert(0, "Unknown graphics api = %d\n", graphics_def.api);
			break;
	}

	return graphics;
}

/*
 * @brief Function to delete the iso_graphics instance
 * @param graphics = Pointer to the iso_graphics
 */

static void iso_graphics_delete(iso_graphics* graphics) {
	iso_hmap_delete(graphics->vertex_buffers);
	iso_hmap_delete(graphics->index_buffers);
	iso_hmap_delete(graphics->shaders);
	iso_hmap_delete(graphics->render_pipelines);
	iso_hmap_delete(graphics->textures);
	iso_free(graphics);
}

#endif // __ISO_GRAPHICS_H__
