#include "testbed.h"
#include "iso_math/iso_mat/iso_mat.h"

f32 vertices[] = {
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 0.5f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f
};

i32 indices[] = {
	0, 1, 2,
	1, 3, 4,
	5, 6, 3,
	7, 3, 6,
	2, 4, 7,
	0, 7, 6,
	0, 5, 1,
	1, 5, 3,
	5, 0, 6,
	7, 4, 3,
	2, 1, 4,
	0, 2, 7
};

/*
f32 vertices[] = {
	 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
};

i32 indices[] = {
	0, 1, 2
};
*/

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
			.v_src = "../../asset/vert.glsl",
			.f_src = "../../asset/frag.glsl",
			.source_type = ISO_GRAPHICS_SHADER_FROM_FILE
		}
	);

	tb->texture = app->graphics->api.texture_new(
		app->graphics,
		(iso_graphics_texture_def) {
			.name = "tex",
			.type = ISO_GRAPHICS_TEXTURE_FROM_FILE,
			.param = {
				.file_param = (iso_graphics_texture_from_file_param) {
					.file_path = "../../asset/bricks.jpg"
				}
			},
			.filter = {
				.min = ISO_GRAPHICS_FILTER_LINEAR,
				.mag = ISO_GRAPHICS_FILTER_LINEAR
			}
		}
	);

	int samplers[] = { tb->texture->id };
	app->graphics->api.shader_bind(app->graphics, "shader");
	app->graphics->api.uniform_set(
		app->graphics,
		(iso_graphics_uniform_def) {
			.name = "tex",
			.shader = tb->shader,
			.type = ISO_GRAPHICS_UNIFORM_SAMPLER2D,
			.data = &(iso_graphics_sampler_def) {
				.count = 1,
				.samplers = samplers
			}
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
			.amt = 3,
			.layout = (iso_graphics_vertex_layout_def[]) {
				{ .amt = 3, .type = ISO_GRAPHICS_FLOAT },
				{ .amt = 4, .type = ISO_GRAPHICS_FLOAT },
				{ .amt = 2, .type = ISO_GRAPHICS_FLOAT }
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
				.aspect_ratio = (f32) WIDTH / (f32) HEIGHT,
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

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
}

void testbed_delete(iso_scene* scene) {
	iso_app* app = ((testbed*) scene->scene_data)->app;

	app->graphics->api.vertex_buffer_delete(app->graphics, "vbo");
	app->graphics->api.index_buffer_delete(app->graphics, "ibo");
	app->graphics->api.shader_delete(app->graphics, "shader");
	app->graphics->api.texture_delete(app->graphics, "tex");
	app->graphics->api.render_pipeline_delete(app->graphics, "pip");

	iso_ortho_camera_delete(app->camera_manager, "o_cam");
	iso_persp_camera_delete(app->camera_manager, "p_cam");

	iso_free(scene->scene_data);
}

void testbed_on_entry(iso_scene* scene) {
}

void testbed_on_exit(iso_scene* scene) {
}

static f32 z_pos = 3.0f;
static f32 ang = 0.0f;
static iso_vec3 camera_pos = { 0.0f, 0.0f, -1.0f };

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


	// Projection matrix
	f32 aspect_ratio = (f32) WIDTH / (f32) HEIGHT;
	f32 fov = 45.0f;
	f32 near = 1.0f;
	f32 far = 1000.0f;
	iso_mat4 proj = iso_persp_projection(aspect_ratio, fov, near, far);

	// Model matrix
	ang += 0.02f;
	iso_mat4 model_rot = iso_mat4_mul(iso_rotate_z(ang), iso_rotate_x(ang));
	iso_vec3 model_pos = { 0.0f, 0.0f, z_pos };
	iso_mat4 model_translation = iso_mat4_translate(iso_mat4_identity(), model_pos);
	iso_mat4 model = iso_mat4_mul(model_translation, model_rot);

	// View matrix
	iso_vec3 u = { 1.0f, 0.0f, 0.0f };
	iso_vec3 v = { 0.0f, 1.0f, 0.0f };
	iso_vec3 n = { 0.0f, 0.0f, 1.0f };

	iso_mat4 camera_trans = iso_mat4_translate(iso_mat4_identity(), (iso_vec3) { -camera_pos.x, -camera_pos.y, -camera_pos.z });
	iso_mat4 camera_rot = {
		.m = {
			{ u.x, u.y, u.z, 0.0f },
			{ v.x, v.y, v.z, 0.0f },
			{ n.x, n.y, n.z, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		}
	};
	iso_mat4 camera_mat = iso_mat4_mul(camera_rot, camera_trans);
	iso_mat4 view = iso_mat4_mul(camera_mat, model);

	// Model View Projection
	iso_mat4 mvp = iso_mat4_mul(proj, view);

	app->graphics->api.uniform_set(
		app->graphics,
		(iso_graphics_uniform_def) {
			.name = "mvp",
			.type = ISO_GRAPHICS_UNIFORM_MAT4,
			.shader = tb->shader,
			.data = &mvp
		}
	);

	app->graphics->api.texture_bind(app->graphics, "tex");

	app->graphics->api.render_pipeline_end(app->graphics, "pip", sizeof(indices) / sizeof(indices[0]));
}

void testbed_on_event(iso_scene* scene, SDL_Event event, f32 dt) {
	testbed* tb = scene->scene_data;
	iso_app* app = tb->app;
	f32 speed = 0.01f;

	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
			case SDLK_w:
				camera_pos.y += speed * dt;
				break;
			case SDLK_a:
				camera_pos.x -= speed * dt;
				break;
			case SDLK_s:
				camera_pos.y -= speed * dt;
				break;
			case SDLK_d:
				camera_pos.x += speed * dt;
				break;
		}
	}
}
