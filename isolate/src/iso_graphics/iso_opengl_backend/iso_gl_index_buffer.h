#ifndef __ISO_GL_INDEX_BUFFER_H__
#define __ISO_GL_INDEX_BUFFER_H__

#include "iso_graphics/iso_graphics.h"

ISO_API_INTERNAL iso_index_buffer* iso_gl_index_buffer_new(iso_graphics* graphics, iso_index_buffer_def def);
ISO_API_INTERNAL void iso_gl_index_buffer_delete(iso_graphics* graphics, iso_index_buffer* ibo);
ISO_API_INTERNAL void iso_gl_index_buffer_update(iso_graphics* graphics, iso_index_buffer* ibo, iso_buffer_update_def def);
ISO_API_INTERNAL void iso_gl_index_buffer_bind(iso_graphics* graphics, iso_index_buffer* ibo);
ISO_API_INTERNAL void iso_gl_index_buffer_unbind(iso_graphics* graphics);

#endif //__ISO_GL_INDEX_BUFFER_H__
