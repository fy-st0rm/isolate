#ifndef __ISO_GL_SHADER_H__
#define __ISO_GL_SHADER_H__

#include "iso_graphics/iso_graphics.h"

ISO_API_INTERNAL iso_shader* iso_gl_shader_new(iso_graphics* graphics, iso_shader_def def);
ISO_API_INTERNAL void iso_gl_shader_delete(iso_graphics* graphics, iso_shader* shader);
ISO_API_INTERNAL void iso_gl_shader_uniform_set(iso_graphics* graphics, iso_shader* shader, iso_uniform_def def);
ISO_API_INTERNAL void iso_gl_shader_bind(iso_graphics* graphics, iso_shader* shader);
ISO_API_INTERNAL void iso_gl_shader_unbind(iso_graphics *graphics);

ISO_API_INTERNAL u32 iso_gl_compile_shader(u32 type, iso_str shader_src);
ISO_API_INTERNAL u32 iso_gl_shader_new_from_str(iso_str v_src, iso_str f_src);
ISO_API_INTERNAL u32 iso_gl_shader_new_from_file(iso_str v_path, iso_str f_path);

#endif //__ISO_GL_SHADER_H__
