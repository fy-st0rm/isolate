#include "iso_vec.h"
#include "iso_math/iso_math_util.h"
#include "iso_util/iso_log.h"

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

iso_vec2 iso_vec2_add(iso_vec2 v1, iso_vec2 v2) {
	return (iso_vec2) {
		.x = v1.x + v2.x,
		.y = v1.y + v2.y
	};
}

iso_vec3 iso_vec3_add(iso_vec3 v1, iso_vec3 v2) {
	return (iso_vec3) {
		.x = v1.x + v2.x,
		.y = v1.y + v2.y,
		.z = v1.z + v2.z
	};
}

iso_vec4 iso_vec4_add(iso_vec4 v1, iso_vec4 v2) {
	return (iso_vec4) {
		.x = v1.x + v2.x,
		.y = v1.y + v2.y,
		.z = v1.z + v2.z,
		.w = v1.w + v2.w
	};
}

iso_vec2 iso_vec2_sub(iso_vec2 v1, iso_vec2 v2) {
	return (iso_vec2) {
		.x = v1.x - v2.x,
		.y = v1.y - v2.y
	};
}

iso_vec3 iso_vec3_sub(iso_vec3 v1, iso_vec3 v2) {
	return (iso_vec3) {
		.x = v1.x - v2.x,
		.y = v1.y - v2.y,
		.z = v1.z - v2.z
	};
}

iso_vec4 iso_vec4_sub(iso_vec4 v1, iso_vec4 v2) {
	return (iso_vec4) {
		.x = v1.x - v2.x,
		.y = v1.y - v2.y,
		.z = v1.z - v2.z,
		.w = v1.w - v2.w
	};
}

iso_vec2 iso_vec2_mul(iso_vec2 v1, iso_vec2 v2) {
	return (iso_vec2) {
		.x = v1.x * v2.x,
		.y = v1.y * v2.y
	};
}

iso_vec3 iso_vec3_mul(iso_vec3 v1, iso_vec3 v2) {
	return (iso_vec3) {
		.x = v1.x * v2.x,
		.y = v1.y * v2.y,
		.z = v1.z * v2.z
	};
}

iso_vec4 iso_vec4_mul(iso_vec4 v1, iso_vec4 v2) {
	return (iso_vec4) {
		.x = v1.x * v2.x,
		.y = v1.y * v2.y,
		.z = v1.z * v2.z,
		.w = v1.w * v2.w
	};
}

iso_vec2 iso_vec2_mul_scalar(iso_vec2 v, f32 scalar) {
	return (iso_vec2) {
		.x = v.x * scalar,
		.y = v.y * scalar
	};
}

iso_vec3 iso_vec3_mul_scalar(iso_vec3 v, f32 scalar) {
	return (iso_vec3) {
		.x = v.x * scalar,
		.y = v.y * scalar,
		.z = v.z * scalar
	};
}

iso_vec4 iso_vec4_mul_scalar(iso_vec4 v, f32 scalar) {
	return (iso_vec4) {
		.x = v.x * scalar,
		.y = v.y * scalar,
		.z = v.z * scalar,
		.w = v.w * scalar
	};
}

iso_vec2 iso_vec2_div(iso_vec2 v1, iso_vec2 v2) {
	return (iso_vec2) {
		.x = v1.x / v2.x,
		.y = v1.y / v2.y
	};
}

iso_vec3 iso_vec3_div(iso_vec3 v1, iso_vec3 v2) {
	return (iso_vec3) {
		.x = v1.x / v2.x,
		.y = v1.y / v2.y,
		.z = v1.z / v2.z
	};
}

iso_vec4 iso_vec4_div(iso_vec4 v1, iso_vec4 v2) {
	return (iso_vec4) {
		.x = v1.x / v2.x,
		.y = v1.y / v2.y,
		.z = v1.z / v2.z,
		.w = v1.w / v2.w
	};
}

f32 iso_vec2_mag(iso_vec2 v) {
	return sqrt(v.x * v.x + v.y * v.y);
}

f32 iso_vec3_mag(iso_vec3 v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

f32 iso_vec4_mag(iso_vec4 v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

iso_vec2 iso_vec2_normalize(iso_vec2 v) {
	f32 r = iso_vec2_mag(v);
	return (iso_vec2) {
		.x = v.x / r,
		.y = v.y / r
	};
}

iso_vec3 iso_vec3_normalize(iso_vec3 v) {
	f32 r = iso_vec3_mag(v);
	return (iso_vec3) {
		.x = v.x / r,
		.y = v.y / r,
		.z = v.z / r
	};
}
iso_vec4 iso_vec4_normalize(iso_vec4 v) {
	f32 r = iso_vec4_mag(v);
	return (iso_vec4) {
		.x = v.x / r,
		.y = v.y / r,
		.z = v.z / r,
		.w = v.w / r
	};
}

ISO_API iso_vec2 iso_vec2_cross(iso_vec2 v1, iso_vec2 v2) {
	iso_assert(false, "iso_vec2_cross is not implemented yet.\n");
}

ISO_API iso_vec3 iso_vec3_cross(iso_vec3 v1, iso_vec3 v2) {
	return (iso_vec3) {
		.x = v1.y * v2.z - v1.z * v2.y,
		.y = - (v1.x * v2.z - v1.z * v2.x),
		.z = v1.x * v2.y - v1.y * v2.x
	};
}

ISO_API iso_vec4 iso_vec4_cross(iso_vec4 v1, iso_vec4 v2) {
	iso_assert(false, "iso_vec3_cross is not implemented yet.\n");
}
