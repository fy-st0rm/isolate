#include "object.h"

object* object_from_data(iso_vec3* positions, iso_vec3* normals, iso_vec2* texture_cords, face* faces, u32 face_cnt) {
	object* obj = iso_alloc(sizeof(object));

	obj->vertices_cnt = face_cnt * 6;
	obj->buff_size = obj->vertices_cnt * sizeof(vertex);
	obj->vertices = iso_alloc(obj->buff_size);

	for (u32 k = 0, i = 0; i < face_cnt; i++) {
		face quad[6] = {
			faces[(i * 4) + 0],
			faces[(i * 4) + 1],
			faces[(i * 4) + 2],
			faces[(i * 4) + 2],
			faces[(i * 4) + 3],
			faces[(i * 4) + 0],
		};

		for (u32 j = 0; j < 6; j++) {
			face f = quad[j];
			vertex v = {
				.pos = positions[--f.v],
				.tex_cords = texture_cords[--f.vt],
				.normal = normals[--f.vn]
			};

			obj->vertices[k++] = v;
		}
	}

	return obj;
}

void object_delete(object* obj) {
	iso_free(obj->vertices);
	iso_free(obj);
}

void object_print(object* obj) {
	i32 cnt = obj->buff_size / sizeof(vertex);
	for (i32 i = 0; i < cnt; i++) {
		vertex v = obj->vertices[i];
		printf("(%f %f %f) (%f %f) (%f %f %f)\n",
				v.pos.x, v.pos.y, v.pos.z,
				v.tex_cords.x, v.tex_cords.y,
				v.normal.x, v.normal.y, v.normal.z
		);
	}
}

void object_print_face(face f) {
	printf("v: %u, vt: %u, vn: %u\n", f.v, f.vt, f.vn);
}
