#ifndef __ISO_GRAPHICS_H__
#define __ISO_GRAPHICS_H__

#include "iso_util/iso_includes.h"
#include "iso_util/iso_defines.h"

/*
 * @brief Enum that holds supported graphics api
 */

typedef enum {
	ISO_OPENGL_API
} iso_graphics_api;

/*
 * @brief Graphics defination structure. Defines on how the graphics should be.
 * @mem api          = Graphics api to use.
 * @mem renderer_amt = Max number of renderer
 */

typedef struct {
	iso_graphics_api api;
	i32 renderer_amt;
} iso_graphics_def;

/*
 * @brief Graphics structure
 * @mem api = Current using graphics api
 */

typedef struct {
	iso_graphics_api api;
} iso_graphics;

#endif // __ISO_GRAPHICS_H__
