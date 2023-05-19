#include "testbed.h"
#include "controller.h"

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

void testbed_new(iso_scene* scene) {
	testbed* tb = (testbed*) scene->scene_data;
	iso_app* app = tb->app;

	// Vertex buffer
	iso_vertex_buffer_def vbo_def = {
		.name = "vbo",
		.size = sizeof(vertices),
		.data = vertices,
		.usage = ISO_BUFFER_STATIC
	};
	tb->vbo = iso_vertex_buffer_new(vbo_def);


	// Index buffer
	iso_index_buffer_def ibo_def = {
		.name = "ibo",
		.size = sizeof(indices),
		.data = indices,
		.usage = ISO_BUFFER_STATIC
	};
	tb->ibo = iso_index_buffer_new(ibo_def);


	// Shader
	iso_shader_def shader_def = {
		.name = "shader",
		.v_src = "../../asset/vert.glsl",
		.f_src = "../../asset/frag.glsl",
		.load_type = ISO_SHADER_FROM_FILE
	};
	tb->shader = iso_shader_new(shader_def);


	// Texture
	iso_texture_from_file_def tex_def = {
		.name = "tex",
		.file_path = "../../asset/bricks.jpg",
		.filter = {
			.min = ISO_TEXTURE_FILTER_LINEAR,
			.mag = ISO_TEXTURE_FILTER_LINEAR
		}
	};
	tb->tex = iso_texture_new_from_file(tex_def);


	// Loading texture sampler to shader
	int samplers[] = { tb->tex->id };
	iso_shader_bind(tb->shader);

	iso_uniform_def uni_def = {
		.name = "tex",
		.type = ISO_UNIFORM_SAMPLER2D,
		.data = &(iso_uniform_sampler_def) {
			.count = 1,
			.samplers = samplers
		}
	};
	iso_shader_uniform_set(tb->shader, uni_def);


	// Render pipeline
	iso_render_pipeline_def pip_def = {
		.name = "pip",
		.draw_type = ISO_TRIANGLES,

		.buffers = {
			.vbo = tb->vbo,
			.ibo = tb->ibo,
			.shader = tb->shader
		},

		.layout_cnt = 3,
		.layout  = (iso_vertex_layout_def[]) {
			{ .amt = 3, .type = ISO_FLOAT },
			{ .amt = 4, .type = ISO_FLOAT },
			{ .amt = 2, .type = ISO_FLOAT }
		}
	};
	tb->pip = iso_render_pipeline_new(pip_def);

	// Camera controller
	tb->con = controller_new((iso_vec3) { 0.0f, 0.0f, 6.0f }, (iso_vec3) { 0.0f, 0.0f, -1.0f });

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
}

void testbed_delete(iso_scene* scene) {
	testbed* tb = scene->scene_data;
	iso_app* app = tb->app;

	// Deleting buffers
	iso_vertex_buffer_delete(tb->vbo);
	iso_index_buffer_delete(tb->ibo);
	iso_shader_delete(tb->shader);
	iso_texture_delete(tb->tex);
	iso_render_pipeline_delete(tb->pip);

	controller_delete(tb->con);

	iso_free(scene->scene_data);
}

void testbed_on_entry(iso_scene* scene) {
}

void testbed_on_exit(iso_scene* scene) {
}

static f32 ang = 0.0f;

void testbed_on_update(iso_scene* scene, f32 dt) {
	testbed* tb = scene->scene_data;
	iso_app* app = tb->app;

	iso_window_clear(app->window, (iso_vec4) { 0, 0, 0, 0 });

	// Rendering
	iso_render_pipeline_begin(tb->pip);

	// Projection matrix
	f32 aspect_ratio = (f32) WIDTH / (f32) HEIGHT;
	f32 fov = 45.0f;
	f32 near = 1.0f;
	f32 far = 1000.0f;
	iso_mat4 proj = iso_persp_projection(aspect_ratio, fov, near, far);

	// Model matrix
	ang += 0.02f;
	iso_mat4 model_rot = iso_mat4_mul(iso_rotate_z(ang), iso_rotate_x(ang));
	iso_vec3 model_pos = { 0.0f, 0.0f, 0.0f };
	iso_mat4 model_translation = iso_mat4_translate(iso_mat4_identity(), model_pos);
	iso_mat4 model = iso_mat4_mul(model_translation, model_rot);

	// View matrix
	controller_update(tb->con, app, dt);
	iso_mat4 look_at = tb->con->look_at;
	iso_mat4 view = iso_mat4_mul(look_at, model);

	// Model View Projection
	iso_mat4 mvp = iso_mat4_mul(proj, view);

	// Sending mvp uniform
	iso_uniform_def uni_def = {
		.name = "mvp",
		.type = ISO_UNIFORM_MAT4,
		.data = &mvp
	};
	iso_shader_uniform_set(tb->shader, uni_def);

	// Binding texture
	iso_texture_bind(tb->tex);

	// Flushing draw call
	iso_render_pipeline_end(tb->pip, sizeof(indices) / sizeof(indices[0]));
}

void testbed_on_event(iso_scene* scene, SDL_Event event, f32 dt) {
	testbed* tb = scene->scene_data;
	iso_app* app = tb->app;
	controller_event(tb->con, event, dt);
}
