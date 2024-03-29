#ifndef __CUBE_H__
#define __CUBE_H__

#include "object.h"

static iso_vec3 cube_v[] = {
	{  1.000000,  1.000000, -1.000000 },
	{  1.000000, -1.000000, -1.000000 },
	{  1.000000,  1.000000,  1.000000 },
	{  1.000000, -1.000000,  1.000000 },
	{ -1.000000,  1.000000, -1.000000 },
	{ -1.000000, -1.000000, -1.000000 },
	{ -1.000000,  1.000000,  1.000000 },
	{ -1.000000, -1.000000,  1.000000 }
};

static iso_vec3 cube_vn[] = {
	{ -0.0000,  1.0000, -0.0000 },
	{ -0.0000, -0.0000,  1.0000 },
	{ -1.0000, -0.0000, -0.0000 },
	{ -0.0000, -1.0000, -0.0000 },
	{  1.0000, -0.0000, -0.0000 },
	{ -0.0000, -0.0000, -1.0000 }
};

static iso_vec2 cube_vt[] = {
	{ 0.004310, 0.005168 },
	{ 0.678448, 0.338501 },
	{ 0.166379, 1.000000 },
	{ 0.997414, 0.664083 },
	{ 0.671552, 0.005168 },
	{ 0.166379, 0.666667 },
	{ 0.004310, 0.338501 },
	{ 0.449138, 0.338501 },
	{ 0.997414, 0.997416 },
	{ 0.449138, 0.005168 },
	{ 0.226724, 0.005168 },
	{ 0.004310, 0.671835 },
	{ 0.388793, 1.000000 },
	{ 0.004310, 0.338501 },
	{ 0.775000, 0.664083 },
	{ 0.388793, 0.666667 },
	{ 0.226724, 0.338501 },
	{ 0.226724, 0.671835 },
	{ 0.226724, 0.005168 },
	{ 0.226724, 0.338501 },
	{ 0.775000, 0.997416 }
};

static face cube_f[] = {
	{ 1, 1, 1 },
	{ 5, 11, 1 },
	{ 7, 17, 1 },
	{ 3, 7, 1 },
	{ 4, 10, 2 },
	{ 3, 8, 2 },
	{ 7, 17, 2 },
	{ 8, 19, 2 },
	{ 8, 20, 3 },
	{ 7, 18, 3 },
	{ 5, 12, 3 },
	{ 6, 14, 3 },
	{ 6, 15, 4 },
	{ 2, 4, 4 },
	{ 4, 9, 4 },
	{ 8, 21, 4 },
	{ 2, 5, 5 },
	{ 1, 2, 5 },
	{ 3, 8, 5 },
	{ 4, 10, 5 },
	{ 6, 16, 6 },
	{ 5, 13, 6 },
	{ 1, 3, 6 },
	{ 2, 6, 6 }
};
static size_t cube_f_cnt = (sizeof(cube_f) / sizeof(cube_f[0])) / 4;

#endif // __CUBE_H__
