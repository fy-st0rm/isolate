#ifndef __ISO_GL_TEXTURE_H__
#define __ISO_GL_TEXTURE_H__

#include "iso_graphics/iso_graphics.h"

ISO_API_INTERNAL iso_texture* iso_gl_texture_new_from_file(iso_graphics* graphics, iso_texture_from_file_def def);
ISO_API_INTERNAL iso_texture* iso_gl_texture_new_from_data(iso_graphics* graphics, iso_texture_from_data_def def);
ISO_API_INTERNAL void iso_gl_texture_delete(iso_graphics* graphics, iso_texture* texture);
ISO_API_INTERNAL void iso_gl_texture_update(iso_graphics* graphics, iso_texture* texture, iso_texture_update_def def);
ISO_API_INTERNAL void iso_gl_texture_bind(iso_graphics* graphics, iso_texture* texture);
ISO_API_INTERNAL void iso_gl_texture_unbind(iso_graphics* graphics, iso_texture* texture);


ISO_API_INTERNAL void iso_gl_flip_surface(SDL_Surface* surface);
ISO_API_INTERNAL u32 iso_gl_get_color_format(SDL_Surface* surface);
ISO_API_INTERNAL void iso_gl_pack_surface(SDL_Surface* surface);

#endif //__ISO_GL_TEXTURE_H__
