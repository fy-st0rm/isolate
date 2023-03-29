#ifndef __ISO_MATH_H__
#define __ISO_MATH_H__

#include "iso_util/iso_defines.h"
#include "iso_util/iso_includes.h"

#define ISO_PI 3.14159
#define iso_radians(x) x * ISO_PI / 180
#define rand_range(l, u) rand() % (u - l + 1) + l

/*====================
 * Vector math
 *==================*/


typedef struct {
	f32 x, y;
} iso_vec2;

typedef struct {
	f32 x, y, z;
} iso_vec3;

typedef struct {
	f32 x, y, z, w;
} iso_vec4;

typedef struct {
	f32 r, g, b, a;
} iso_color;

static void iso_print_vec3(iso_vec3 v) {
	printf("\n%f %f %f\n\n", v.x, v.y, v.z);
}


/*====================
 * Matrix math
 *==================*/


typedef struct {
	f32 m[4][4];
} iso_mat4;

typedef struct {
	iso_vec3 axes;
	f32      angle;
} iso_rotation;

static void iso_mat4_clear(iso_mat4* m) {
	memset(m, 0, sizeof(*m));
}

static void iso_print_mat4(iso_mat4 m) {
	printf("\n");
	for (i32 i = 0; i < 4; i++) {
		for (i32 j = 0; j < 4; j++) {
			printf("%f\t", m.m[j][i]);
		}
		printf("\n");
	}
	printf("\n");
}

static iso_mat4 iso_mat4_mul(iso_mat4 m1, iso_mat4 m2) {
	iso_mat4 out;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			out.m[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				out.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}	
		}
	}
	return out;
}

static iso_vec3 iso_mat4_mul_vec3(iso_mat4 m, iso_vec3 v) {
	iso_vec3 out;
	out.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
	out.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
	out.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];
	f32 w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];
	if (w) {
		out.x /= w;
		out.y /= w;
		out.z /= w;
	}
	return out;
}

static iso_mat4 iso_mat4_identity() {
	iso_mat4 out;
	iso_mat4_clear(&out);
	out.m[0][0] = 1.0f;
	out.m[1][1] = 1.0f;
	out.m[2][2] = 1.0f;
	out.m[3][3] = 1.0f;
	return out;
}

static iso_mat4 iso_mat4_inverse(iso_mat4 in)
{
	iso_mat4 out;
	iso_mat4_clear(&out);
	out.m[0][0] = in.m[0][0]; 
	out.m[0][1] = in.m[1][0]; 
	out.m[0][2] = in.m[2][0]; 
	out.m[0][3] = 0.0f;
	out.m[1][0] = in.m[0][1]; 
	out.m[1][1] = in.m[1][1];
	out.m[1][2] = in.m[2][1];
	out.m[1][3] = 0.0f;
	out.m[2][0] = in.m[0][2];
	out.m[2][1] = in.m[1][2];
	out.m[2][2] = in.m[2][2];
	out.m[2][3] = 0.0f;
	out.m[3][0] = -(in.m[3][0] * out.m[0][0] + in.m[3][1] * out.m[1][0] + in.m[3][2] * out.m[2][0]);
	out.m[3][1] = -(in.m[3][0] * out.m[0][1] + in.m[3][1] * out.m[1][1] + in.m[3][2] * out.m[2][1]);
	out.m[3][2] = -(in.m[3][0] * out.m[0][2] + in.m[3][1] * out.m[1][2] + in.m[3][2] * out.m[2][2]);
	out.m[3][3] = 1.0f;
	return out;
}

static void iso_mat4_translate(iso_mat4* out, iso_vec3 v)
{
	out->m[0][0] = 1.0f;
	out->m[1][1] = 1.0f;
	out->m[2][2] = 1.0f;
	out->m[3][3] = 1.0f;
	out->m[3][0] = v.x;
	out->m[3][1] = v.y;
	out->m[3][2] = v.z;
}

static iso_mat4 iso_ortho_projection(f32 left, f32 right, f32 top, f32 bottom, f32 near, f32 far) {
	iso_mat4 out;
	iso_mat4_clear(&out);
	out.m[0][0] = 2 / (right - left);
	out.m[0][1] = 0;
	out.m[0][2] = 0;
	out.m[0][3] = 0;

	out.m[1][0] = 0;
	out.m[1][1] = 2 / (top - bottom);
	out.m[1][2] = 0;
	out.m[1][3] = 0;

	out.m[2][0] = 0;
	out.m[2][1] = 0;
	out.m[2][2] = -2 / (far - near);
	out.m[2][3] = 0;

	out.m[3][0] = -(right + left) / (right - left);
	out.m[3][1] = -(top + bottom) / (top - bottom);
	out.m[3][2] = -(far + near) / (far - near);
	out.m[3][3] = 1;
	return out;
}

static iso_mat4 iso_persp_projection(f32 aspect_ratio, f32 fov, f32 near, f32 far) {
	f32 t = tanf(iso_radians(fov/2));

	iso_mat4 out;
	iso_mat4_clear(&out);

	out.m[0][0] = (1 / (t * aspect_ratio));
	out.m[0][1] = 0;
	out.m[0][2] = 0;
	out.m[0][3] = 0;

	out.m[1][0] = 0;
	out.m[1][1] = (1 / t);
	out.m[1][2] = 0;
	out.m[1][3] = 0;

	out.m[2][0] = 0;
	out.m[2][1] = 0;
	out.m[2][2] = (far + near)/(far - near);
	out.m[2][3] = -(2 * far * near)/(far - near);

	out.m[3][0] = 0;
	out.m[3][1] = 0;
	out.m[3][2] = 1;
	out.m[3][3] = 0;

	return out;
}

static iso_mat4 iso_rotate_x(float theta)
{
	theta = iso_radians(theta);
	iso_mat4 out;
	iso_mat4_clear(&out);
	out.m[0][0] = 1.0f;
	out.m[1][1] = cosf(theta);
	out.m[1][2] = sinf(theta);
	out.m[2][1] = -sinf(theta);
	out.m[2][2] = cosf(theta);
	out.m[3][3] = 1.0f;
	return out;
}

static iso_mat4 iso_rotate_y(float theta)
{
	theta = iso_radians(theta);
	iso_mat4 out;
	iso_mat4_clear(&out);
	out.m[0][0] = cosf(theta);
	out.m[0][2] = sinf(theta);
	out.m[2][0] = -sinf(theta);
	out.m[1][1] = 1.0f;
	out.m[2][2] = cosf(theta);
	out.m[3][3] = 1.0f;
	return out;
}

static iso_mat4 iso_rotate_z(float theta)
{
	theta = iso_radians(theta);
	iso_mat4 out;
	iso_mat4_clear(&out);
	out.m[0][0] = cosf(theta);
	out.m[0][1] = sinf(theta);
	out.m[1][0] = -sinf(theta);
	out.m[1][1] = cosf(theta);
	out.m[2][2] = 1.0f;
	out.m[3][3] = 1.0f;
	return out;
}

static iso_mat4 iso_rotate(iso_rotation rot) {
	iso_mat4 out = iso_mat4_identity();

	// Positive rotation
	if (rot.axes.x > 0) out = iso_mat4_mul(out, iso_rotate_x(rot.angle));
	if (rot.axes.y > 0) out = iso_mat4_mul(out, iso_rotate_y(rot.angle));
	if (rot.axes.z > 0) out = iso_mat4_mul(out, iso_rotate_z(rot.angle));

	// Negative rotation
	if (rot.axes.z < 0) out = iso_mat4_mul(out, iso_rotate_z(-rot.angle));
	if (rot.axes.x < 0) out = iso_mat4_mul(out, iso_rotate_x(-rot.angle));
	if (rot.axes.y < 0) out = iso_mat4_mul(out, iso_rotate_y(-rot.angle));

	return out;
}

#endif // __ISO_MATH_H__
