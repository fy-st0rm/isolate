#include "iso_graphics_util.h"

size_t iso_data_type_size(iso_data_type type) {
	size_t sz = 0;
	switch (type) {
		case ISO_FLOAT        : sz = sizeof(f32); break;
		case ISO_INT          : sz = sizeof(i32); break;
		case ISO_UNSIGNED_INT : sz = sizeof(u32); break;
		case ISO_UNSIGNED_BYTE: sz = sizeof(u8); break;
		default: iso_assert(0, "Unknown ISO_GRAPHICS_DATA_TYPE: %d\n", type); break;
	}
	return sz;
}
