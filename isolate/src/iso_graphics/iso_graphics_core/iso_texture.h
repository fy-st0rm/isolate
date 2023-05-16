#ifndef __ISO_TEXTURE_H__
#define __ISO_TEXTURE_H__

#include "iso_util/iso_defines.h"
#include "iso_util/iso_str.h"
#include "iso_graphics_util.h"


/*
 * @brief Enum for supported texture filters.
 */

typedef enum {
	ISO_TEXTURE_FILTER_LINEAR,
	ISO_TEXTURE_FILTER_NEAREST
} iso_texture_filter;


/*
 * @brief Enum for the texture format
 */

typedef enum {
	ISO_RGB,
	ISO_RGBA
} iso_texture_fmt;


/*
 * @brief Structure definition for texture from file
 * @mem name             = Name of the texture (in engine space)
 * @mem file_path        = Path to the file
 * @mem filter(min, mag) = Texture filters
 */

typedef struct {
	iso_str name;
	iso_str file_path;

	struct {
		iso_texture_filter min, mag;
	} filter;

} iso_texture_from_file_def;


/*
 * @brief Structure definition for texture from data
 * @mem name             = Name of the texture (in engine space)
 * @mem pixels           = Path to the file
 * @mem fmt              = iso texture format
 * @mem width, height    = Width and Height of texture
 * @mem filter(min, mag) = Texture filters
 */

typedef struct {
	iso_str name;
	void* pixels;
	iso_texture_fmt fmt;
	u32 width, height;

	struct {
		iso_texture_filter min, mag;
	} filter;

} iso_texture_from_data_def;


/*
 * @brief Texture format
 * @mem id   = Texture id
 * @mem name = Name of the texture in engine
 * @mem width, height = Width and Height of the texture
 * @mem fmt  = Texture format
 */

typedef struct {
	u32 id;
	iso_str name;
	u32 width, height;
	iso_texture_fmt fmt;
} iso_texture;


/*
 * @brief Structure that holds the definition of update texture
 * @mem pixels = Pointer to the pixel buffers
 * @mem lod    = Level of details
 * @mem x_offset = X offset in buffer
 * @mem y_offset = Y offset in buffer
 * @mem width    = Width of the image
 * @mem height   = Height of the image
 * @mem format   = Pixel format
 * @mem type     = iso_data_type
 */

typedef struct {
	void* pixels;
	u32   lod;
	u32   x_offset, y_offset;
	u32   width, height;
	iso_texture_fmt format;
	iso_data_type   type;
} iso_texture_update_def;


/*
 * @brief Function to create texture from file
 * @param def = iso_texture_from_file_def struct
 */

ISO_API iso_texture* iso_texture_new_from_file(iso_texture_from_file_def def);


/*
 * @brief Function to create texture from data
 * @param def = iso_texture_from_data_def struct
 */

ISO_API iso_texture* iso_texture_new_from_data(iso_texture_from_data_def def);


/*
 * @brief Function to delete texture
 * @param texture = Pointer to iso_texture struct
 */

ISO_API void iso_texture_delete(iso_texture* texture);


/*
 * @brief Function to update the texture data
 * @param texture = Pointer to the iso_texture
 * @param def     = iso_texture_update_def struct
 */

ISO_API void iso_texture_update(iso_texture* texture, iso_texture_update_def def);

/*
 * @brief Function to bind texture
 * @param texture = Pointer to iso_texture struct
 */

ISO_API void iso_texture_bind(iso_texture* texture);


/*
 * @brief Function to unbind texture
 * @param texture = Pointer to iso_texture struct
 */

ISO_API void iso_texture_unbind(iso_texture* texture);

#endif // __ISO_TEXTURE_H__
