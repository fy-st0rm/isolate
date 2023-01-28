#ifndef __ISO_MATH_H__
#define __ISO_MATH_H__

#include "iso_util/iso_defines.h"

typedef struct {
	f32 x, y, z;
} iso_vec3;

typedef struct {
	f32 x, y, z, w;
} iso_vec4;

typedef struct {
	f32 r, g, b, a;
} iso_color;

#endif // __ISO_MATH_H__
