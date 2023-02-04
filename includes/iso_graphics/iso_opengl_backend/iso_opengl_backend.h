#ifndef __ISO_OPENGL_BACKEND_H__
#define __ISO_OPENGL_BACKEND_H__

#include "iso_util/iso_includes.h"
#include "iso_util/iso_defines.h"
#include "iso_util/iso_log.h"
#include "iso_window/iso_window.h"
#include "iso_math.h"

/*
 * @brief Function to initialize opengl backend
 * @param window = Pointer to the iso_window
 */

static void iso_gl_init(iso_window* window) {

	// Setting some attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	// Creating OPENGL context
	iso_sdl_check_ptr(SDL_GL_CreateContext(window->sdl_window));

	if (glewInit() != GLEW_OK) iso_assert(0, "Failed to initialize GLEW.\n");

	// Setting up alpha channels and blendings
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

/*
 * @brief Function to update the window for opengl
 * @param window = Pointer to the iso_window
 */

static void iso_gl_update(iso_window* window) {
	SDL_GL_SwapWindow(window->sdl_window);
}

/*
 * @brief Function to clear the window
 * @param window = Pointer to the iso_window
 * @param color  = Color of the background
 */

static void iso_gl_clear_window(iso_window* window, iso_color color) {
	GLCall(glClearColor(color.r, color.g, color.b, color.a));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

/*
 * @brief Function to convert iso_buffer_usage to glEnum
 * @param usage = Iso buffer usage type
 * @return Returns opengl buffer usage enum
 */

static i32 iso_gl_buffer_usage_to_gl_enum(iso_graphics_buffer_usage_type usage) {
	i32 mode = GL_STATIC_DRAW;
	switch (usage) {
		case ISO_GRAPHICS_STREAM : mode = GL_STREAM_DRAW ; break;
		case ISO_GRAPHICS_STATIC : mode = GL_STATIC_DRAW ; break;
		case ISO_GRAPHICS_DYNAMIC: mode = GL_DYNAMIC_DRAW; break;
		default: iso_assert(0, "Unknown ISO_GRAPHICS_BUFFER_USAGE: %d\n", usage); break;
	}
	return mode;
}

/*
 * @brief Function to convert iso_graphics_data_type to gl_types
 * @param type = iso data type
 * @return Returns opengl data type
 */

static i32 iso_graphics_type_to_gl_type(iso_graphics_data_type type) {
	i32 t = GL_FLOAT;
	switch (type) {
		case ISO_GRAPHICS_FLOAT       : t = GL_FLOAT       ; break;
		case ISO_GRAPHICS_INT         : t = GL_INT         ; break;
		case ISO_GRAPHICS_UNSIGNED_INT: t = GL_UNSIGNED_INT; break;
		default: iso_assert(0, "Unknown ISO_GRAPHICS_DATA_TYPE: %d\n", type); break;
	}
	return t;
}

/*
 * @brief Function to convert iso_graphics_filter to gl_filters
 * @param filter = iso_graphics_filter
 * @return Returns opengl filter
 */

static i32 iso_graphics_filter_to_gl_filter(iso_graphics_filter filter) {
	i32 gl_f;
	switch (filter) {
		case ISO_GRAPHICS_FILTER_LINEAR : gl_f = GL_LINEAR ; break;
		case ISO_GRAPHICS_FILTER_NEAREST: gl_f = GL_NEAREST; break;
		default: iso_assert(0, "Unknown ISO_GRAPHICS_FILTER: %d\n", filter); break;
	}
	return gl_f;
}

/*=========================
 * Buffer constructions
 *=======================*/

/*
 * @brief Function to create opengl vertex buffer
 * @param graphics = Pointer to the iso_graphics
 * @param def      = Vertex buffer definition
 * @return Returns pointer to the vertex buffer
 */

static iso_graphics_vertex_buffer* iso_gl_vertex_buffer_new(iso_graphics* graphics, iso_graphics_vertex_buffer_def def) {
	iso_graphics_vertex_buffer* vbo = iso_alloc(sizeof(iso_graphics_vertex_buffer));

	iso_assert(strlen(def.name), "Name of vertex buffer is not defined.\n");

	// Initializing data
	vbo->id   = 0;
	vbo->name = iso_alloc(strlen(def.name));
	strcpy(vbo->name, def.name);

	// Generating opengl vertex buffer
	GLCall(glGenBuffers(1, &vbo->id));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo->id));

	i32 mode = iso_gl_buffer_usage_to_gl_enum(def.usage);
	if (mode == GL_STATIC_DRAW && !def.data) {
		iso_assert(0, "Data for vertex buffer cant stay NULL for ISO_GRAPHICS_STATIC usage.\n");
	}

	GLCall(glBufferData(GL_ARRAY_BUFFER, def.size, def.data, mode));

	// Saving the graphics memory
	iso_hmap_add(graphics->vertex_buffers, vbo->name, vbo);

	return vbo;
}

/*
 * @brief Function to create opengl index buffer
 * @param graphics = Pointer to the iso_graphics
 * @param def      = Vertex buffer definition
 * @return Returns pointer to the index buffer
 */

static iso_graphics_index_buffer* iso_gl_index_buffer_new(iso_graphics* graphics, iso_graphics_index_buffer_def def) {
	iso_graphics_index_buffer* ibo = iso_alloc(sizeof(iso_graphics_index_buffer));

	iso_assert(strlen(def.name), "Name of index buffer is not defined.\n");

	// Initializing data
	ibo->id   = 0;
	ibo->name = iso_alloc(strlen(def.name));
	strcpy(ibo->name, def.name);

	// Generating opengl index buffer
	GLCall(glGenBuffers(1, &ibo->id));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->id));

	i32 mode = iso_gl_buffer_usage_to_gl_enum(def.usage);
	if (mode == GL_STATIC_DRAW && !def.data) {
		iso_assert(0, "Data for index buffer cant stay NULL for ISO_GRAPHICS_STATIC usage.\n");
	}

	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, def.size, def.data, mode));

	// Saving the graphics memory
	iso_hmap_add(graphics->index_buffers, ibo->name, ibo);

	return ibo;
}

/*
 * @brief Function to compile individual opengl shader.
 * @param type = Type of shader to compile (GL_VERTEX_SHADER / GL_FRAGMENT_SHADER)
 * @param shader_src = Shader source in string form
 * @return returns the shader id
 */

static u32 iso_gl_compile_shader(u32 type, char* shader_src) {
	u32 id = glCreateShader(type);

	GLCall(glShaderSource(id, 1, &shader_src, NULL));
	GLCall(glCompileShader(id));

	// Checking error in shader
	i32 result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		i32 length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

		char* message = iso_alloc(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));

		iso_assert(0, "Failed to compile [%s shader]\n%s\n", (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment"), message);

		iso_free(message);
		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

/*
 * @brief Function to create a shader from string source.
 * @param v_src = Vertex shader source
 * @param f_src = Fragment shader source
 * @return Returns the shader program
 */

static u32 iso_gl_shader_new_from_str(char* v_src, char* f_src) {
	u32 program = glCreateProgram();

	u32 vs = iso_gl_compile_shader(GL_VERTEX_SHADER, v_src);
	u32 fs = iso_gl_compile_shader(GL_FRAGMENT_SHADER, f_src);

	// Attaching shader
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

/*
 * @brief Function to create opengl shader according to the shader defination
 * @param graphics = Pointer to the iso_graphics
 * @param def      = Shader program defination
 * @return Returns pointer to the iso_graphics_shader
 */

static iso_graphics_shader* iso_gl_shader_new(iso_graphics* graphics, iso_graphics_shader_def def) {
	iso_graphics_shader* shader = iso_alloc(sizeof(iso_graphics_shader));

	iso_assert(strlen(def.name), "Name of shader is not defined.\n");

	// Initializing data
	shader->id = 0;
	shader->name = iso_alloc(strlen(def.name));
	strcpy(shader->name, def.name);

	switch (def.source_type) {
		case ISO_SHADER_FROM_STR:
			shader->id = iso_gl_shader_new_from_str(def.v_src, def.f_src);
			break;
		case ISO_SHADER_FROM_FILE:
			iso_assert(0, "ISO_SHADER_FROM_FILE hasnt been implemented yet.\n");
			break;
		default:
			iso_assert(0, "Invalid shader source type: %d\n", def.source_type);
			break;
	}

	// Saving in graphics memory
	iso_hmap_add(graphics->shaders, shader->name, shader);

	return shader;
}

/*
 * @brief Function to create opengl render pipleline according to its definition
 * @param graphics = Pointer to the iso_graphics
 * @param def      = Render pipeline definition
 * @return Returns pointer to the iso_graphics_render_pipeline
 */

static iso_graphics_render_pipeline* iso_gl_render_pipeline_new(iso_graphics* graphics, iso_graphics_render_pipeline_def def) {
	iso_graphics_render_pipeline* pip = iso_alloc(sizeof(iso_graphics_render_pipeline));

	iso_assert(strlen(def.name), "Name of render pipeline is not defined.\n");

	// Initializing data
	pip->id = 0;
	pip->name = iso_alloc(strlen(def.name));
	strcpy(pip->name, def.name);

	// Setting buffers
	iso_assert(def.buffers.vbo, "Vertex buffer is not given to render pipeline: %s\n", pip->name);
	iso_assert(def.buffers.ibo, "Index buffer is not given to render pipeline: %s\n", pip->name);
	iso_assert(def.buffers.shader, "Shader is not given to render pipeline: %s\n", pip->name);

	pip->buffers.vbo    = def.buffers.vbo;
	pip->buffers.ibo    = def.buffers.ibo;
	pip->buffers.shader = def.buffers.shader;

	// Creating vertex array object
	GLCall(glGenVertexArrays(1, &pip->id));
	GLCall(glBindVertexArray(pip->id));

	// Calculating stride
	size_t stride = 0;
	for (i32 i = 0; i < def.amt; i++) {
		iso_graphics_vertex_layout_def layout = def.layout[i];
		stride += layout.amt * iso_graphics_get_type_size(layout.type);
	}

	// Generating layout
	size_t offset = 0;
	for (i32 i = 0; i < def.amt; i++) {
		iso_graphics_vertex_layout_def layout = def.layout[i];

		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, layout.amt, iso_graphics_type_to_gl_type(layout.type), GL_FALSE, stride, (const void*) offset));

		// Calculating offsets
		offset += layout.amt * iso_graphics_get_type_size(layout.type);
	}

	// Saving in graphics memory
	iso_hmap_add(graphics->render_pipelines, pip->name, pip);

	return pip;
}

/*
 * @brief Function to flip SDL_Surface
 * @param surface = Pointer to the SDL_Surface
 */

static void iso_gl_flip_surface(SDL_Surface* surface)  {
	SDL_LockSurface(surface);
	
	i32 pitch = surface->pitch; // row size
	char temp[pitch];
	char* pixels = (char*) surface->pixels;
	
	for(i32 i = 0; i < surface->h / 2; ++i) {
		// get pointers to the two rows to swap
		char* row1 = pixels + i * pitch;
		char* row2 = pixels + (surface->h - i - 1) * pitch;
		
		// swap rows
		memcpy(temp, row1, pitch);
		memcpy(row1, row2, pitch);
		memcpy(row2, temp, pitch);
	}
	SDL_UnlockSurface(surface);
}

/*
 * @brief Function to get the opengl image format from SDL_Surface
 * @param surface = Pointer to the SDL_Surface
 * @return Returns for format as u32
 */

static u32 iso_gl_get_color_format(SDL_Surface* surface) {
	u32 colors = surface->format->BytesPerPixel;
	u32 format;

	if (colors == 4) {   // alpha
		if (surface->format->Rmask == 0x000000ff)
			format = GL_RGBA;
		else
			format = GL_BGRA;
	} else {             // no alpha
		if (surface->format->Rmask == 0x000000ff)
			format = GL_RGB;
		else
			format = GL_BGR;
	}
	return format;
}

/*
 * @brief Packs the surface and makes the pitch same as width (reference: https://discourse.libsdl.org/t/sdl-ttf-2-0-18-surface-to-opengl-texture-not-consistent-with-ttf-2-0-15/34529/5)
 * @param surface = Pointer to SDL_Surface
 */

static void iso_gl_pack_surface(SDL_Surface* surface) {
	i32 i;
	u32 len = surface->w * surface->format->BytesPerPixel;
	u8 *src = (Uint8*) surface->pixels;
	u8 *dst = (Uint8*) surface->pixels;
	for (i = 0; i < surface->h; i++) {
		SDL_memmove(dst, src, len);
		dst += len; 
		src += surface->pitch;
	}
	surface->pitch = len;
}

/*
 * @brief Function to create new opengl texture
 * @param graphics = Pointer to the iso_graphics
 * @param def      = iso_graphics_texture_def struct
 * @return Returns the pointer to the iso_graphics_texture
 */

static iso_graphics_texture* iso_gl_texture_new(iso_graphics* graphics, iso_graphics_texture_def def) {
	iso_graphics_texture* texture = iso_alloc(sizeof(iso_graphics_texture));

	// Loading image using sdl_image
	SDL_Surface* surface = iso_sdl_check_ptr(IMG_Load(def.file_path));
	iso_gl_flip_surface(surface);
	iso_gl_pack_surface(surface);
	u32 format = iso_gl_get_color_format(surface);

	// Initialize data
	iso_assert(strlen(def.name), "Name of texture is not defined.\n");

	// Initializing data
	texture->id = 0;
	texture->width  = surface->w;
	texture->height = surface->h;
	texture->name = iso_alloc(strlen(def.name));
	strcpy(texture->name, def.name);

	// Binding the texture
	GLCall(glGenTextures(1, &texture->id));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture->id));
	
	// Setting up some basic modes to display texture
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, iso_graphics_filter_to_gl_filter(def.filter.min)));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, iso_graphics_filter_to_gl_filter(def.filter.mag)));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	
	// Sending the pixel data to opengl
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	
	SDL_FreeSurface(surface);

	// Saving in graphics memory
	iso_hmap_add(graphics->textures, texture->name, texture);

	return texture;
}

/*=========================
 * Buffer updates
 *=======================*/

/*
 * @brief Function to update opengl vertex buffer data
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the vertex buffer
 * @param def      = iso_graphics_buffer_update_def struct
 */

static void iso_gl_vertex_buffer_update(iso_graphics* graphics, char* name, iso_graphics_buffer_update_def def) {
	iso_graphics_vertex_buffer* vbo;
	iso_hmap_get(graphics->vertex_buffers, name, vbo);

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo->id));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, def.start_sz, def.end_sz, def.data));
}

/*
 * @brief Function to set the opengl uniform
 * @param graphics = Pointer to the iso_graphics
 * @param def      = iso_graphics_unform_def struct
 */

static void iso_gl_uniform_set(iso_graphics* graphics, iso_graphics_uniform_def def) {
	i32 u_loc = GLCall(glGetUniformLocation(def.shader->id, def.name));
	iso_assert(u_loc != -1, "`%s` uniform not found in shader `%s`.\n", def.name, def.shader->name);

	switch (def.type) {
		case ISO_GRAPHICS_UNIFORM_INT:
			GLCall(glUniform1i(u_loc, *((int*) def.data)));
			break;
		case ISO_GRAPHICS_UNIFORM_FLOAT:
			GLCall(glUniform1f(u_loc, *((float*) def.data)));
			break;
		case ISO_GRAPHICS_UNIFORM_VEC2:
			GLCall(glUniform2fv(u_loc, 1, (iso_vec2*) def.data));
			break;
		case ISO_GRAPHICS_UNIFORM_VEC3:
			GLCall(glUniform3fv(u_loc, 1, (iso_vec3*) def.data));
			break;
		case ISO_GRAPHICS_UNIFORM_VEC4:
			GLCall(glUniform4fv(u_loc, 1, (iso_vec4*) def.data));
			break;
		case ISO_GRAPHICS_UNIFORM_MAT4:
			GLCall(glUniformMatrix4fv(u_loc, 1, GL_FALSE, &((iso_mat4*) def.data)->m[0][0]));
			break;
		case ISO_GRAPHICS_UNIFORM_SAMPLER2D:
			iso_graphics_sampler_def* sampler = def.data;
			GLCall(glUniform1iv(u_loc, sampler->count, sampler->samplers));
			break;
		default:
			iso_assert(false, "Unknown ISO_GRAPHICS_UNIFORM_TYPE: %d\n", def.type);
			break;
	}
}

/*
 * @brief Function to bind all the buffers to prepare for rendering
 * @param graphics    = Pointer to the iso_graphics
 * @param name        = Name of the render_pipeline
 */

static void iso_gl_render_pipeline_begin(iso_graphics* graphics, char* name) {
	iso_graphics_render_pipeline* pip;
	iso_hmap_get(graphics->render_pipelines, name, pip);

	// Binding all buffers
	graphics->api.vertex_buffer_bind(graphics, pip->buffers.vbo->name);
	graphics->api.index_buffer_bind(graphics, pip->buffers.ibo->name);
	graphics->api.shader_bind(graphics, pip->buffers.shader->name);
}

/*
 * @brief Function to flush the opengl buffer with a draw call.
 * @param graphics    = Pointer to the iso_graphics
 * @param name        = Name of the render_pipeline
 * @param indices_cnt = Number of indices to render
 */

static void iso_gl_render_pipeline_end(iso_graphics* graphics, char* name, i32 indices_cnt) {
	iso_graphics_render_pipeline* pip;
	iso_hmap_get(graphics->render_pipelines, name, pip);

	// Draw call
	GLCall(glBindVertexArray(pip->id));
	GLCall(glDrawElements(GL_TRIANGLES, indices_cnt, GL_UNSIGNED_INT, NULL));
}

/*=========================
 * Buffer binds
 *=======================*/

/*
 * @brief Function to bind vertex buffer
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the vertex buffer
 */

static void iso_gl_vertex_buffer_bind(iso_graphics* graphics, char* name) {
	iso_graphics_vertex_buffer* vbo;
	iso_hmap_get(graphics->vertex_buffers, name, vbo);
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo->id));
}

/*
 * @brief Function to bind index buffer
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the index buffer
 */

static void iso_gl_index_buffer_bind(iso_graphics* graphics, char* name) {
	iso_graphics_index_buffer* ibo;
	iso_hmap_get(graphics->index_buffers, name, ibo);
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->id));
}

/*
 * @brief Function to bind shader
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the shader
 */

static void iso_gl_shader_bind(iso_graphics* graphics, char* name) {
	iso_graphics_shader* shader;
	iso_hmap_get(graphics->shaders, name, shader);
	GLCall(glUseProgram(shader->id));
}

/*
 * @brief Function to bind texture
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the texture
 */

static void iso_gl_texture_bind(iso_graphics* graphics, char* name) {
	iso_graphics_texture* texture;
	iso_hmap_get(graphics->textures, name, texture);
	GLCall(glBindTextureUnit(texture->id, texture->id));
}

/*=========================
 * Buffer destruction
 *=======================*/

/*
 * @brief Function to delete opengl vertex buffer
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the vertex buffer
 */

static void iso_gl_vertex_buffer_delete(iso_graphics* graphics, char* name) {
	iso_graphics_vertex_buffer* vbo;
	iso_hmap_get(graphics->vertex_buffers, name, vbo);

	GLCall(glDeleteBuffers(1, &vbo->id));

	iso_hmap_remove(graphics->vertex_buffers, name);
	iso_free(vbo->name);
	iso_free(vbo);
}

/*
 * @brief Function to delete opengl index buffer
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the index buffer
 */

static void iso_gl_index_buffer_delete(iso_graphics* graphics, char* name) {
	iso_graphics_index_buffer* ibo;
	iso_hmap_get(graphics->index_buffers, name, ibo);

	GLCall(glDeleteBuffers(1, &ibo->id));

	iso_hmap_remove(graphics->index_buffers, name);
	iso_free(ibo->name);
	iso_free(ibo);
}

/*
 * @brief Function to delete opengl shader
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the shader
 */

static void iso_gl_shader_delete(iso_graphics* graphics, char* name) {
	iso_graphics_shader* shader;
	iso_hmap_get(graphics->shaders, name, shader);

	GLCall(glDeleteProgram(shader->id));

	iso_hmap_remove(graphics->shaders, name);
	iso_free(shader->name);
	iso_free(shader);
}

/*
 * @brief Function to delete opengl render pipeline
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the render pipeline
 */

static void iso_gl_render_pipeline_delete(iso_graphics* graphics, char* name) {
	iso_graphics_render_pipeline* pip;
	iso_hmap_get(graphics->render_pipelines, name, pip);

	GLCall(glDeleteVertexArrays(1, &pip->id));
	
	iso_hmap_remove(graphics->render_pipelines, name);
	iso_free(pip->name);
	iso_free(pip);
}

/*
 * @brief Function to delete opengl texture
 * @param graphics = Pointer to the iso_graphics
 * @param name     = Name of the texture
 */

static void iso_gl_texture_delete(iso_graphics* graphics, char* name) {
	iso_graphics_texture* texture;
	iso_hmap_get(graphics->textures, name, texture);

	GLCall(glDeleteTextures(1, &texture->id));

	iso_hmap_remove(graphics->textures, name);
	iso_free(texture->name);
	iso_free(texture);
}

#endif // __ISO_OPENGL_BACKEND_H__
