#include "testbed.h"

f32 vertices[] = {
	0.0f, 0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f
};

i32 indices[] = {
	0, 1, 2
};

void testbed_new(iso_scene* scene) {
	testbed* tb = (testbed*) scene->scene_data;
	iso_app* app = tb->app;

	tb->vbo = app->graphics->api.vertex_buffer_new(
			app->graphics,
			(iso_graphics_vertex_buffer_def) {
				.name = "vbo",
				.size = sizeof(vertices),
				.data = vertices,
				.usage = ISO_GRAPHICS_STATIC
			}
	);

	tb->ibo = app->graphics->api.index_buffer_new(
			app->graphics,
			(iso_graphics_index_buffer_def) {
				.name = "ibo",
				.size = sizeof(indices),
				.data = indices,
				.usage = ISO_GRAPHICS_STATIC
			}
	);

	tb->shader = app->graphics->api.shader_new(
			app->graphics,
			(iso_graphics_shader_def) {
				.name = "shader",
				.v_src = vert_shader,
				.f_src = frag_shader,
				.source_type = ISO_GRAPHICS_SHADER_FROM_STR
			}
	);

	tb->pip = app->graphics->api.render_pipeline_new(
			app->graphics,
			(iso_graphics_render_pipeline_def) {
				.name = "pip",
				.buffers = {
					.vbo = tb->vbo,
					.ibo = tb->ibo,
					.shader = tb->shader
				},
				.amt = 1,
				.layout = (iso_graphics_vertex_layout_def[]) {
					{ .amt = 3, .type = ISO_GRAPHICS_FLOAT }
				}
			}
	);
}

void testbed_delete(iso_scene* scene) {
	iso_app* app = ((testbed*) scene->scene_data)->app;

	app->graphics->api.vertex_buffer_delete(app->graphics, "vbo");
	app->graphics->api.index_buffer_delete(app->graphics, "ibo");
	app->graphics->api.shader_delete(app->graphics, "shader");
	app->graphics->api.render_pipeline_delete(app->graphics, "pip");
	iso_free(scene->scene_data);
}

void testbed_on_entry(iso_scene* scene) {
}

void testbed_on_exit(iso_scene* scene) {
}

void testbed_on_update(iso_scene* scene, f32 dt) {
	testbed* tb = scene->scene_data;
	iso_app* app = tb->app;

	app->graphics->api.clear_window(app->window, (iso_color) { 0, 0, 0, 0 });

	app->graphics->api.render_pipeline_begin(app->graphics, "pip");
	app->graphics->api.render_pipeline_end(app->graphics, "pip", 3);
}

void testbed_on_event(iso_scene* scene, SDL_Event event, f32 dt) {
}
