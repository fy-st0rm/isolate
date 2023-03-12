#define ISO_IMPLEMENTATION
#include "isolate.h"

static char* vertex_shader = "#version 460 core\n"
"\n"
"layout(location = 0) in vec3 in_position; \n"
"layout(location = 1) in vec2 in_tex_cord; \n"
"\n "
"out vec2 out_tex_cord;\n"
"void main()\n  "
"{\n"
"	out_tex_cord = in_tex_cord;\n"
"	gl_Position = vec4(in_position, 1);\n"
"};\n";

static char* fragment_shader = "#version 460 core\n"
"layout(location = 0) out vec4 color;\n"
"\n"
"in vec2 out_tex_cord;\n"
"uniform sampler2D tex;\n"
"void main()\n"
"{\n"
"    color = texture(tex, out_tex_cord) * vec4(1, 1, 1, 1);\n"
"};\n";

iso_app_def iso_init() {
	iso_window_def window_def = {
		.title  = "Window",
		.width  = 800,
		.height = 600
	};

	iso_graphics_def graphics_def = {
		.api = ISO_OPENGL_API,
	};

	return (iso_app_def) {
		.window_def   = window_def,
		.graphics_def = graphics_def
	};
}

void iso_start(iso_app* app) {
	// Triangle points and indices
	f32 vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
	};
	
	u32 indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	// Vertex buffer
	iso_graphics_vertex_buffer_def vbd = {
		.name  = "vbo",
		.data  = vertices,
		.size  = sizeof(vertices),
		.usage = ISO_GRAPHICS_STATIC
	};

	iso_graphics_vertex_buffer* vbo = app->graphics->api.vertex_buffer_new(app->graphics, vbd);

	// Index buffer
	iso_graphics_index_buffer_def ibd = {
		.name  = "ibo",
		.data  = indices,
		.size  = sizeof(indices),
		.usage = ISO_GRAPHICS_STATIC
	};

	iso_graphics_index_buffer* ibo = app->graphics->api.index_buffer_new(app->graphics, ibd);

	// Shader
	iso_graphics_shader_def shader_def = {
		.name        = "shader",
		.v_src       = vertex_shader,
		.f_src       = fragment_shader,
		.source_type = ISO_SHADER_FROM_STR
	};

	iso_graphics_shader* shader = app->graphics->api.shader_new(app->graphics, shader_def);

	// Texture
	iso_graphics_texture_def tex_def = {
		.name      = "image",
		.type      = ISO_GRAPHICS_TEXTURE_FROM_FILE,
		.param     = {
			.file_param = (iso_graphics_texture_from_file_param) {
				.file_path = "image.png"
			}
		},
		.filter    = {
			.min   = ISO_GRAPHICS_FILTER_LINEAR,
			.mag   = ISO_GRAPHICS_FILTER_NEAREST
		}
	};

	iso_graphics_texture* texture = app->graphics->api.texture_new(app->graphics, tex_def);

	// Giving the texture id to shader as uniform
	app->graphics->api.shader_bind(app->graphics, "shader");
	app->graphics->api.uniform_set(app->graphics, (iso_graphics_uniform_def) {
				.name   = "tex",
				.shader = app->graphics->memory.get_shader(app->graphics, "shader"),
				.data   = &(iso_graphics_sampler_def) { 
								.count    = 1,
								.samplers = &app->graphics->memory.get_texture(app->graphics, "image")->id
							},
				.type   = ISO_GRAPHICS_UNIFORM_SAMPLER2D
			});

	// Render pipeline
	iso_graphics_render_pipeline_def pipeline_def = {
		.name = "pip",

		.buffers = {
			.vbo = vbo,
			.ibo = ibo,
			.shader = shader
		},

		.layout = (iso_graphics_vertex_layout_def[]) {
			{ .amt = 3, .type = ISO_GRAPHICS_FLOAT },
			{ .amt = 2, .type = ISO_GRAPHICS_FLOAT }
		},
		.amt = 2
	};

	iso_graphics_render_pipeline* pip = app->graphics->api.render_pipeline_new(app->graphics, pipeline_def);
}

void iso_event(iso_app* app, SDL_Event event) {
	if (event.type == SDL_QUIT) {
		app->state = ISO_APP_CLOSED;
	}
}

void iso_update(iso_app* app, f32 dt) {
	app->graphics->api.clear_window(app->window, (iso_color) { 0, 0.5, 0.5, 1 });

	app->graphics->api.render_pipeline_begin(app->graphics, "pip");
	app->graphics->api.texture_bind(app->graphics, "image");
	app->graphics->api.render_pipeline_end(app->graphics, "pip", 6);
}

void iso_exit(iso_app* app) {
	app->graphics->api.vertex_buffer_delete(app->graphics, "vbo");
	app->graphics->api.index_buffer_delete(app->graphics, "ibo");
	app->graphics->api.shader_delete(app->graphics, "shader");
	app->graphics->api.texture_delete(app->graphics, "image");
	app->graphics->api.render_pipeline_delete(app->graphics, "pip");
}

int main(int argc, char** argv) {
	return iso_run(argc, argv);
}
