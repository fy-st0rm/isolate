#ifndef __ISO_OPENGL_BACKEND_H__
#define __ISO_OPENGL_BACKEND_H__

#include "iso_util/iso_file.h"
#include "iso_graphics/iso_graphics.h"

/*
 * @brief Function to initialize opengl backend
 * @param window = Pointer to the iso_window
 */

ISO_API_INTERNAL void iso_gl_init(iso_window* window);

/*
 * @brief Function to update the window for opengl
 * @param window = Pointer to the iso_window
 */

ISO_API_INTERNAL void iso_gl_update(iso_window* window);

/*
 * @brief Function to clear the window
 * @param window = Pointer to the iso_window
 * @param color  = Color of the background
 */

ISO_API_INTERNAL void iso_gl_clear_window(iso_window* window, iso_color color);



/*=========================
 * Opengl Vertex Buffer
 *=======================*/

/*
 * @brief Function to create opengl vertex buffer
 * @param graphics = Pointer to the iso_graphics
 * @param def      = Vertex buffer definition
 * @return Returns pointer to the vertex buffer
 */

ISO_API_INTERNAL iso_graphics_vertex_buffer* iso_gl_vertex_buffer_new(iso_graphics* graphics, iso_graphics_vertex_buffer_def def);

/*
 * @brief Function to update opengl vertex buffer data
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the vertex buffer
 * @param def      = iso_graphics_buffer_update_def struct
 */

ISO_API_INTERNAL void iso_gl_vertex_buffer_update(iso_graphics* graphics, iso_str name, iso_graphics_buffer_update_def def);

/*
 * @brief Function to bind vertex buffer
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the vertex buffer
 */

ISO_API_INTERNAL void iso_gl_vertex_buffer_bind(iso_graphics* graphics, iso_str name);

/*
 * @brief Function to unbind vertex buffer
 * @param graphics = Pointer to the iso_graphics
 */

ISO_API_INTERNAL void iso_gl_vertex_buffer_unbind(iso_graphics* graphics);

/*
 * @brief Function to delete opengl vertex buffer
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the vertex buffer
 */

ISO_API_INTERNAL void iso_gl_vertex_buffer_delete(iso_graphics* graphics, iso_str name);



/*=========================
 * Opengl Index Buffer
 *=======================*/

/*
 * @brief Function to create opengl index buffer
 * @param graphics = Pointer to the iso_graphics
 * @param def      = Vertex buffer definition
 * @return Returns pointer to the index buffer
 */

ISO_API_INTERNAL iso_graphics_index_buffer* iso_gl_index_buffer_new(iso_graphics* graphics, iso_graphics_index_buffer_def def);

/*
 * @brief Function to update opengl index buffer data
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the index buffer
 * @param def      = iso_graphics_buffer_update_def struct
 */

ISO_API_INTERNAL void iso_gl_index_buffer_update(iso_graphics* graphics, iso_str name, iso_graphics_buffer_update_def def);

/*
 * @brief Function to bind index buffer
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the index buffer
 */

ISO_API_INTERNAL void iso_gl_index_buffer_bind(iso_graphics* graphics, iso_str name);

/*
 * @brief Function to unbind index buffer
 * @param graphics = Pointer to the iso_graphics
 */

ISO_API_INTERNAL void iso_gl_index_buffer_unbind(iso_graphics* graphics);

/*
 * @brief Function to delete opengl index buffer
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the index buffer
 */

ISO_API_INTERNAL void iso_gl_index_buffer_delete(iso_graphics* graphics, iso_str name);



/*=========================
 * Opengl Frame Buffer
 *=======================*/

/*
 * @brief Function to create new opengl frame buffer
 * @param graphics = Pointer to the iso_graphics
 * @param def      = iso_graphics_frame_buffer_def struct
 * @return Returns the pointer to the iso_graphics_frame_buffer
 */

ISO_API_INTERNAL iso_graphics_frame_buffer* iso_gl_frame_buffer_new(iso_graphics* graphics, iso_graphics_frame_buffer_def def);

/*
 * @brief Function to bind frame buffer
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the frame buffer
 */

ISO_API_INTERNAL void iso_gl_frame_buffer_bind(iso_graphics* graphics, iso_str name);

/*
 * @brief Function to unbind frame buffer
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the frame buffer
 */

ISO_API_INTERNAL void iso_gl_frame_buffer_unbind(iso_graphics* graphics, iso_str name);

/*
 * @brief Function to delete opengl frame buffer
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the frame buffer
 */

ISO_API_INTERNAL void iso_gl_frame_buffer_delete(iso_graphics* graphics, iso_str name);



/*=========================
 * Opengl Shader
 *=======================*/

/*
 * @brief Function to create opengl shader according to the shader defination
 * @param graphics = Pointer to the iso_graphics
 * @param def      = Shader program defination
 * @return Returns pointer to the iso_graphics_shader
 */

ISO_API_INTERNAL iso_graphics_shader* iso_gl_shader_new(iso_graphics* graphics, iso_graphics_shader_def def);

/*
 * @brief Function to set the opengl uniform
 * @param graphics = Pointer to the iso_graphics
 * @param def      = iso_graphics_unform_def struct
 */

ISO_API_INTERNAL void iso_gl_uniform_set(iso_graphics* graphics, iso_graphics_uniform_def def);

/*
 * @brief Function to bind shader
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the shader
 */

ISO_API_INTERNAL void iso_gl_shader_bind(iso_graphics* graphics, iso_str name);

/*
 * @brief Function to unbind shader
 * @param graphics = Pointer to the iso_graphics
 */

ISO_API_INTERNAL void iso_gl_shader_unbind(iso_graphics* graphics);

/*
 * @brief Function to delete opengl shader
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the shader
 */

ISO_API_INTERNAL void iso_gl_shader_delete(iso_graphics* graphics, iso_str name);



/*=========================
 * Opengl Texture
 *=======================*/

/*
 * @brief Function to create new opengl texture
 * @param graphics = Pointer to the iso_graphics
 * @param def      = iso_graphics_texture_def struct
 * @return Returns the pointer to the iso_graphics_texture
 */

ISO_API_INTERNAL iso_graphics_texture* iso_gl_texture_new(iso_graphics* graphics, iso_graphics_texture_def def);

/*
 * @brief Function that updates the texture's pixel data
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the texture
 * @param def      = iso_graphics_texture_update_def struct
 */

ISO_API_INTERNAL void iso_gl_texture_update(iso_graphics* graphics, iso_str name, iso_graphics_texture_update_def def);

/*
 * @brief Function to bind texture
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the texture
 */

ISO_API_INTERNAL void iso_gl_texture_bind(iso_graphics* graphics, iso_str name);

/*
 * @brief Function to unbind texture
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the texture
 */

ISO_API_INTERNAL void iso_gl_texture_unbind(iso_graphics* graphics, iso_str name);

/*
 * @brief Function to delete opengl texture
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the texture
 */

ISO_API_INTERNAL void iso_gl_texture_delete(iso_graphics* graphics, iso_str name);



/*=========================
 * Opengl Render pipeline
 *=======================*/

/*
 * @brief Function to create opengl render pipleline according to its definition
 * @param graphics = Pointer to the iso_graphics
 * @param def      = Render pipeline definition
 * @return Returns pointer to the iso_graphics_render_pipeline
 */

ISO_API_INTERNAL iso_graphics_render_pipeline* iso_gl_render_pipeline_new(iso_graphics* graphics, iso_graphics_render_pipeline_def def);

/*
 * @brief Function to bind all the buffers to prepare for rendering
 * @param graphics    = Pointer to the iso_graphics
 * @param name        = Name of the render_pipeline
 */

ISO_API_INTERNAL void iso_gl_render_pipeline_begin(iso_graphics* graphics, iso_str name);

/*
 * @brief Function to flush the opengl buffer with a draw call.
 * @param graphics    = Pointer to the iso_graphics
 * @param name        = Name of the render_pipeline
 * @param indices_cnt = Number of indices to render
 */

ISO_API_INTERNAL void iso_gl_render_pipeline_end(iso_graphics* graphics, iso_str name, i32 indices_cnt);

/*
 * @brief Function to delete opengl render pipeline
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the render pipeline
 */

ISO_API_INTERNAL void iso_gl_render_pipeline_delete(iso_graphics* graphics, iso_str name);


#endif // __ISO_OPENGL_BACKEND_H__
