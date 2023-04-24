#ifndef __ISO_MAT_H__
#define __ISO_MAT_H__

#include "iso_util/iso_defines.h"
#include "iso_util/iso_includes.h"
#include "iso_math/iso_math_util.h"
#include "iso_math/iso_vec/iso_vec.h"

/*
 * @brief Matrix definitions
 */

typedef struct {
	f32 m[4][4];
} iso_mat4;

typedef struct {
	f32 angle;
	iso_vec3 axes;
} iso_rotation;


/*
 * @brief Matrix print functions
 */

ISO_API void iso_print_mat4(iso_mat4 m);
ISO_API void iso_mat4_clear(iso_mat4* m);

/*
 * @brief Matrix arithematics
 */

ISO_API iso_mat4 iso_mat4_mul(iso_mat4 m1, iso_mat4 m2);
ISO_API iso_vec3 iso_mat4_mul_vec3(iso_mat4 m, iso_vec3 v);

ISO_API iso_mat4 iso_mat4_identity();
ISO_API iso_mat4 iso_mat4_inverse(iso_mat4 in);
ISO_API iso_mat4 iso_mat4_translate(iso_mat4 m , iso_vec3 v);

/*
 * @brief Projection matrices
 */

ISO_API iso_mat4 iso_ortho_projection(f32 left, f32 right, f32 top, f32 bottom, f32 near, f32 far);
ISO_API iso_mat4 iso_persp_projection(f32 aspect_ratio, f32 fov, f32 near, f32 far);

/*
 * @brief Rotation matrices
 */

ISO_API iso_mat4 iso_rotate_x(f32 theta);
ISO_API iso_mat4 iso_rotate_y(f32 theta);
ISO_API iso_mat4 iso_rotate_z(f32 theta);
ISO_API iso_mat4 iso_rotate(iso_rotation rot);

#endif //__ISO_MAT_H__
