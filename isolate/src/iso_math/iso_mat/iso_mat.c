#include "iso_mat.h"

void iso_print_mat4(iso_mat4 m) {
	for (i32 i = 0; i < 4; i++) {
		for (i32 j = 0; j < 4; j++) {
			printf("%f\t", m.m[i][j]);
		}
		printf("\n");
	}
}

void iso_mat4_clear(iso_mat4* m) {
	memset(m, 0, sizeof(iso_mat4));
}

iso_mat4 iso_mat4_mul(iso_mat4 m1, iso_mat4 m2) {
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

iso_vec3 iso_mat4_mul_vec3(iso_mat4 m, iso_vec3 v) {
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

iso_mat4 iso_mat4_identity() {
	return (iso_mat4) {
		.m = {
			{ 1.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		}
	};
}

iso_mat4 iso_mat4_inverse(iso_mat4 in) {
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

iso_mat4 iso_mat4_translate(iso_mat4 m, iso_vec3 v) {
	return (iso_mat4) {
		.m = {
			{      1.0f, m.m[0][1], m.m[0][2],  v.x },
			{ m.m[1][0],      1.0f, m.m[1][2],  v.y },
			{ m.m[2][0], m.m[2][1],      1.0f,  v.z },
			{ m.m[3][0], m.m[3][1], m.m[3][2], 1.0f },
		}
	};
}

iso_mat4 iso_ortho_projection(f32 left, f32 right, f32 top, f32 bottom, f32 near, f32 far) {
	f32 x_range = right - left;
	f32 y_range = top - bottom;
	f32 z_range = far - near;

	return (iso_mat4) {
		.m = {
			{               2 / x_range,                         0,                       0, 0 },
			{                         0,               2 / y_range,                       0, 0 },
			{                         0,                         0,            -2 / z_range, 0 },
			{ -(right + left) / x_range, -(top + bottom) / y_range, -(far + near) / z_range, 1 }
		}
	};
}

iso_mat4 iso_persp_projection(f32 aspect_ratio, f32 fov, f32 near, f32 far) {
	f32 t = tanf(iso_radians(fov / 2));
	f32 z_range = near - far;
	f32 A = (-far - near) / z_range;
	f32 B = (2 * far * near) / z_range;

	return (iso_mat4) {
		.m = {
			{ 1 / (aspect_ratio * t),     0, 0, 0 },
			{                      0, 1 / t, 0, 0 },
			{                      0,     0, A, B },
			{                      0,     0, 1, 0 }
		}
	};
}

iso_mat4 iso_rotate_x(f32 theta) {
	return (iso_mat4) {
		.m = {
			{ 1.0f,         0.0f,        0.0f, 0.0f },
			{ 0.0f,  cosf(theta), sinf(theta), 0.0f },
			{ 0.0f, -sinf(theta), cosf(theta), 0.0f },
			{ 0.0f,         0.0f,        0.0f, 1.0f }
		}
	};
}


iso_mat4 iso_rotate_y(f32 theta) {
	return (iso_mat4) {
		.m = {
			{  cosf(theta),  0.0f, sinf(theta), 0.0f },
			{         0.0f,  1.0f,        0.0f, 0.0f },
			{ -sinf(theta),  0.0f, cosf(theta), 0.0f },
			{         0.0f,  0.0f,        0.0f, 1.0f }
		}
	};
}

iso_mat4 iso_rotate_z(f32 theta) {
	return (iso_mat4) {
		.m = {
			{  cosf(theta), sinf(theta), 0.0f, 0.0f },
			{ -sinf(theta), cosf(theta), 0.0f, 0.0f },
			{         0.0f,        0.0f, 1.0f, 0.0f },
			{         0.0f,        0.0f, 0.0f, 1.0f }
		}
	};
}

iso_mat4 iso_rotate(iso_rotation rot) {
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
