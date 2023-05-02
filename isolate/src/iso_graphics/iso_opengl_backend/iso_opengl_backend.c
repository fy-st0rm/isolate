#include "iso_opengl_backend.h"

/*
 * @brief Function to initialize opengl backend
 * @param window = Pointer to the iso_window
 */

void iso_gl_init(iso_window* window) {
	iso_log_info("Initializing opengl backend...\n");

	// Setting some attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	// Creating OPENGL context
	iso_sdl_check_ptr(SDL_GL_CreateContext(window->sdl_window));

	if (glewInit() != GLEW_OK) iso_assert(0, "Failed to initialize GLEW.\n");

	// Setting up alpha channels and blendings
	GLCall(glEnable(GL_BLEND));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	iso_log_sucess("Opengl has been initialized\n");
}

/*
 * @brief Function to update the window for opengl
 * @param window = Pointer to the iso_window
 */

void iso_gl_update(iso_window* window) {
	SDL_GL_SwapWindow(window->sdl_window);
}

/*
 * @brief Function to clear the window
 * @param window = Pointer to the iso_window
 * @param color  = Color of the background
 */

void iso_gl_clear_window(iso_window* window, iso_vec4 color) {
	GLCall(glClearColor(color.r, color.g, color.b, color.a));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

/*
 * @brief Function to convert iso_buffer_usage to glEnum
 * @param usage = Iso buffer usage type
 * @return Returns opengl buffer usage enum
 */

i32 iso_gl_buffer_usage_to_gl_enum(iso_graphics_buffer_usage_type usage) {
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

i32 iso_graphics_type_to_gl_type(iso_graphics_data_type type) {
	i32 t = GL_FLOAT;
	switch (type) {
		case ISO_GRAPHICS_FLOAT        : t = GL_FLOAT        ; break;
		case ISO_GRAPHICS_INT          : t = GL_INT          ; break;
		case ISO_GRAPHICS_UNSIGNED_INT : t = GL_UNSIGNED_INT ; break;
		case ISO_GRAPHICS_UNSIGNED_BYTE: t = GL_UNSIGNED_BYTE; break;
		default: iso_assert(0, "Unknown ISO_GRAPHICS_DATA_TYPE: %d\n", type); break;
	}
	return t;
}

/*
 * @brief Function to convert iso_graphics_filter to gl_filters
 * @param filter = iso_graphics_filter
 * @return Returns opengl filter
 */

i32 iso_graphics_filter_to_gl_filter(iso_graphics_filter filter) {
	i32 gl_f;
	switch (filter) {
		case ISO_GRAPHICS_FILTER_LINEAR : gl_f = GL_LINEAR ; break;
		case ISO_GRAPHICS_FILTER_NEAREST: gl_f = GL_NEAREST; break;
		default: iso_assert(0, "Unknown ISO_GRAPHICS_FILTER: %d\n", filter); break;
	}
	return gl_f;
}


/*
 * @brief Function to convert gl_shader tyoe to string
 * @param type = u32 opengl shader type
 * @return Returns shader name in string
 */

iso_str iso_gl_shader_to_str(u32 type) {
	switch (type) {
		case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
		case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
		default: iso_assert(false, "Unknown shader type: %d\n", type);
	}
}


/*=========================
 * Opengl Vertex Buffer
 *=======================*/

iso_graphics_vertex_buffer* iso_gl_vertex_buffer_new(iso_graphics* graphics, iso_graphics_vertex_buffer_def def) {
	iso_log_info("Constructing opengl_vertex_buffer...\n");

	iso_graphics_vertex_buffer* vbo = iso_alloc(sizeof(iso_graphics_vertex_buffer));

	iso_str tmp = iso_str_new(def.name);
	iso_assert(iso_str_len(tmp) > 0, "Name of vertex buffer is not defined.\n");

	// Initializing data
	vbo->id   = 0;
	vbo->name = iso_str_new(tmp);

	// Generating opengl vertex buffer
	GLCall(glGenBuffers(1, &vbo->id));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo->id));

	i32 mode = iso_gl_buffer_usage_to_gl_enum(def.usage);
	if (mode == GL_STATIC_DRAW && !def.data) {
		iso_assert(0, "Data for vertex buffer cant stay NULL for ISO_GRAPHICS_usage.\n");
	}

	GLCall(glBufferData(GL_ARRAY_BUFFER, def.size, def.data, mode));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

	// Saving the graphics memory
	iso_hmap_add(graphics->vertex_buffers, vbo->name, vbo);

	iso_log_sucess("Created opengl_vertex_buffer: (Name:`%s` ID:%d)\n", vbo->name, vbo->id);

	iso_str_delete(tmp);
	return vbo;
}

void iso_gl_vertex_buffer_update(iso_graphics* graphics, iso_str name, iso_graphics_buffer_update_def def) {
	iso_graphics_vertex_buffer* vbo;
	iso_hmap_get(graphics->vertex_buffers, name, vbo);

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo->id));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, def.start_sz, def.end_sz, def.data));
}

void iso_gl_vertex_buffer_bind(iso_graphics* graphics, iso_str name) {
	iso_graphics_vertex_buffer* vbo;
	iso_hmap_get(graphics->vertex_buffers, name, vbo);
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo->id));
}

void iso_gl_vertex_buffer_unbind(iso_graphics* graphics) {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void iso_gl_vertex_buffer_delete(iso_graphics* graphics, iso_str name) {
	iso_log_info("Deleting opengl_vertex_buffer: `%s`...\n", name);

	iso_graphics_vertex_buffer* vbo;
	iso_hmap_get(graphics->vertex_buffers, name, vbo);

	GLCall(glDeleteBuffers(1, &vbo->id));

	iso_hmap_remove(graphics->vertex_buffers, name);
	iso_str_delete(vbo->name);
	iso_free(vbo);

	iso_log_sucess("Deleted opengl_vertex_buffer: `%s`\n", name);
}


/*=========================
 * Opengl Index Buffer
 *=======================*/


iso_graphics_index_buffer* iso_gl_index_buffer_new(iso_graphics* graphics, iso_graphics_index_buffer_def def) {
	iso_log_info("Constructing opengl_index_buffer...\n");

	iso_graphics_index_buffer* ibo = iso_alloc(sizeof(iso_graphics_index_buffer));

	iso_str tmp = iso_str_new(def.name);
	iso_assert(iso_str_len(tmp), "Name of index buffer is not defined.\n");

	// Initializing data
	ibo->id   = 0;
	ibo->name = iso_str_new(tmp);

	// Generating opengl index buffer
	GLCall(glGenBuffers(1, &ibo->id));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->id));

	i32 mode = iso_gl_buffer_usage_to_gl_enum(def.usage);
	if (mode == GL_STATIC_DRAW && !def.data) {
		iso_assert(0, "Data for index buffer cant stay NULL for ISO_GRAPHICS_usage.\n");
	}

	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, def.size, def.data, mode));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	// Saving the graphics memory
	iso_hmap_add(graphics->index_buffers, ibo->name, ibo);

	iso_log_sucess("Created opengl_index_buffer: (Name:`%s` ID:%d)\n", ibo->name, ibo->id);

	iso_str_delete(tmp);
	return ibo;
}

void iso_gl_index_buffer_update(iso_graphics* graphics, iso_str name, iso_graphics_buffer_update_def def) {
	iso_graphics_index_buffer* ibo;
	iso_hmap_get(graphics->index_buffers, name, ibo);

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->id));
	GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, def.start_sz, def.end_sz, def.data));
}

void iso_gl_index_buffer_bind(iso_graphics* graphics, iso_str name) {
	iso_graphics_index_buffer* ibo;
	iso_hmap_get(graphics->index_buffers, name, ibo);
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->id));
}

void iso_gl_index_buffer_unbind(iso_graphics* graphics) {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void iso_gl_index_buffer_delete(iso_graphics* graphics, iso_str name) {
	iso_log_info("Deleting opengl_index_buffer: `%s`...\n", name);

	iso_graphics_index_buffer* ibo;
	iso_hmap_get(graphics->index_buffers, name, ibo);

	GLCall(glDeleteBuffers(1, &ibo->id));

	iso_hmap_remove(graphics->index_buffers, name);
	iso_str_delete(ibo->name);
	iso_free(ibo);

	iso_log_sucess("Deleted opengl_index_buffer: `%s`\n", name);
}


/*=========================
 * Opengl Frame Buffer
 *=======================*/

iso_graphics_frame_buffer* iso_gl_frame_buffer_new(iso_graphics* graphics, iso_graphics_frame_buffer_def def) {
	iso_log_info("Constructing opengl_frame_buffer...\n");

	iso_graphics_frame_buffer* fbo = iso_alloc(sizeof(iso_graphics_frame_buffer));

	iso_str tmp = iso_str_new(def.name);
	iso_assert(iso_str_len(tmp), "Name of frame buffer is not defined.\n");

	// Initializing data
	fbo->id = 0;
	fbo->texture_id = 0;
	fbo->name = iso_str_new(tmp);
	fbo->width = def.width;
	fbo->height = def.height;

	// Setting up opengl frame buffer
	GLCall(glGenFramebuffers(1, &fbo->id));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo->id));

	// Generating texture for frame buffer
	GLCall(glGenTextures(1, &fbo->texture_id));
	GLCall(glBindTexture(GL_TEXTURE_2D, fbo->texture_id));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, def.format, def.width, def.height, 0, def.format, GL_UNSIGNED_BYTE, NULL));
	GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fbo->texture_id, 0));

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	// Saving in graphics memory
	iso_hmap_add(graphics->frame_buffers, fbo->name, fbo);
	iso_log_sucess("Created opengl_frame_buffer: (Name: `%s` ID: %d Texture ID: %d tRes: %dx%d)\n", fbo->name, fbo->id, fbo->texture_id, fbo->width, fbo->height);

	return fbo;
}

void iso_gl_frame_buffer_bind(iso_graphics* graphics, iso_str name) {
	iso_graphics_frame_buffer* fbo;
	iso_hmap_get(graphics->frame_buffers, name, fbo);
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo->id));
	//GLCall(glBindTextureUnit(fbo->texture_id, fbo->texture_id));
	//GLCall(glReadBuffer(GL_COLOR_ATTACHMENT0));
}

void iso_gl_frame_buffer_unbind(iso_graphics* graphics, iso_str name) {
	iso_graphics_frame_buffer* fbo;
	iso_hmap_get(graphics->frame_buffers, name, fbo);
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GLCall(glBindTextureUnit(fbo->texture_id, 0));
}

void iso_gl_frame_buffer_delete(iso_graphics* graphics, iso_str name) {
	iso_log_info("Deleting opengl_frame_buffer: `%s`...\n", name);

	iso_graphics_frame_buffer* fbo;
	iso_hmap_get(graphics->frame_buffers, name, fbo);

	GLCall(glDeleteFramebuffers(1, &fbo->id));
	GLCall(glDeleteTextures(1, &fbo->texture_id));

	iso_hmap_remove(graphics->frame_buffers, name);
	iso_str_delete(fbo->name);
	iso_free(fbo);

	iso_log_sucess("Deleted opengl_frame_buffer: `%s`\n", name);
}


/*=========================
 * Opengl Shader
 *=======================*/

u32 iso_gl_compile_shader(u32 type, iso_str shader_src) {
	iso_log_info("Compiling shader: %s\n", iso_gl_shader_to_str(type));

	u32 id = glCreateShader(type);

	GLCall(glShaderSource(id, 1, &shader_src, NULL));
	GLCall(glCompileShader(id));

	// Checking error in shader
	i32 result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		i32 length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

		iso_str message = iso_alloc(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));

		iso_assert(0, "Failed to compile [%s shader]\n%s\n", (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment"), message);

		iso_free(message);
		GLCall(glDeleteShader(id));
		return 0;
	}

	iso_log_sucess("Compiled shader: %s\n", iso_gl_shader_to_str(type));
	return id;
}

u32 iso_gl_shader_new_from_str(iso_str v_src, iso_str f_src) {
	u32 program = glCreateProgram();

	iso_str tmp_vs = iso_str_new(v_src);
	iso_str tmp_fs = iso_str_new(f_src);

	u32 vs = iso_gl_compile_shader(GL_VERTEX_SHADER, tmp_vs);
	u32 fs = iso_gl_compile_shader(GL_FRAGMENT_SHADER, tmp_fs);

	// Attaching shader
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	iso_str_delete(tmp_vs);
	iso_str_delete(tmp_fs);

	return program;
}

u32 iso_gl_shader_new_from_file(iso_str v_path, iso_str f_path) {
	iso_file* v_file = iso_file_read(v_path);
	iso_file* f_file = iso_file_read(f_path);

	u32 program = iso_gl_shader_new_from_str(v_file->data, f_file->data);

	iso_file_close(v_file);
	iso_file_close(f_file);
	return program;
}

iso_graphics_shader* iso_gl_shader_new(iso_graphics* graphics, iso_graphics_shader_def def) {
	iso_log_info("Constructing opengl_shader...\n");

	iso_graphics_shader* shader = iso_alloc(sizeof(iso_graphics_shader));

	iso_str tmp = iso_str_new(def.name);
	iso_assert(iso_str_len(tmp), "Name of shader is not defined.\n");

	// Initializing data
	shader->id = 0;
	shader->name = iso_str_new(tmp);

	switch (def.source_type) {
		case ISO_GRAPHICS_SHADER_FROM_STR:
			shader->id = iso_gl_shader_new_from_str(def.v_src, def.f_src);
			break;
		case ISO_GRAPHICS_SHADER_FROM_FILE:
			shader->id = iso_gl_shader_new_from_file(def.v_src, def.f_src);
			break;
		default:
			iso_assert(0, "Invalid shader source type: %d\n", def.source_type);
			break;
	}

	// Saving in graphics memory
	iso_hmap_add(graphics->shaders, shader->name, shader);

	iso_log_sucess("Created shader: (Name: `%s` ID: %d)\n", shader->name, shader->id);

	iso_str_delete(tmp);
	return shader;
}

void iso_gl_uniform_set(iso_graphics* graphics, iso_graphics_uniform_def def) {
	i32 u_loc = GLCall(glGetUniformLocation(def.shader->id, def.name));
	iso_assert(u_loc != -1, "`%s` uniform not found in shader `%s`.\n", def.name, def.shader->name);

	switch (def.type) {
		case ISO_GRAPHICS_UNIFORM_INT:
			GLCall(glUniform1i(u_loc, *((int*)def.data)));
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
			GLCall(glUniformMatrix4fv(u_loc, 1, GL_TRUE, &((iso_mat4*) def.data)->m[0][0]));
			break;
		case ISO_GRAPHICS_UNIFORM_SAMPLER2D: {
			iso_graphics_sampler_def* sampler = def.data;
			GLCall(glUniform1iv(u_loc, sampler->count, sampler->samplers));
			break;
		}
		default:
			iso_assert(false, "Unknown ISO_GRAPHICS_UNIFORM_TYPE: %d\n", def.type);
			break;
	}
}

void iso_gl_shader_bind(iso_graphics* graphics, iso_str name) {
	iso_graphics_shader* shader;
	iso_hmap_get(graphics->shaders, name, shader);
	GLCall(glUseProgram(shader->id));
}

void iso_gl_shader_unbind(iso_graphics* graphics) {
	GLCall(glUseProgram(0));
}

void iso_gl_shader_delete(iso_graphics* graphics, iso_str name) {
	iso_log_info("Deleteing opengl_shader: `%s`...\n", name);

	iso_graphics_shader* shader;
	iso_hmap_get(graphics->shaders, name, shader);

	GLCall(glDeleteProgram(shader->id));

	iso_hmap_remove(graphics->shaders, name);
	iso_str_delete(shader->name);
	iso_free(shader);
	
	iso_log_sucess("Deleted opengl_shader: `%s`\n", name);
}


/*=========================
 * Opengl Texture
 *=======================*/

void iso_gl_flip_surface(SDL_Surface* surface)  {
	SDL_LockSurface(surface);
	
	i32 pitch = surface->pitch; // row size
	char temp[pitch];
	iso_str pixels = (iso_str) surface->pixels;
	
	for(i32 i = 0; i < surface->h / 2; ++i) {
		// get pointers to the two rows to swap
		iso_str row1 = pixels + i * pitch;
		iso_str row2 = pixels + (surface->h - i - 1) * pitch;
		
		// swap rows
		memcpy(temp, row1, pitch);
		memcpy(row1, row2, pitch);
		memcpy(row2, temp, pitch);
	}
	SDL_UnlockSurface(surface);
}

u32 iso_gl_get_color_format(SDL_Surface* surface) {
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
	reference: https://discourse.libsdl.org/t/sdl-ttf-2-0-18-surface-to-opengl-texture-not-consistent-with-ttf-2-0-15/34529/5
 */

void iso_gl_pack_surface(SDL_Surface* surface) {
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

iso_graphics_texture* __iso_gl_texture_new_from_data(iso_graphics* graphics, iso_graphics_texture_def def) {
	iso_log_info("Loading texture from data...\n");
	iso_graphics_texture* texture = iso_alloc(sizeof(iso_graphics_texture));

	// Getting the param
	iso_graphics_texture_from_data_param param = def.param.data_param;

	// Initialize data
	iso_str tmp = iso_str_new(def.name);
	iso_assert(iso_str_len(tmp), "Name of texture is not defined.\n");

	// Initializing data
	texture->id = 0;
	texture->width  = param.width;
	texture->height = param.height;
	texture->name   = iso_str_new(tmp);

	// Binding the texture
	GLCall(glGenTextures(1, &texture->id));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture->id));
	
	// Setting up some basic modes to display texture
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, iso_graphics_filter_to_gl_filter(def.filter.min)));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, iso_graphics_filter_to_gl_filter(def.filter.mag)));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	
	// Sending the pixel data to opengl
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, param.format, GL_UNSIGNED_BYTE, param.pixels));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	// Saving in graphics memory
	iso_hmap_add(graphics->textures, texture->name, texture);
	iso_log_sucess("Created opengl_texture: (Name: `%s` ID: %d Res: %dx%d)\n", texture->name, texture->id, texture->width, texture->height);

	iso_str_delete(tmp);

	return texture;
}

iso_graphics_texture* __iso_gl_texture_new_from_file(iso_graphics* graphics, iso_graphics_texture_def def) {
	iso_graphics_texture* texture = iso_alloc(sizeof(iso_graphics_texture));

	// Getting the param
	iso_graphics_texture_from_file_param param = def.param.file_param;

	iso_log_info("Loading texture from file: `%s`...\n", param.file_path);

	// Loading image using sdl_image
	SDL_Surface* surface = iso_sdl_check_ptr(IMG_Load(param.file_path));
	iso_gl_flip_surface(surface);
	iso_gl_pack_surface(surface);
	u32 format = iso_gl_get_color_format(surface);

	// Initialize data
	iso_str tmp = iso_str_new(def.name);
	iso_assert(iso_str_len(tmp), "Name of texture is not defined.\n");

	// Initializing data
	texture->id = 0;
	texture->width  = surface->w;
	texture->height = surface->h;
	texture->name   = iso_str_new(tmp);

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
	iso_log_sucess("Created opengl_texture: (Name: `%s` ID: %d Res: %dx%d)\n", texture->name, texture->id, texture->width, texture->height);

	iso_str_delete(tmp);
	return texture;
}

iso_graphics_texture* iso_gl_texture_new(iso_graphics* graphics, iso_graphics_texture_def def) {
	iso_log_info("Constructing opengl_texture\n");

	switch (def.type) {
		case ISO_GRAPHICS_TEXTURE_FROM_FILE:
			return __iso_gl_texture_new_from_file(graphics, def);
			break;
		case ISO_GRAPHICS_TEXTURE_FROM_DATA:
			return __iso_gl_texture_new_from_data(graphics, def);
			break;
		default:
			iso_assert(false, "Unknown texture type: %d\n", def.type);
			break;
	}
}

void iso_gl_texture_update(iso_graphics* graphics, iso_str name, iso_graphics_texture_update_def def) {
	iso_graphics_texture* tex;
	iso_hmap_get(graphics->textures, name, tex);

	GLCall(glTextureSubImage2D(
		tex->id, def.lod, def.x_offset, def.y_offset,
		def.width, def.height, def.format,
		GL_UNSIGNED_BYTE, def.pixels
	));
}

void iso_gl_texture_bind(iso_graphics* graphics, iso_str name) {
	iso_graphics_texture* texture;
	iso_hmap_get(graphics->textures, name, texture);
	GLCall(glBindTextureUnit(texture->id, texture->id));
}

void iso_gl_texture_unbind(iso_graphics* graphics, iso_str name) {
	iso_graphics_texture* texture;
	iso_hmap_get(graphics->textures, name, texture);
	GLCall(glBindTextureUnit(texture->id, 0));
}

void iso_gl_texture_delete(iso_graphics* graphics, iso_str name) {
	iso_log_info("Deleting opengl_texture: `%s`...\n", name);

	iso_graphics_texture* texture;
	iso_hmap_get(graphics->textures, name, texture);

	GLCall(glDeleteTextures(1, &texture->id));

	iso_hmap_remove(graphics->textures, name);
	iso_str_delete(texture->name);
	iso_free(texture);

	iso_log_sucess("Deleted opengl_texture: `%s`\n", name);
}


/*=========================
 * Opengl Render pipeline
 *=======================*/

iso_graphics_render_pipeline* iso_gl_render_pipeline_new(iso_graphics* graphics, iso_graphics_render_pipeline_def def) {
	iso_log_info("Constructing render pipeline...\n");

	iso_graphics_render_pipeline* pip = iso_alloc(sizeof(iso_graphics_render_pipeline));

	iso_str tmp = iso_str_new(def.name);
	iso_assert(iso_str_len(tmp), "Name of render pipeline is not defined.\n");

	// Initializing data
	pip->id = 0;
	pip->name = iso_str_new(tmp);

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

	// Binding the buffers
	graphics->api.vertex_buffer_bind(graphics, pip->buffers.vbo->name);
	graphics->api.index_buffer_bind(graphics, pip->buffers.ibo->name);
	graphics->api.shader_bind(graphics, pip->buffers.shader->name);

	// Calculating stride
	size_t stride = 0;
	for (i32 i = 0; i < def.amt; i++) {
		iso_graphics_vertex_layout_def layout = def.layout[i];
		stride += layout.amt * iso_graphics_get_type_size(layout.type);
	}

	iso_log_info("Setting up vertex attribute layouts...\n");

	// Generating layout
	// TODO: Improve this because "glVertexAttribPointer" doesnt support integers as types use "glVertexAttribIPointer"
	size_t offset = 0;
	for (i32 i = 0; i < def.amt; i++) {
		iso_graphics_vertex_layout_def layout = def.layout[i];

		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, layout.amt, iso_graphics_type_to_gl_type(layout.type), GL_FALSE, stride, (const void*) offset));

		// Calculating offsets
		offset += layout.amt * iso_graphics_get_type_size(layout.type);
	}

	// Unbinding all
	GLCall(glBindVertexArray(0));
	graphics->api.vertex_buffer_unbind(graphics);
	graphics->api.index_buffer_unbind(graphics);
	graphics->api.shader_unbind(graphics);

	// Saving in graphics memory
	iso_hmap_add(graphics->render_pipelines, pip->name, pip);

	iso_log_sucess("Created opengl_render_pipeline: (Name:`%s` ID: %d Layouts: %d)\n", pip->name, pip->id, def.amt);

	iso_str_delete(tmp);
	return pip;
}

void iso_gl_render_pipeline_begin(iso_graphics* graphics, iso_str name) {
	iso_graphics_render_pipeline* pip;
	iso_hmap_get(graphics->render_pipelines, name, pip);

	// Binding all buffers
	graphics->api.vertex_buffer_bind(graphics, pip->buffers.vbo->name);
	graphics->api.index_buffer_bind(graphics, pip->buffers.ibo->name);
	graphics->api.shader_bind(graphics, pip->buffers.shader->name);
}

void iso_gl_render_pipeline_end(iso_graphics* graphics, iso_str name, i32 indices_cnt) {
	iso_graphics_render_pipeline* pip;
	iso_hmap_get(graphics->render_pipelines, name, pip);

	// Draw call
	GLCall(glBindVertexArray(pip->id));
	GLCall(glDrawElements(GL_TRIANGLES, indices_cnt, GL_UNSIGNED_INT, NULL));
}

void iso_gl_render_pipeline_delete(iso_graphics* graphics, iso_str name) {
	iso_log_info("Deleting opengl_render_pipeline: `%s`...\n", name);

	iso_graphics_render_pipeline* pip;
	iso_hmap_get(graphics->render_pipelines, name, pip);

	GLCall(glDeleteVertexArrays(1, &pip->id));
	
	iso_hmap_remove(graphics->render_pipelines, name);
	iso_str_delete(pip->name);
	iso_free(pip);

	iso_log_sucess("Deleted opengl_render_pipeline: `%s`\n", name);
}

