#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "isolate.h"

#define VERTEX_STRIDE_CNT 8
typedef struct {
	iso_vec3 pos;
	iso_vec2 tex_cords;
	iso_vec3 normal;
} vertex;

typedef struct {
	u32 v, vt, vn;
} face;

typedef struct {
	vertex* vertices;
	size_t vertices_cnt;
	size_t buff_size;
} object;

object* object_from_data(iso_vec3* positions, iso_vec3* normals, iso_vec2* texture_cords, face* faces, u32 face_cnt);
void object_delete(object* obj);

void object_print(object* obj);
void object_print_face(face f);

#endif // __OBJECT_H__
