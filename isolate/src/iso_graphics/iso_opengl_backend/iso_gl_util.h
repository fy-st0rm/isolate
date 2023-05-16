#ifndef __ISO_OPENGL_UTIL_H__
#define __ISO_OPENGL_UTIL_H__

#include "iso_graphics/iso_graphics_core/iso_graphics_util.h"
#include "iso_graphics/iso_graphics_core/iso_texture.h"
#include "iso_graphics/iso_graphics_core/iso_render_pipeline.h"

/*
 * @brief Function to convert iso_buffer_usage to glEnum
 * @param usage = Iso buffer usage type
 * @return Returns opengl buffer usage enum
 */

ISO_API i32 iso_buffer_usage_to_gl_enum(iso_buffer_usage_type usage);

/*
 * @brief Function to convert iso_graphics_data_type to gl_types
 * @param type = iso data type
 * @return Returns opengl data type
 */

ISO_API i32 iso_data_type_to_gl_type(iso_data_type type);

/*
 * @brief Function to convert Opengl Shader name to strings
 * @param type = Opengl shader type
 * @return Returns the type in string
 */

ISO_API char* iso_gl_shader_to_str(u32 type);

/*
 * @brief Function to convert iso_texture_filter to opengl filter
 * @param filter = iso_texture_filter
 * @return Returns opengl filter alternative
 */

ISO_API i32 iso_texture_filter_to_gl_filter(iso_texture_filter filter);

/*
 * @brief Function to convert iso_texture_fmt to opengl format
 * @param fmt = iso_texture_fmt
 * @return Returns opengl fmt alternative
 */

ISO_API i32 iso_texture_fmt_to_gl_fmt(iso_texture_fmt fmt);

/*
 * @brief Function to convert iso_draw_type to opengl equivalent
 * @param dt = iso_draw_type
 * @return Returns opengl draw type
 */

ISO_API i32 iso_draw_type_to_gl_draw_type(iso_draw_type dt);

#endif //__ISO_OPENGL_UTIL_H__
