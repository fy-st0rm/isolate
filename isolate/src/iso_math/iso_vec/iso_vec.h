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

ISO_API iso_vec2 iso_add_vec2(iso_vec2 v1, iso_vec2 v2);
ISO_API iso_vec3 iso_add_vec3(iso_vec3 v1, iso_vec3 v2);
ISO_API iso_vec4 iso_add_vec4(iso_vec4 v1, iso_vec4 v2);

ISO_API iso_vec2 iso_sub_vec2(iso_vec2 v1, iso_vec2 v2);
ISO_API iso_vec3 iso_sub_vec3(iso_vec3 v1, iso_vec3 v2);
ISO_API iso_vec4 iso_sub_vec4(iso_vec4 v1, iso_vec4 v2);

ISO_API iso_vec2 iso_mul_vec2(iso_vec2 v1, iso_vec2 v2);
ISO_API iso_vec3 iso_mul_vec3(iso_vec3 v1, iso_vec3 v2);
ISO_API iso_vec4 iso_mul_vec4(iso_vec4 v1, iso_vec4 v2);

ISO_API iso_vec2 iso_div_vec2(iso_vec2 v1, iso_vec2 v2);
ISO_API iso_vec3 iso_div_vec3(iso_vec3 v1, iso_vec3 v2);
ISO_API iso_vec4 iso_div_vec4(iso_vec4 v1, iso_vec4 v2);

#endif // __ISO_VEC_H__