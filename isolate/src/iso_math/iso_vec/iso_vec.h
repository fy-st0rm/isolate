#ifndef __ISO_VEC_H__
#define __ISO_VEC_H__

#include "iso_util/iso_defines.h"
#include "iso_util/iso_includes.h"

/*
 * @brief Vector definitions
 */

typedef struct {
	f32 x, y;
} iso_vec2;

typedef struct {
	f32 x, y, z;
} iso_vec3;

typedef struct {
    union { f32 x, r; };
    union { f32 y, g; };
    union { f32 z, b; };
    union { f32 w, a; };
} iso_vec4;


/*
 * @brief Vector print functions
 */

ISO_API void iso_print_vec2(iso_vec2 v);
ISO_API void iso_print_vec3(iso_vec3 v);
ISO_API void iso_print_vec4(iso_vec4 v);

/*
 * @brief Vector compares
 */

ISO_API b8 iso_vec2_eq(iso_vec2 v1, iso_vec2 v2);
ISO_API b8 iso_vec3_eq(iso_vec3 v1, iso_vec3 v2);
ISO_API b8 iso_vec4_eq(iso_vec4 v1, iso_vec4 v2);

/*
 * @brief Vector arithematics
 */

ISO_API iso_vec2 iso_vec2_add(iso_vec2 v1, iso_vec2 v2);
ISO_API iso_vec3 iso_vec3_add(iso_vec3 v1, iso_vec3 v2);
ISO_API iso_vec4 iso_vec4_add(iso_vec4 v1, iso_vec4 v2);

ISO_API iso_vec2 iso_vec2_sub(iso_vec2 v1, iso_vec2 v2);
ISO_API iso_vec3 iso_vec3_sub(iso_vec3 v1, iso_vec3 v2);
ISO_API iso_vec4 iso_vec4_sub(iso_vec4 v1, iso_vec4 v2);

ISO_API iso_vec2 iso_vec2_mul(iso_vec2 v1, iso_vec2 v2);
ISO_API iso_vec3 iso_vec3_mul(iso_vec3 v1, iso_vec3 v2);
ISO_API iso_vec4 iso_vec4_mul(iso_vec4 v1, iso_vec4 v2);

ISO_API iso_vec2 iso_vec2_mul_scalar(iso_vec2 v, f32 scalar);
ISO_API iso_vec3 iso_vec3_mul_scalar(iso_vec3 v, f32 scalar);
ISO_API iso_vec4 iso_vec4_mul_scalar(iso_vec4 v, f32 scalar);

ISO_API iso_vec2 iso_vec2_div(iso_vec2 v1, iso_vec2 v2);
ISO_API iso_vec3 iso_vec3_div(iso_vec3 v1, iso_vec3 v2);
ISO_API iso_vec4 iso_vec4_div(iso_vec4 v1, iso_vec4 v2);

ISO_API f32 iso_vec2_mag(iso_vec2 v);
ISO_API f32 iso_vec3_mag(iso_vec3 v);
ISO_API f32 iso_vec4_mag(iso_vec4 v);

ISO_API iso_vec2 iso_vec2_normalize(iso_vec2 v);
ISO_API iso_vec3 iso_vec3_normalize(iso_vec3 v);
ISO_API iso_vec4 iso_vec4_normalize(iso_vec4 v);

ISO_API iso_vec2 iso_vec2_cross(iso_vec2 v1, iso_vec2 v2);
ISO_API iso_vec3 iso_vec3_cross(iso_vec3 v1, iso_vec3 v2);
ISO_API iso_vec4 iso_vec4_cross(iso_vec4 v1, iso_vec4 v2);

#endif // __ISO_VEC_H__
