#ifndef __ISO_OPENGL_BACKEND_H__
#define __ISO_OPENGL_BACKEND_H__

#include "iso_gl_vertex_buffer.h"
#include "iso_gl_index_buffer.h"
#include "iso_gl_shader.h"
#include "iso_gl_texture.h"
#include "iso_gl_render_pipeline.h"

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

ISO_API_INTERNAL void iso_gl_clear_window(iso_window* window, iso_vec4 color);

#endif // __ISO_OPENGL_BACKEND_H__
