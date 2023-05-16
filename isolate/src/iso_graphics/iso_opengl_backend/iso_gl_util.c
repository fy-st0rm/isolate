#include "iso_gl_util.h"
#include "iso_graphics/iso_graphics_core/iso_texture.h"

i32 iso_buffer_usage_to_gl_enum(iso_buffer_usage_type usage) {
	i32 mode = GL_STATIC_DRAW;
	switch (usage) {
		case ISO_BUFFER_STREAM : mode = GL_STREAM_DRAW ; break;
		case ISO_BUFFER_STATIC : mode = GL_STATIC_DRAW ; break;
		case ISO_BUFFER_DYNAMIC: mode = GL_DYNAMIC_DRAW; break;
		default: iso_assert(0, "Unknown ISO_GRAPHICS_BUFFER_USAGE: %d\n", usage); break;
	}
	return mode;
}

i32 iso_data_type_to_gl_type(iso_data_type type) {
	i32 t = GL_FLOAT;
	switch (type) {
		case ISO_FLOAT        : t = GL_FLOAT        ; break;
		case ISO_INT          : t = GL_INT          ; break;
		case ISO_UNSIGNED_INT : t = GL_UNSIGNED_INT ; break;
		case ISO_UNSIGNED_BYTE: t = GL_UNSIGNED_BYTE; break;
		default: iso_assert(0, "Unknown ISO_GRAPHICS_DATA_TYPE: %d\n", type); break;
	}
	return t;
}

char* iso_gl_shader_to_str(u32 type) {
	switch (type) {
		case GL_VERTEX_SHADER  : return "GL_VERTEX_SHADER"  ; break;
		case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER"; break;
		default: iso_assert(0, "Unknown OPENGL_SHADER_TYPE: %d\n", type); break;
	}
}

i32 iso_texture_filter_to_gl_filter(iso_texture_filter filter) {
	switch (filter) {
		case ISO_TEXTURE_FILTER_LINEAR : return GL_LINEAR; break;
		case ISO_TEXTURE_FILTER_NEAREST: return GL_NEAREST; break;
		default: iso_assert(0, "Unknown ISO_TEXTURE_FILTER: %d\n", filter); break;
	}
}

i32 iso_texture_fmt_to_gl_fmt(iso_texture_fmt fmt) {
	switch (fmt) {
		case ISO_RGB : return GL_RGB; break;
		case ISO_RGBA: return GL_RGBA; break;
		default: iso_assert(0, "Unknown ISO_TEXTURE_FMT: %d\n", fmt); break;
	}
}

i32 iso_draw_type_to_gl_draw_type(iso_draw_type dt) {
	switch (dt) {
		case ISO_LINES: return GL_LINES; break;
		case ISO_POINTS: return GL_POINTS; break;
		case ISO_TRIANGLES: return GL_TRIANGLES; break;
		default: iso_assert(0, "Unknown ISO_DRAW_TYPE: %d\n", dt); break;
	}
}
