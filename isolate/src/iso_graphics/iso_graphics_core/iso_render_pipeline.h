#ifndef __ISO_RENDER_PIPELINE_H__
#define __ISO_RENDER_PIPELINE_H__

#include "iso_util/iso_defines.h"
#include "iso_util/iso_str.h"

#include "iso_graphics_util.h"
#include "iso_vertex_buffer.h"
#include "iso_index_buffer.h"
#include "iso_shader.h"


/*
 * @brief Enum of different draw commands
 */

typedef enum {
	ISO_LINES,
	ISO_POINTS,
	ISO_TRIANGLES
} iso_draw_type;


/*
 * @brief Defination to define the vertex layout in GPU
 * @mem amt  = Amount of data for a single type
 * @mem type = Data type
 */

typedef struct {
	u32 amt;
	iso_data_type type;
} iso_vertex_layout_def;


/*
 * @brief Render Pipeline defination
 * @mem name      = Name of the pipeline
 * @mem draw_type = Draw type
 * @mem vbo       = Pointer to vertex buffer
 * @mem ibo       = Pointer to index buffer
 * @mem shader    = Pointer to shader
 * @mem layout    = Array of the vertex layouts
 * @mem amt       = Amount of layout
 */

typedef struct {
	iso_str name;
	iso_draw_type draw_type;

	struct {
		iso_vertex_buffer* vbo;
		iso_index_buffer* ibo;
		iso_shader* shader;
	} buffers;

	iso_vertex_layout_def* layout;
	u32 layout_cnt;

} iso_render_pipeline_def;


/*
 * @brief Render Pipeline Struct
 * @mem id        = Id of the buffer
 * @mem name      = Name of the pipeline
 * @mem draw_type = Draw type
 * @mem vbo       = Pointer to vertex buffer
 * @mem ibo       = Pointer to index buffer
 * @mem shader    = Pointer to shader
 */

typedef struct {
	u32 id;
	iso_str name;
	iso_draw_type draw_type;

	struct {
		iso_vertex_buffer* vbo;
		iso_index_buffer* ibo;
		iso_shader* shader;
	} buffers;

} iso_render_pipeline;


/*
 * @brief Function to create new render pipeline
 * @param def = iso_render_pipeline_def definition struct
 * @return Returns pointer to iso_render_pipeline
 */

ISO_API iso_render_pipeline* iso_render_pipeline_new(iso_render_pipeline_def def);


/*
 * @brief Function to delete render pipeline
 * @param pip = Pointer to iso_render_pipeline
 */

ISO_API void iso_render_pipeline_delete(iso_render_pipeline* pip);


/*
 * @brief Function to begin render pipeline
 * @param pip = Pointer to iso_render_pipeline
 */

ISO_API void iso_render_pipeline_begin(iso_render_pipeline* pip);


/*
 * @brief Function to end render pipeline
 * @param pip         = Pointer to iso_render_pipeline
 * @param indices_cnt = Number of indices to draw
 */

ISO_API void iso_render_pipeline_end(iso_render_pipeline* pip, i32 indices_cnt);

#endif //__ISO_RENDER_PIPELINE_H__
