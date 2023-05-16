#include "iso_graphics.h"
#include "iso_opengl_backend/iso_opengl_backend.h"

void __iso_load_opengl_functions(iso_graphics* graphics) {
	iso_log_info("Loading iso_opengl_backend functions...\n");

	graphics->api.init         = iso_gl_init;
	graphics->api.update       = iso_gl_update;
	graphics->api.clear_window = iso_gl_clear_window;

	// Construction
	graphics->api.vertex_buffer_new   = iso_gl_vertex_buffer_new;
	graphics->api.index_buffer_new    = iso_gl_index_buffer_new;
	graphics->api.shader_new          = iso_gl_shader_new;
	graphics->api.render_pipeline_new = iso_gl_render_pipeline_new;
	graphics->api.texture_new_from_file = iso_gl_texture_new_from_file;
	graphics->api.texture_new_from_data = iso_gl_texture_new_from_data;

	// Updates
	graphics->api.vertex_buffer_update  = iso_gl_vertex_buffer_update;
	graphics->api.index_buffer_update   = iso_gl_index_buffer_update;
	graphics->api.shader_uniform_set    = iso_gl_shader_uniform_set;
	graphics->api.render_pipeline_begin = iso_gl_render_pipeline_begin;
	graphics->api.render_pipeline_end   = iso_gl_render_pipeline_end;
	graphics->api.texture_update        = iso_gl_texture_update;

	// Binds
	graphics->api.vertex_buffer_bind = iso_gl_vertex_buffer_bind;
	graphics->api.index_buffer_bind  = iso_gl_index_buffer_bind;
	graphics->api.shader_bind        = iso_gl_shader_bind;
	graphics->api.texture_bind       = iso_gl_texture_bind;

	// Unbinds
	graphics->api.vertex_buffer_unbind = iso_gl_vertex_buffer_unbind;
	graphics->api.index_buffer_unbind  = iso_gl_index_buffer_unbind;
	graphics->api.shader_unbind        = iso_gl_shader_unbind;
	graphics->api.texture_unbind       = iso_gl_texture_unbind;

	// Destructions
	graphics->api.vertex_buffer_delete   = iso_gl_vertex_buffer_delete;
	graphics->api.index_buffer_delete    = iso_gl_index_buffer_delete;
	graphics->api.shader_delete          = iso_gl_shader_delete;
	graphics->api.render_pipeline_delete = iso_gl_render_pipeline_delete;
	graphics->api.texture_delete         = iso_gl_texture_delete;

	iso_log_sucess("Loaded iso_opengl_backend functions\n");
}

iso_graphics* iso_graphics_new(iso_graphics_def graphics_def) {
	iso_graphics* graphics = iso_alloc(sizeof(iso_graphics));

	iso_log_info("Constructing iso_graphics api: `%s` ...\n", iso_graphics_api_to_str(graphics_def.api));

	switch (graphics_def.api) {
		case ISO_OPENGL_API:
			__iso_load_opengl_functions(graphics);
			break;
		default:
			iso_assert(0, "Unknown graphics api = %d\n", graphics_def.api);
			break;
	}

	iso_log_sucess("Created iso_graphics:\n\tAPI: `%s`\n", iso_graphics_api_to_str(graphics_def.api));

	return graphics;
}

void iso_graphics_delete(iso_graphics* graphics) {
	iso_log_info("Deleting iso_graphics...\n");

	iso_hmap_delete(graphics->memory);
	iso_free(graphics);

	iso_log_sucess("Deleted iso_graphics\n");
}

void* iso_graphics_get(iso_graphics* graphics, iso_str name) {
	void* data;
	iso_hmap_get(graphics->memory, name, data);
	return data;
}

