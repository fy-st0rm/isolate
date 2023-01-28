#ifndef __ISO_GRAPHICS_H__
#define __ISO_GRAPHICS_H__

#include "iso_util/iso_includes.h"
#include "iso_util/iso_defines.h"
#include "iso_util/iso_memory.h"
#include "iso_util/iso_hash_map.h"
#include "iso_window/iso_window.h"
#include "iso_math.h"

/*
 * @brief Enum of the buffer usage types.
 */

typedef enum {
	ISO_GRAPHICS_STREAM,
	ISO_GRAPHICS_STATIC,
	ISO_GRAPHICS_DYNAMIC
} iso_graphics_buffer_usage_type;

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
typedef iso_graphics_vertex_buffer_def iso_graphics_index_buffer_def;

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
 * @brief Graphics Buffer objects
 * @mem id   = Id of the buffer
 * @mem name = Name of the buffer
 */

typedef struct { u32 id; char* name; } iso_graphics_vertex_buffer;
typedef iso_graphics_vertex_buffer     iso_graphics_index_buffer;
typedef iso_graphics_vertex_buffer     iso_graphics_shader;

/*
 * @brief Enum that holds supported graphics api
 */

typedef enum {
	ISO_OPENGL_API = SDL_WINDOW_OPENGL,
} iso_graphics_api;

/*
 * @brief Graphics defination structure. Defines on how the graphics should be.
 * @mem api          = Graphics api to use.
 * @mem renderer_amt = Max number of renderer
 */

typedef struct {
	iso_graphics_api api;
} iso_graphics_def;

/*
 * @brief Graphics structure
 * @mem vertex_buffers = Hashmap to store vertex buffers id.
 * @mem index_buffers  = Hashmap to store index buffers id.
 * @mem api    = A structure that holds function pointers to the specified graphics api.
 * @mem memory = A structure that holds function pointers to the memory getters and setters.
 */

#define ISO_GRAPHICS_MEM_SIZE 100

typedef struct iso_graphics iso_graphics;
struct iso_graphics {

	// Internal memory
	iso_hmap_def(char*, iso_graphics_vertex_buffer*, ISO_GRAPHICS_MEM_SIZE) vertex_buffers;
	iso_hmap_def(char*, iso_graphics_index_buffer*,  ISO_GRAPHICS_MEM_SIZE) index_buffers;
	iso_hmap_def(char*, iso_graphics_shader*,        ISO_GRAPHICS_MEM_SIZE) shaders;

	struct {
		void (*init)         (iso_window* window);                   // Initializes graphics api
		void (*update)       (iso_window* window);                   // Updates the window
		void (*clear_window) (iso_window* window, iso_color color);  // Clears the window with specified color
		
		// Construction
		iso_graphics_vertex_buffer* (*vertex_buffer_new) (iso_graphics* graphics, iso_graphics_vertex_buffer_def def);    // Function to create vertex buffer
		iso_graphics_index_buffer*  (*index_buffer_new)  (iso_graphics* graphics, iso_graphics_index_buffer_def  def);    // Function to create index buffer
		iso_graphics_shader*        (*shader_new)        (iso_graphics* graphics, iso_graphics_shader_def def);           // Function to create shader

		// Destruction
		void (*vertex_buffer_delete) (iso_graphics* graphics, char* name);   // Function to delete vertex buffer
		void (*index_buffer_delete)  (iso_graphics* graphics, char* name);   // Function to delete index buffer
		void (*shader_delete)        (iso_graphics* graphics, char* name);   // Function to delete shader
	} api;

	struct {
		iso_graphics_vertex_buffer* (*get_vertex_buffer) (iso_graphics* graphics, char* name);
		iso_graphics_index_buffer*  (*get_index_buffer)  (iso_graphics* graphics, char* name);
		iso_graphics_shader*        (*get_shader)        (iso_graphics* graphics, char* name);
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
 * @brief Function to load the memory functions
 * @param graphics = Pointer to the iso_graphics
 */

static void __iso_load_memory_functions(iso_graphics* graphics) {
	graphics->memory.get_vertex_buffer = __iso_graphics_get_vertex_buffer;
	graphics->memory.get_index_buffer  = __iso_graphics_get_index_buffer;
	graphics->memory.get_shader        = __iso_graphics_get_shader;
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

	graphics->api.vertex_buffer_new = iso_gl_vertex_buffer_new;
	graphics->api.index_buffer_new  = iso_gl_index_buffer_new;
	graphics->api.shader_new        = iso_gl_shader_new;

	graphics->api.vertex_buffer_delete = iso_gl_vertex_buffer_delete;
	graphics->api.index_buffer_delete  = iso_gl_index_buffer_delete;
	graphics->api.shader_delete        = iso_gl_shader_delete;
}

/*
 * @brief  Function to construct iso_graphics
 * @param  graphics_def = Graphics defination structure
 * @return Returns pointer to the iso_graphics
 */

static iso_graphics* iso_graphics_new(iso_graphics_def graphics_def) {
	iso_graphics* graphics = iso_alloc(sizeof(iso_graphics));

	graphics->vertex_buffers = NULL;

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
	iso_free(graphics);
}

#endif // __ISO_GRAPHICS_H__
