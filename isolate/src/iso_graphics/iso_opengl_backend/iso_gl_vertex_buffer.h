#ifndef __ISO_GL_VERTEX_BUFFER_H__
#define __ISO_GL_VERTEX_BUFFER_H__

#include "iso_graphics/iso_graphics.h"

ISO_API_INTERNAL iso_vertex_buffer* iso_gl_vertex_buffer_new(iso_graphics* graphics, iso_vertex_buffer_def def);
ISO_API_INTERNAL void iso_gl_vertex_buffer_delete(iso_graphics* graphics, iso_vertex_buffer* vbo);
ISO_API_INTERNAL void iso_gl_vertex_buffer_update(iso_graphics* graphics, iso_vertex_buffer* vbo, iso_buffer_update_def def);
ISO_API_INTERNAL void iso_gl_vertex_buffer_bind(iso_graphics* graphics, iso_vertex_buffer* vbo);
ISO_API_INTERNAL void iso_gl_vertex_buffer_unbind(iso_graphics* graphics);

#endif // __ISO_GL_VERTEX_BUFFER_H__

