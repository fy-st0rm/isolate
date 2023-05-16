#include "iso_gl_texture.h"
#include "iso_gl_util.h"

iso_texture* iso_gl_texture_new_from_file(iso_graphics* graphics, iso_texture_from_file_def def) {
	iso_log_info("Loading texture from file: `%s`...\n", def.file_path);

	iso_texture* texture = iso_alloc(sizeof(iso_texture));

	// Checking the name
	iso_str tmp = iso_str_new(def.name);
	iso_assert(iso_str_len(tmp), "Name of texture is not defined.\n");

	// Loading image using sdl_image
	SDL_Surface* surface = iso_sdl_check_ptr(IMG_Load(def.file_path));
	iso_gl_flip_surface(surface);
	iso_gl_pack_surface(surface);
	u32 format = iso_gl_get_color_format(surface);

	// Initializing data
	texture->id = 0;
	texture->width  = surface->w;
	texture->height = surface->h;
	texture->name   = iso_str_new(tmp);

	// Binding the texture
	GLCall(glGenTextures(1, &texture->id));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture->id));
	
	// Setting up some basic modes to display texture
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, iso_texture_filter_to_gl_filter(def.filter.min)));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, iso_texture_filter_to_gl_filter(def.filter.mag)));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	
	// Sending the pixel data to opengl
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	
	SDL_FreeSurface(surface);

	// Saving in graphics memory
	iso_hmap_add(graphics->memory, texture->name, texture);
	iso_log_sucess("Created opengl_texture: (Name: `%s` ID: %d Res: %dx%d)\n", texture->name, texture->id, texture->width, texture->height);

	iso_str_delete(tmp);
	return texture;
}

iso_texture* iso_gl_texture_new_from_data(iso_graphics* graphics, iso_texture_from_data_def def) {
	iso_log_info("Loading texture from data...\n");

	iso_texture* texture = iso_alloc(sizeof(iso_texture));

	// Checking the name
	iso_str tmp = iso_str_new(def.name);
	iso_assert(iso_str_len(tmp), "Name of texture is not defined.\n");

	// Initializing data
	texture->id = 0;
	texture->width  = def.width;
	texture->height = def.height;
	texture->name   = iso_str_new(tmp);

	// Binding the texture
	GLCall(glGenTextures(1, &texture->id));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture->id));
	
	// Setting up some basic modes to display texture
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, iso_texture_filter_to_gl_filter(def.filter.min)));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, iso_texture_filter_to_gl_filter(def.filter.mag)));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	
	// Sending the pixel data to opengl
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, iso_texture_fmt_to_gl_fmt(def.fmt), GL_UNSIGNED_BYTE, def.pixels));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	// Saving in graphics memory
	iso_hmap_add(graphics->memory, texture->name, texture);
	iso_log_sucess("Created opengl_texture: (Name: `%s` ID: %d Res: %dx%d)\n", texture->name, texture->id, texture->width, texture->height);

	iso_str_delete(tmp);
	return texture;
}

void iso_gl_texture_delete(iso_graphics* graphics, iso_texture* texture) {
	iso_assert(texture, "Tried deleting uninitialized iso_texture\n");
	iso_log_info("Deleting opengl_texture: `%s`...\n", texture->name);

	GLCall(glDeleteTextures(1, &texture->id));

	iso_log_sucess("Deleted opengl_texture: `%s`\n", texture->name);

	iso_hmap_remove(graphics->memory, texture->name);
	iso_str_delete(texture->name);
	iso_free(texture);
}

void iso_gl_texture_update(iso_graphics* graphics, iso_texture* texture, iso_texture_update_def def) {
	iso_assert(texture, "Tried updating uninitialized iso_texture\n");
	GLCall(glTextureSubImage2D(
		texture->id, def.lod, def.x_offset, def.y_offset,
		def.width, def.height, def.format,
		GL_UNSIGNED_BYTE, def.pixels
	));
}

void iso_gl_texture_bind(iso_graphics* graphics, iso_texture* texture) {
	iso_assert(texture, "Tried binding uninitialized iso_texture\n");
	GLCall(glBindTextureUnit(texture->id, texture->id));
}

void iso_gl_texture_unbind(iso_graphics* graphics, iso_texture* texture) {
	iso_assert(texture, "Tried unbinding uninitialized iso_texture\n");
	GLCall(glBindTextureUnit(texture->id, 0));
}

void iso_gl_flip_surface(SDL_Surface* surface)  {
	SDL_LockSurface(surface);
	
	i32 pitch = surface->pitch; // row size
	char temp[pitch];
	iso_str pixels = (iso_str) surface->pixels;
	
	for(i32 i = 0; i < surface->h / 2; ++i) {
		// get pointers to the two rows to swap
		iso_str row1 = pixels + i * pitch;
		iso_str row2 = pixels + (surface->h - i - 1) * pitch;
		
		// swap rows
		memcpy(temp, row1, pitch);
		memcpy(row1, row2, pitch);
		memcpy(row2, temp, pitch);
	}
	SDL_UnlockSurface(surface);
}

u32 iso_gl_get_color_format(SDL_Surface* surface) {
	u32 colors = surface->format->BytesPerPixel;
	u32 format;

	if (colors == 4) {   // alpha
		if (surface->format->Rmask == 0x000000ff)
			format = GL_RGBA;
		else
			format = GL_BGRA;
	} else {             // no alpha
		if (surface->format->Rmask == 0x000000ff)
			format = GL_RGB;
		else
			format = GL_BGR;
	}
	return format;
}

/*
	reference: https://discourse.libsdl.org/t/sdl-ttf-2-0-18-surface-to-opengl-texture-not-consistent-with-ttf-2-0-15/34529/5
 */

void iso_gl_pack_surface(SDL_Surface* surface) {
	i32 i;
	u32 len = surface->w * surface->format->BytesPerPixel;
	u8 *src = (Uint8*) surface->pixels;
	u8 *dst = (Uint8*) surface->pixels;
	for (i = 0; i < surface->h; i++) {
		SDL_memmove(dst, src, len);
		dst += len; 
		src += surface->pitch;
	}
	surface->pitch = len;
}
