#include "isolate.h"

// Triangle points and indices
#define W 300.0f
#define H 300.0f
#define X (800 / 2 - W / 2)
#define Y (600 / 2 - H / 2)

#define VERTEX_BUFFER_SIZE sizeof(f32) * 4 * 7

static u32 indices[] = {
	0, 1, 2,
	2, 3, 0
};

iso_app_def iso_init() {
	return (iso_app_def) {
		.window_def = (iso_window_def) {
			.title = "Window",
			.width = 800,
			.height = 600
		},
		.graphics_def = (iso_graphics_def) {
			.api = ISO_OPENGL_API
		}
	};
}

void iso_start(iso_app* app) {

	// Vertex buffer
	iso_graphics_vertex_buffer* vbo = app->graphics->api.vertex_buffer_new(
		app->graphics,
		(iso_graphics_vertex_buffer_def) {
			.name  = "vbo",
			.data  = NULL,
			.size  = VERTEX_BUFFER_SIZE,
			.usage = ISO_GRAPHICS_DYNAMIC
		}
	);

	// Index buffer
	iso_graphics_index_buffer* ibo = app->graphics->api.index_buffer_new(
		app->graphics,
		(iso_graphics_index_buffer_def) {
			.name  = "ibo",
			.data  = indices,
			.size  = sizeof(indices),
			.usage = ISO_GRAPHICS_STATIC
		}
	);

	// Shader
	iso_graphics_shader* shader = app->graphics->api.shader_new(
		app->graphics,
		(iso_graphics_shader_def) {
			.name         = "shader",
			.v_src        = "vertex.vs",
			.f_src        = "fragment.fs",
			.source_type  = ISO_SHADER_FROM_FILE
		}
	);

	// Render pipeline
	iso_graphics_render_pipeline* pip = app->graphics->api.render_pipeline_new(
		app->graphics,
		(iso_graphics_render_pipeline_def) {
			.name = "pip",

			.buffers = {
				.vbo = vbo,
				.ibo = ibo,
				.shader = shader
			},

			.layout = (iso_graphics_vertex_layout_def[]) {
				{ .amt = 3, .type = ISO_GRAPHICS_FLOAT },
				{ .amt = 4, .type = ISO_GRAPHICS_FLOAT }
			},
			.amt = 2
		}
	);

	// Orthographic camera
	iso_camera* camera = app->camera_man->api.camera_new(
		app->camera_man,
		(iso_camera_def) {
			.name = "camera",
			.rot  = (iso_rotation) { .angle = 0.0f, .axes = (iso_vec3) { 0, 0, 1 } },
			.type = ISO_ORTHOGRAPHIC_CAMERA,
			.pos  = (iso_vec3) { 0, 0, 0 },
			.ortho_viewport = (iso_camera_ortho_viewport_def) {
				.left   = 0.0f,
				.right  = 800.0f,
				.top    = 600.0f,
				.bottom = 0.0f,
				.near   = -1.0f,
				.far    = 1000.0f
			}
		}
	);
}

void iso_event(iso_app* app, SDL_Event event) {
	if (event.type == SDL_QUIT) {
		app->state = ISO_APP_CLOSED;
	}
	else if (event.type == SDL_KEYDOWN) {
		iso_camera* cam = app->camera_man->memory.get_camera(app->camera_man, "camera");
		float speed = 0.1f;

		switch (event.key.keysym.sym) {
			case SDLK_w:
				cam->pos.y += speed;
				break;
			case SDLK_a:
				cam->pos.x -= speed;
				break;
			case SDLK_s:
				cam->pos.y -= speed;
				break;
			case SDLK_d:
				cam->pos.x += speed;
				break;
			case SDLK_q: {
				if (cam->rot.axes.z == -1) {
					cam->rot.axes.z = 1;
				} else {
					cam->rot.angle += speed;
				}
				break;
			}
			case SDLK_e:
				if (cam->rot.axes.z == 1) {
					cam->rot.axes.z = -1;
				} else {
					cam->rot.angle += speed;
				}
				break;
		}
	}
}

f32 ang = 0.0f;
void iso_update(iso_app* app, f32 dt) {
	app->graphics->api.clear_window(app->window, (iso_color) { 0.3, 0.3, 0.3, 1 });

	app->graphics->api.render_pipeline_begin(app->graphics, "pip");

	iso_mat4 rot = iso_rotate_z(ang);
	iso_vec3 bottom_left  = { X,     Y,     1.0f };
	iso_vec3 top_left     = { X,     Y + H, 1.0f };
	iso_vec3 top_right    = { X + W, Y + H, 1.0f };
	iso_vec3 bottom_right = { X + W, Y,     1.0f };

	bottom_left  = iso_mat4_mul_vec3(rot, bottom_left);
	top_left     = iso_mat4_mul_vec3(rot, top_left);
	top_right    = iso_mat4_mul_vec3(rot, top_right);
	bottom_right = iso_mat4_mul_vec3(rot, bottom_right);
	ang += 0.001f;

	f32 vertices[] = {
		bottom_left.x, bottom_left.y, bottom_left.z, 0.0f, 0.0f, 1.0f, 1.0f,
		top_left.x, top_left.y, top_left.z, 0.0f, 1.0f, 1.0f, 1.0f,
		top_right.x, top_right.y, top_right.z, 1.0f, 1.0f, 1.0f, 1.0f,
		bottom_right.x, bottom_right.y, bottom_right.z, 1.0f, 0.0f, 1.0f, 1.0f
	};

	// Updating vertex buffer data
	app->graphics->api.vertex_buffer_update(
		app->graphics,
		"vbo",
		(iso_graphics_buffer_update_def) {
			.start_sz = 0,
			.end_sz   = sizeof(vertices),
			.data     = vertices
		}
	);

	// Updating camera
	app->camera_man->api.camera_update(app->camera_man, "camera");
	iso_mat4 mvp = app->camera_man->memory.get_camera(app->camera_man, "camera")->mvp;

	// Sending uniform
	app->graphics->api.uniform_set(
		app->graphics,
		(iso_graphics_uniform_def) {
			.name   = "mvp",
			.shader = app->graphics->memory.get_shader(app->graphics, "shader"),
			.data   = &mvp,
			.type   = ISO_GRAPHICS_UNIFORM_MAT4
		}
	);

	app->graphics->api.render_pipeline_end(app->graphics, "pip", 6);
}

void iso_exit(iso_app* app) {
	app->graphics->api.vertex_buffer_delete(app->graphics, "vbo");
	app->graphics->api.index_buffer_delete(app->graphics, "ibo");
	app->graphics->api.shader_delete(app->graphics, "shader");
	app->graphics->api.render_pipeline_delete(app->graphics, "pip");
	app->camera_man->api.camera_delete(app->camera_man, "camera");
}
