#ifndef __ISO_GRAPHICS_H__
#define __ISO_GRAPHICS_H__

#include "iso_util/iso_includes.h"
#include "iso_util/iso_defines.h"
#include "iso_util/iso_memory.h"
#include "iso_util/iso_hash_map.h"
#include "iso_window/iso_window.h"
#include "iso_math/iso_math.h"
#include "iso_graphics_types.h"


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
	iso_hmap_def(char*, iso_graphics_vertex_buffer*,   ISO_GRAPHICS_MEM_SIZE) vertex_buffers;
	iso_hmap_def(char*, iso_graphics_index_buffer*,    ISO_GRAPHICS_MEM_SIZE) index_buffers;
	iso_hmap_def(char*, iso_graphics_shader*,          ISO_GRAPHICS_MEM_SIZE) shaders;
	iso_hmap_def(char*, iso_graphics_render_pipeline*, ISO_GRAPHICS_MEM_SIZE) render_pipelines;
	iso_hmap_def(char*, iso_graphics_texture*,         ISO_GRAPHICS_MEM_SIZE) textures;
	iso_hmap_def(char*, iso_graphics_frame_buffer*,    ISO_GRAPHICS_MEM_SIZE) frame_buffers;

	struct {
		void (*init)         (iso_window* window);                   // Initializes graphics api
		void (*update)       (iso_window* window);                   // Updates the window
		void (*clear_window) (iso_window* window, iso_vec4 color);  // Clears the window with specified color
		
		// Construction
		iso_graphics_vertex_buffer*   (*vertex_buffer_new)   (iso_graphics* graphics, iso_graphics_vertex_buffer_def def);    // Function to create vertex buffer
		iso_graphics_index_buffer*    (*index_buffer_new)    (iso_graphics* graphics, iso_graphics_index_buffer_def  def);    // Function to create index buffer
		iso_graphics_shader*          (*shader_new)          (iso_graphics* graphics, iso_graphics_shader_def def);           // Function to create shader
		iso_graphics_render_pipeline* (*render_pipeline_new) (iso_graphics* graphics, iso_graphics_render_pipeline_def def);  // Function to create render pipeline
		iso_graphics_texture*         (*texture_new)         (iso_graphics* graphics, iso_graphics_texture_def def);          // Function to create texture
		iso_graphics_frame_buffer*    (*frame_buffer_new)    (iso_graphics* graphics, iso_graphics_frame_buffer_def def);     // Function to create frame buffer

		// Updates
		void (*vertex_buffer_update)  (iso_graphics* graphics, char* name, iso_graphics_buffer_update_def def);    // Function to update the vertex buffer data
		void (*index_buffer_update)   (iso_graphics* graphics, char* name, iso_graphics_buffer_update_def def);    // Function to update the index  buffer data
		void (*uniform_set)           (iso_graphics* graphics, iso_graphics_uniform_def def);                      // Function to set the uniform
		void (*render_pipeline_begin) (iso_graphics* graphics, char* name);                                        // Function to bind all the buffers to prepare for rendering
		void (*render_pipeline_end)   (iso_graphics* graphics, char* name, i32 indices_cnt);                       // Function to do a draw call for render pipeline
		void (*texture_update)        (iso_graphics* graphics, char* name, iso_graphics_texture_update_def def);   // Function to update the texture data

		// Binds
		void (*vertex_buffer_bind) (iso_graphics* graphics, char* name);    // Function to bind vertex buffer
		void (*index_buffer_bind)  (iso_graphics* graphics, char* name);    // Function to bind index buffer
		void (*shader_bind)        (iso_graphics* graphics, char* name);    // Function to bind shader
		void (*texture_bind)       (iso_graphics* graphics, char* name);    // Function to bind texture
		void (*frame_buffer_bind)  (iso_graphics* graphics, char* name);    // Function to bind frame buffer
		
		// Unbinds
		void (*vertex_buffer_unbind) (iso_graphics* graphics);                // Function to unbind vertex buffer
		void (*index_buffer_unbind)  (iso_graphics* graphics);                // Function to unbind index buffer
		void (*shader_unbind)        (iso_graphics* graphics);                // Function to unbind shader
		void (*texture_unbind)       (iso_graphics* graphics, char* name);    // Function to unbind texture
		void (*frame_buffer_unbind)  (iso_graphics* graphics, char* name);    // Function to unbind frame buffer

		// Destruction
		void (*vertex_buffer_delete)   (iso_graphics* graphics, char* name);   // Function to delete vertex buffer
		void (*index_buffer_delete)    (iso_graphics* graphics, char* name);   // Function to delete index buffer
		void (*shader_delete)          (iso_graphics* graphics, char* name);   // Function to delete shader
		void (*render_pipeline_delete) (iso_graphics* graphics, char* name);   // Function to delete render pipeline
		void (*texture_delete)         (iso_graphics* graphics, char* name);   // Function to delete texture
		void (*frame_buffer_delete)    (iso_graphics* graphics, char* name);   // Function to delete frame buffer
	} api;

	struct {
		iso_graphics_vertex_buffer*   (*get_vertex_buffer)   (iso_graphics* graphics, char* name);
		iso_graphics_index_buffer*    (*get_index_buffer)    (iso_graphics* graphics, char* name);
		iso_graphics_shader*          (*get_shader)          (iso_graphics* graphics, char* name);
		iso_graphics_render_pipeline* (*get_render_pipeline) (iso_graphics* graphics, char* name);
		iso_graphics_texture*         (*get_texture)         (iso_graphics* graphics, char* name);
		iso_graphics_frame_buffer*    (*get_frame_buffer)    (iso_graphics* graphics, char* name);
	} memory;
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

#endif // __ISO_GRAPHICS_H__
