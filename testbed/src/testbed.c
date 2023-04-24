#include "testbed.h"

f32 vertices[] = {
	 0.0f,  0.5f, -3.0f,
	-0.5f, -0.5f, -3.0f,
	 0.5f, -0.5f, -3.0f
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

	tb->o_cam = iso_ortho_camera_new(
		app->camera_manager,
		(iso_ortho_camera_def) {
			.name = "o_cam",
			.pos = (iso_vec3) { 0, 0, 0 },
			.rot = (iso_rotation) {
				.angle = 0.0f,
				.axes  = { 0, 0, 0 }
			},
			.viewport = (iso_camera_ortho_viewport_def) {
				.left = -1.0f,
				.right = 1.0f,
				.bottom = -1.0f,
				.top = 1.0f,
				.near = 0.0f,
				.far = 1000.0f
			}
		}
	);

	tb->p_cam = iso_persp_camera_new(
		app->camera_manager,
		(iso_persp_camera_def) {
			.name = "p_cam",
			.pos = (iso_vec3) { 0, 0, -1 },
			.rot = (iso_rotation) {
				.angle = 0.0f,
				.axes  = (iso_vec3) { 0, 0, 0 }
			},
			.viewport = (iso_camera_persp_viewport_def) {
				.aspect_ratio = WIDTH / HEIGHT,
				.near = 1.0f,
				.far = 1000.0f,
				.fov = 45.0f
			}
		}
	);

#ifdef ORTHO
	tb->cam = tb->o_cam;
#else
	tb->cam = tb->p_cam;
#endif

}

void testbed_delete(iso_scene* scene) {
	iso_app* app = ((testbed*) scene->scene_data)->app;

	app->graphics->api.vertex_buffer_delete(app->graphics, "vbo");
	app->graphics->api.index_buffer_delete(app->graphics, "ibo");
	app->graphics->api.shader_delete(app->graphics, "shader");
	app->graphics->api.render_pipeline_delete(app->graphics, "pip");

	iso_ortho_camera_delete(app->camera_manager, "o_cam");
	iso_persp_camera_delete(app->camera_manager, "p_cam");

	iso_free(scene->scene_data);
}

void testbed_on_entry(iso_scene* scene) {
}

void testbed_on_exit(iso_scene* scene) {
}

void testbed_on_update(iso_scene* scene, f32 dt) {
	testbed* tb = scene->scene_data;
	iso_app* app = tb->app;

	app->graphics->api.clear_window(app->window, (iso_vec4) { 0, 0, 0, 0 });

	app->graphics->api.render_pipeline_begin(app->graphics, "pip");

#ifdef ORTHO
	iso_ortho_camera_update(app->camera_manager, "o_cam");
#else
	iso_persp_camera_update(app->camera_manager, "p_cam");
#endif

	app->graphics->api.uniform_set(
		app->graphics,
		(iso_graphics_uniform_def) {
			.name = "mvp",
			.type = ISO_GRAPHICS_UNIFORM_MAT4,
			.shader = tb->shader,
			.data = &tb->cam->mvp
		}
	);

	app->graphics->api.render_pipeline_end(app->graphics, "pip", 3);
}

void testbed_on_event(iso_scene* scene, SDL_Event event, f32 dt) {
	testbed* tb = scene->scene_data;
	iso_app* app = tb->app;
	f32 speed = 0.01f;

	printf("POS: ");
	iso_print_vec3(tb->cam->pos);


	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
			case SDLK_w:
				tb->cam->pos.y += speed * dt;
				break;
			case SDLK_a:
				tb->cam->pos.x -= speed * dt;
				break;
			case SDLK_s:
				tb->cam->pos.y -= speed * dt;
				break;
			case SDLK_d:
				tb->cam->pos.x += speed * dt;
				break;
		}
	}
}
