#include "isolate.h"

static char* vertex_shader = "#version 460 core\n"
"\n"
"layout(location = 0) in vec3 in_position; \n "
"\n "
"void main()\n  "
"{\n  "
"	gl_Position = vec4(in_position, 1);\n"
"};\n";

static char* fragment_shader = "#version 460 core\n"
"layout(location = 0) out vec4 color;\n"
"\n"
"void main()\n"
"{\n"
"    color = vec4(1, 0, 0, 1);\n"
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
		-0.5f, -0.5f, 0.0f,
		 0.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f
	};

	u32 indices[] = {
		0, 1, 2
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

	printf("VBO: %d\n", vbo->id);
	printf("IBO: %d\n", ibo->id);
	printf("Shader: %d\n", shader->id);
}

void iso_event(iso_app* app, SDL_Event event) {
	if (event.type == SDL_QUIT) {
		app->state = ISO_APP_CLOSED;
	}
}

void iso_update(iso_app* app, f32 dt) {
	app->graphics->api.clear_window(app->window, (iso_color) { 0, 0.5, 0.5, 1 });
	//GLCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL));
}

void iso_exit(iso_app* app) {
	app->graphics->api.vertex_buffer_delete(app->graphics, "vbo");
	app->graphics->api.index_buffer_delete(app->graphics, "ibo");
	app->graphics->api.shader_delete(app->graphics, "shader");
}
