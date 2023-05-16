#ifndef __ISO_GRAPHICS_UTIL_H__
#define __ISO_GRAPHICS_UTIL_H__

#include "iso_util/iso_defines.h"
#include "iso_util/iso_includes.h"
#include "iso_util/iso_log.h"
#include "iso_window/iso_window.h"
#include "iso_math/iso_math.h"

/*
 * @brief Enum of the graphics data type
 */

typedef enum {
	ISO_FLOAT,
	ISO_INT,
	ISO_UNSIGNED_INT,
	ISO_UNSIGNED_BYTE
} iso_data_type;

/*
 * @brief Enum of the buffer usage types.
 */

typedef enum {
	ISO_BUFFER_STREAM,
	ISO_BUFFER_STATIC,
	ISO_BUFFER_DYNAMIC
} iso_buffer_usage_type;

/*
 * @brief Struct that holds the data to update the buffer
 * @mem start_sz = Starting size of the data
 * @mem end_sz   = Ending size of the data
 * @mem data     = Pointer to the data
 */

typedef struct {
	size_t start_sz, end_sz;
	void* data;
} iso_buffer_update_def;

/*
 * @brief Function to get size of iso graphics data types
 * @param type = iso graphics data type
 * @return Returns the size of data type
 */

ISO_API size_t iso_data_type_size(iso_data_type type);

#endif //__ISO_GRAPHICS_UTIL_H__
