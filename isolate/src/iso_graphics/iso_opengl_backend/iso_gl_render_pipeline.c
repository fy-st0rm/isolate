#include "iso_gl_render_pipeline.h"
#include "iso_gl_util.h"

b8 iso_validate_draw_type(iso_draw_type type) {
	switch (type) {
		case ISO_LINES:
		case ISO_POINTS:
		case ISO_TRIANGLES:
			return true;
		default:
			return false;
	}
}

b8 iso_validate_render_type(iso_render_type type) {
	switch (type) {
		case ISO_RENDER_USING_VBO:
		case ISO_RENDER_USING_IBO:
			return true;
		default:
			return false;
	}
}

iso_str iso_draw_type_to_str(iso_draw_type type) {
	iso_assert(iso_validate_draw_type(type), "Unknown iso_draw_type: %d\n", type);

	switch (type) {
		case ISO_LINES    : return "ISO_LINES";
		case ISO_POINTS   : return "ISO_POINTS";
		case ISO_TRIANGLES: return "ISO_TRIANGLES";
	}
}

iso_str iso_render_type_to_str(iso_render_type type) {
	iso_assert(iso_validate_render_type(type), "Unknown iso_render_type: %d\n", type);

	switch (type) {
		case ISO_RENDER_USING_VBO: return "ISO_RENDER_USING_VBO";
		case ISO_RENDER_USING_IBO: return "ISO_RENDER_USING_IBO";
	}
}

iso_render_pipeline* iso_gl_render_pipeline_new(iso_graphics* graphics, iso_render_pipeline_def def) {
	iso_log_info("Constructing render pipeline...\n");

	iso_render_pipeline* pip = iso_alloc(sizeof(iso_render_pipeline));

	// Checking the name
	iso_str tmp = iso_str_new(def.name);
	iso_assert(iso_str_len(tmp), "Name of render pipeline is not defined.\n");

	// Validation
	iso_assert(iso_validate_draw_type(def.draw_type), "Unknown iso_draw_type: %d\n", def.draw_type);
	iso_assert(iso_validate_render_type(def.render_type), "Unknown iso_render_type: %d\n", def.render_type);

	// Initializing data
	pip->id = 0;
	pip->name = iso_str_new(tmp);
	pip->draw_type = def.draw_type;
	pip->render_type = def.render_type;

	// Setting buffers
	iso_assert(def.buffers.vbo, "Vertex buffer is not given to render pipeline: %s\n", pip->name);
	iso_assert(def.buffers.shader, "Shader is not given to render pipeline: %s\n", pip->name);
	
	if (def.render_type == ISO_RENDER_USING_IBO)
		iso_assert(def.buffers.ibo, "Index buffer is not given to render pipeline: %s\n", pip->name);

	pip->buffers.vbo    = def.buffers.vbo;
	pip->buffers.ibo    = def.buffers.ibo;
	pip->buffers.shader = def.buffers.shader;

	// Creating vertex array object
	GLCall(glGenVertexArrays(1, &pip->id));
	GLCall(glBindVertexArray(pip->id));

	// Binding the buffers
	graphics->api.vertex_buffer_bind(graphics, pip->buffers.vbo);
	graphics->api.shader_bind(graphics, pip->buffers.shader);

	if (def.render_type == ISO_RENDER_USING_IBO)
		graphics->api.index_buffer_bind(graphics, pip->buffers.ibo);

	// Calculating stride
	size_t stride = 0;
	for (i32 i = 0; i < def.layout_cnt; i++) {
		iso_vertex_layout_def layout = def.layout[i];
		stride += layout.amt * iso_data_type_size(layout.type);
	}

	iso_log_info("Setting up vertex attribute layouts...\n");

	// Generating layout
	// TODO: Improve this because "glVertexAttribPointer" doesnt support integers as types use "glVertexAttribIPointer"
	size_t offset = 0;
	for (i32 i = 0; i < def.layout_cnt; i++) {
		iso_vertex_layout_def layout = def.layout[i];

		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, layout.amt, iso_data_type_to_gl_type(layout.type), GL_FALSE, stride, (const void*) offset));

		// Calculating offsets
		offset += layout.amt * iso_data_type_size(layout.type);
	}

	// Unbinding all
	GLCall(glBindVertexArray(0));
	graphics->api.vertex_buffer_unbind(graphics);
	graphics->api.shader_unbind(graphics);

	if (def.render_type == ISO_RENDER_USING_IBO)
		graphics->api.index_buffer_unbind(graphics);

	// Saving in graphics memory
	iso_hmap_add(graphics->memory, pip->name, pip);
	iso_log_sucess(
			"Created opengl_render_pipeline: (Name:`%s` ID: %d Layouts: %d Draw_Type: %s Render_Type: %s)\n",
			pip->name, pip->id, def.layout_cnt, iso_draw_type_to_str(pip->draw_type), iso_render_type_to_str(pip->render_type)
	);

	iso_str_delete(tmp);
	return pip;
}

void iso_gl_render_pipeline_delete(iso_graphics* graphics, iso_render_pipeline* pip) {
	iso_assert(pip, "Tried deleting uninitialized iso_render_pipeline\n");
	iso_log_info("Deleting opengl_render_pipeline: `%s`...\n", pip->name);

	GLCall(glDeleteVertexArrays(1, &pip->id));

	iso_log_sucess("Deleted opengl_render_pipeline: `%s`\n", pip->name);
	
	iso_hmap_remove(graphics->memory, pip->name);
	iso_str_delete(pip->name);
	iso_free(pip);
}

void iso_gl_render_pipeline_begin(iso_graphics* graphics, iso_render_pipeline* pip) {
	iso_assert(pip, "Tried to begin uninitialized iso_render_pipeline\n");

	graphics->api.vertex_buffer_bind(graphics, pip->buffers.vbo);
	graphics->api.shader_bind(graphics, pip->buffers.shader);

	if (pip->render_type == ISO_RENDER_USING_IBO)
		graphics->api.index_buffer_bind(graphics, pip->buffers.ibo);
}

void iso_gl_render_pipeline_end(iso_graphics* graphics, iso_render_pipeline* pip, i32 indices_cnt) {
	iso_assert(pip, "Tried to end uninitialized iso_render_pipeline\n");

	// Draw call
	GLCall(glBindVertexArray(pip->id));

	switch (pip->render_type) {
		case ISO_RENDER_USING_IBO:
			GLCall(glDrawElements(iso_draw_type_to_gl_draw_type(pip->draw_type), indices_cnt, GL_UNSIGNED_INT, NULL));
			break;
		case ISO_RENDER_USING_VBO:
			GLCall(glDrawArrays(iso_draw_type_to_gl_draw_type(pip->draw_type), 0, indices_cnt));
			break;
		default:
			iso_assert(false, "Unknown iso_render_type: %d\n", pip->render_type);
			break;
	}
}
