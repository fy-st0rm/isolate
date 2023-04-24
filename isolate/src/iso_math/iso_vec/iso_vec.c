#include "iso_vec.h"
#include "iso_math/iso_math_util.h"

/*
 * @brief Vector print functions
 */

void iso_print_vec2(iso_vec2 v) {
	printf("(%f, %f)\n", v.x, v.y);
}

void iso_print_vec3(iso_vec3 v) {
	printf("(%f, %f, %f)\n", v.x, v.y, v.z);
}

void iso_print_vec4(iso_vec4 v) {
	printf("(%f, %f, %f, %f)\n", v.x, v.y, v.z, v.w);
}

/*
 * @brief Vector compares
 */

b8 iso_vec2_eq(iso_vec2 v1, iso_vec2 v2) {
	return f32_eq(v1.x, v2.x) &&
         f32_eq(v1.y, v2.y);
}

b8 iso_vec3_eq(iso_vec3 v1, iso_vec3 v2) {
	return f32_eq(v1.x, v2.x) &&
         f32_eq(v1.y, v2.y) &&
         f32_eq(v1.z, v2.z);
}

b8 iso_vec4_eq(iso_vec4 v1, iso_vec4 v2) {
	return f32_eq(v1.x, v2.x) &&
         f32_eq(v1.y, v2.y) &&
         f32_eq(v1.z, v2.z) &&
         f32_eq(v1.w, v2.w);
}

/*
 * @brief Vector arithematics
 */

iso_vec2 iso_add_vec2(iso_vec2 v1, iso_vec2 v2) {
	return (iso_vec2) {
		.x = v1.x + v2.x,
		.y = v1.y + v2.y
	};
}

iso_vec3 iso_add_vec3(iso_vec3 v1, iso_vec3 v2) {
	return (iso_vec3) {
		.x = v1.x + v2.x,
		.y = v1.y + v2.y,
		.z = v1.z + v2.z
	};
}

iso_vec4 iso_add_vec4(iso_vec4 v1, iso_vec4 v2) {
	return (iso_vec4) {
		.x = v1.x + v2.x,
		.y = v1.y + v2.y,
		.z = v1.z + v2.z,
		.w = v1.w + v2.w
	};
}

iso_vec2 iso_sub_vec2(iso_vec2 v1, iso_vec2 v2) {
	return (iso_vec2) {
		.x = v1.x - v2.x,
		.y = v1.y - v2.y
	};
}

iso_vec3 iso_sub_vec3(iso_vec3 v1, iso_vec3 v2) {
	return (iso_vec3) {
		.x = v1.x - v2.x,
		.y = v1.y - v2.y,
		.z = v1.z - v2.z
	};
}

iso_vec4 iso_sub_vec4(iso_vec4 v1, iso_vec4 v2) {
	return (iso_vec4) {
		.x = v1.x - v2.x,
		.y = v1.y - v2.y,
		.z = v1.z - v2.z,
		.w = v1.w - v2.w
	};
}

iso_vec2 iso_mul_vec2(iso_vec2 v1, iso_vec2 v2) {
	return (iso_vec2) {
		.x = v1.x * v2.x,
		.y = v1.y * v2.y
	};
}

iso_vec3 iso_mul_vec3(iso_vec3 v1, iso_vec3 v2) {
	return (iso_vec3) {
		.x = v1.x * v2.x,
		.y = v1.y * v2.y,
		.z = v1.z * v2.z
	};
}

iso_vec4 iso_mul_vec4(iso_vec4 v1, iso_vec4 v2) {
	return (iso_vec4) {
		.x = v1.x * v2.x,
		.y = v1.y * v2.y,
		.z = v1.z * v2.z,
		.w = v1.w * v2.w
	};
}

iso_vec2 iso_div_vec2(iso_vec2 v1, iso_vec2 v2) {
	return (iso_vec2) {
		.x = v1.x / v2.x,
		.y = v1.y / v2.y
	};
}

iso_vec3 iso_div_vec3(iso_vec3 v1, iso_vec3 v2) {
	return (iso_vec3) {
		.x = v1.x / v2.x,
		.y = v1.y / v2.y,
		.z = v1.z / v2.z
	};
}

iso_vec4 iso_div_vec4(iso_vec4 v1, iso_vec4 v2) {
	return (iso_vec4) {
		.x = v1.x / v2.x,
		.y = v1.y / v2.y,
		.z = v1.z / v2.z,
		.w = v1.w / v2.w
	};
}
