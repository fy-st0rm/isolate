#ifndef __ISO_GRAPHICS_H__
#define __ISO_GRAPHICS_H__

#include "iso_util/iso_includes.h"
#include "iso_util/iso_defines.h"
#include "iso_util/iso_memory.h"
#include "iso_util/iso_hash_map.h"

#include "iso_window/iso_window.h"
#include "iso_math/iso_math.h"

#include "iso_graphics_core/iso_vertex_buffer.h"
#include "iso_graphics_core/iso_index_buffer.h"
#include "iso_graphics_core/iso_shader.h"
#include "iso_graphics_core/iso_texture.h"
#include "iso_graphics_core/iso_render_pipeline.h"


/*
 * @brief Enum that holds supported graphics api
 */

typedef enum {
	ISO_OPENGL_API = SDL_WINDOW_OPENGL,
} iso_graphics_api;


/*
 * @brief Function that converts api id to string
 * @param api = iso_graphics_api
 * @return Returns the api name in string
 */

static char* iso_graphics_api_to_str(iso_graphics_api api) {
	switch (api) {
		case ISO_OPENGL_API: return "ISO_OPENGL_API";
		default: iso_assert(false, "Unsupported iso_graphics_api: %d\n", api);
	}
}

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
 * @mem frame_buffers    = Hashmap to store frame buffer struct pointer.
 * @mem api              = A structure that holds function pointers to the specified graphics api.
 * @mem memory           = A structure that holds function pointers to the memory getters and setters.
 */

#define ISO_GRAPHICS_MEM_SIZE 100

typedef struct iso_graphics iso_graphics;
struct iso_graphics {

	// Internal memory
	iso_hmap_def(iso_str, void*, ISO_GRAPHICS_MEM_SIZE) memory;

	struct {
		void (*init)         (iso_window* window);                  // Initializes graphics api
		void (*update)       (iso_window* window);                  // Updates the window
		void (*clear_window) (iso_window* window, iso_vec4 color);  // Clears the window with specified color

		// Construction
		iso_vertex_buffer*   (*vertex_buffer_new)     (iso_graphics* graphics, iso_vertex_buffer_def def);       // Function to create vertex buffer
		iso_index_buffer*    (*index_buffer_new)      (iso_graphics* graphics, iso_index_buffer_def  def);       // Function to create index buffer
		iso_shader*          (*shader_new)            (iso_graphics* graphics, iso_shader_def def);              // Function to create shader
		iso_texture*         (*texture_new_from_file) (iso_graphics* graphics, iso_texture_from_file_def def);   // Function to create texture from file
		iso_texture*         (*texture_new_from_data) (iso_graphics* graphics, iso_texture_from_data_def def);   // Function to create texture from data
		iso_render_pipeline* (*render_pipeline_new)   (iso_graphics* graphics, iso_render_pipeline_def def);     // Function to create render pipeline

		// Updates
		void (*vertex_buffer_update)  (iso_graphics* graphics, iso_vertex_buffer* vbo, iso_buffer_update_def def); // Function to update the vertex buffer data
		void (*index_buffer_update)   (iso_graphics* graphics, iso_index_buffer* ibo, iso_buffer_update_def def);  // Function to update the index  buffer data
		void (*shader_uniform_set)    (iso_graphics* graphics, iso_shader* shader, iso_uniform_def def);           // Function to set the uniform
		void (*texture_update)        (iso_graphics* graphics, iso_texture* texture, iso_texture_update_def def);  // Function to update the texture data
		void (*render_pipeline_begin) (iso_graphics* graphics, iso_render_pipeline* pip);                          // Function to bind all the buffers to prepare for rendering
		void (*render_pipeline_end)   (iso_graphics* graphics, iso_render_pipeline* pip, i32 indices_cnt);         // Function to do a draw call for render pipeline

		// Binds
		void (*vertex_buffer_bind) (iso_graphics* graphics, iso_vertex_buffer* vbo);    // Function to bind vertex buffer
		void (*index_buffer_bind)  (iso_graphics* graphics, iso_index_buffer*  ibo);    // Function to bind index buffer
		void (*shader_bind)        (iso_graphics* graphics, iso_shader*     shader);    // Function to bind shader
		void (*texture_bind)       (iso_graphics* graphics, iso_texture*   texture);    // Function to bind texture
		
		// Unbinds
		void (*vertex_buffer_unbind) (iso_graphics* graphics);                          // Function to unbind vertex buffer
		void (*index_buffer_unbind)  (iso_graphics* graphics);                          // Function to unbind index buffer
		void (*shader_unbind)        (iso_graphics* graphics);                          // Function to unbind shader
		void (*texture_unbind)       (iso_graphics* graphics, iso_texture* texture);    // Function to unbind texture

		// Destruction
		void (*vertex_buffer_delete)   (iso_graphics* graphics, iso_vertex_buffer* vbo);   // Function to delete vertex buffer
		void (*index_buffer_delete)    (iso_graphics* graphics, iso_index_buffer* ibo);    // Function to delete index buffer
		void (*shader_delete)          (iso_graphics* graphics, iso_shader* shader);       // Function to delete shader
		void (*texture_delete)         (iso_graphics* graphics, iso_texture* texture);     // Function to delete texture
		void (*render_pipeline_delete) (iso_graphics* graphics, iso_render_pipeline* pip); // Function to delete render pipeline
	} api;
};


/*
 * @brief  Function to construct iso_graphics
 * @param  graphics_def = Graphics defination structure
 * @return Returns pointer to the iso_graphics
 */

ISO_API iso_graphics* iso_graphics_new(iso_graphics_def graphics_def);


/*
 * @brief Function to delete the iso_graphics instance
 * @param graphics = Pointer to the iso_graphics
 */

ISO_API void iso_graphics_delete(iso_graphics* graphics);

/*
 * @brief Function to get memory from the graphics
 * @param 
 */

ISO_API void* iso_graphics_get(iso_graphics* graphics, iso_str name);


#endif // __ISO_GRAPHICS_H__
