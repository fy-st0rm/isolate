#include "iso_gl_index_buffer.h"
#include "iso_gl_util.h"

iso_index_buffer* iso_gl_index_buffer_new(iso_graphics* graphics, iso_index_buffer_def def) {
	iso_log_info("Constructing iso_gl_index_buffer...\n");

	iso_index_buffer* ibo = iso_alloc(sizeof(iso_index_buffer));

	// Checking the name
	iso_str tmp = iso_str_new(def.name);
	iso_assert(iso_str_len(tmp), "Name of index buffer is not defined.\n");

	// Initializing data
	ibo->id   = 0;
	ibo->name = iso_str_new(tmp);

	// Generating opengl index buffer
	GLCall(glGenBuffers(1, &ibo->id));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->id));

	i32 mode = iso_buffer_usage_to_gl_enum(def.usage);
	if (mode == GL_STATIC_DRAW && !def.data) {
		iso_assert(0, "Data for index buffer cant stay NULL for `ISO_STATIC_DRAW` iso_buffer_usage.\n");
	}

	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, def.size, def.data, mode));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	// Saving the graphics memory
	iso_hmap_add(graphics->memory, ibo->name, ibo);

	iso_log_sucess("Created opengl_index_buffer: (Name:`%s` ID:%d)\n", ibo->name, ibo->id);
	iso_str_delete(tmp);
	return ibo;
}

void iso_gl_index_buffer_delete(iso_graphics* graphics, iso_index_buffer* ibo) {
	iso_assert(ibo, "Tried to delete uninitialized iso_index_buffer\n");
	iso_log_info("Deleting iso_gl_index_buffer: `%s`...\n", ibo->name);

	GLCall(glDeleteBuffers(1, &ibo->id));

	iso_log_sucess("Deleted opengl_index_buffer: `%s`\n", ibo->name);

	// Removing from engine memory
	iso_hmap_remove(graphics->memory, ibo->name);
	iso_str_delete(ibo->name);
	iso_free(ibo);
}

void iso_gl_index_buffer_update(iso_graphics* graphics, iso_index_buffer* ibo, iso_buffer_update_def def) {
	iso_assert(ibo, "Tried to update uninitialized iso_index_buffer\n");

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->id));
	GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, def.start_sz, def.end_sz, def.data));
}

void iso_gl_index_buffer_bind(iso_graphics* graphics, iso_index_buffer* ibo) {
	iso_assert(ibo, "Tried to bind uninitialized iso_index_buffer\n");
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->id));
}

void iso_gl_index_buffer_unbind(iso_graphics* graphics) {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
