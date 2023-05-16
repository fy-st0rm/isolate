#include "iso_gl_vertex_buffer.h"
#include "iso_gl_util.h"

iso_vertex_buffer* iso_gl_vertex_buffer_new(iso_graphics* graphics, iso_vertex_buffer_def def) {
	iso_log_info("Constructing iso_gl_vertex_buffer...\n");

	iso_vertex_buffer* vbo = iso_alloc(sizeof(iso_vertex_buffer));

	// Checking the name
	iso_str tmp = iso_str_new(def.name);
	iso_assert(iso_str_len(tmp) > 0, "Name of vertex buffer is not defined.\n");

	// Initializing data
	vbo->id   = 0;
	vbo->name = iso_str_new(tmp);

	// Generating opengl vertex buffer
	GLCall(glGenBuffers(1, &vbo->id));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo->id));

	i32 mode = iso_buffer_usage_to_gl_enum(def.usage);
	if (mode == GL_STATIC_DRAW && !def.data) {
		iso_assert(0, "Data for vertex buffer cant stay NULL for `ISO_STATIC_DRAW` iso_buffer_usage.\n");
	}

	GLCall(glBufferData(GL_ARRAY_BUFFER, def.size, def.data, mode));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

	// Saving the graphics memory
	iso_hmap_add(graphics->memory, vbo->name, (void*)vbo);

	iso_log_sucess("Created iso_gl_vertex_buffer: (Name:`%s` ID:%d)\n", vbo->name, vbo->id);
	iso_str_delete(tmp);
	return vbo;
}

void iso_gl_vertex_buffer_delete(iso_graphics* graphics, iso_vertex_buffer* vbo) {
	iso_assert(vbo, "Tried to delete uninitialized iso_vertex_buffer\n");
	iso_log_info("Deleting iso_gl_vertex_buffer: `%s`...\n", vbo->name);

	GLCall(glDeleteBuffers(1, &vbo->id));

	iso_log_sucess("Deleted opengl_vertex_buffer: `%s`\n", vbo->name);

	// Removing from engine memory
	iso_hmap_remove(graphics->memory, vbo->name);
	iso_str_delete(vbo->name);
	iso_free(vbo);
}

void iso_gl_vertex_buffer_update(iso_graphics* graphics, iso_vertex_buffer* vbo, iso_buffer_update_def def) {
	iso_assert(vbo, "Tried to update uninitialized iso_vertex_buffer\n");

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo->id));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, def.start_sz, def.end_sz, def.data));
}

void iso_gl_vertex_buffer_bind(iso_graphics* graphics, iso_vertex_buffer* vbo) {
	iso_assert(vbo, "Tried to bind uninitialized iso_vertex_buffer\n");
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo->id));
}

void iso_gl_vertex_buffer_unbind(iso_graphics* graphics) {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
