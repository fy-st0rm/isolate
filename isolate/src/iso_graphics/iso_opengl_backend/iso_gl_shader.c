#include "iso_gl_shader.h"
#include "iso_gl_util.h"
#include "iso_util/iso_file.h"

iso_shader* iso_gl_shader_new(iso_graphics* graphics, iso_shader_def def) {
	iso_log_info("Constructing opengl_shader...\n");

	iso_shader* shader = iso_alloc(sizeof(iso_shader));

	// Checking the name
	iso_str tmp = iso_str_new(def.name);
	iso_assert(iso_str_len(tmp), "Name of shader is not defined.\n");

	// Initializing data
	shader->id = 0;
	shader->name = iso_str_new(tmp);

	// Loading shader according to load type
	switch (def.load_type) {
		case ISO_SHADER_FROM_STR:
			shader->id = iso_gl_shader_new_from_str(def.v_src, def.f_src);
			break;
		case ISO_SHADER_FROM_FILE:
			shader->id = iso_gl_shader_new_from_file(def.v_src, def.f_src);
			break;
		default:
			iso_assert(0, "Invalid shader load type: %d\n", def.load_type);
			break;
	}

	// Saving in graphics memory
	iso_hmap_add(graphics->memory, shader->name, shader);

	iso_log_sucess("Created shader: (Name: `%s` ID: %d)\n", shader->name, shader->id);
	iso_str_delete(tmp);
	return shader;
}

void iso_gl_shader_delete(iso_graphics* graphics, iso_shader* shader) {
	iso_assert(shader, "Tried deleting uninitiallized iso_shader\n");
	iso_log_info("Deleteing opengl_shader: `%s`...\n", shader->name);

	GLCall(glDeleteProgram(shader->id));

	iso_log_sucess("Deleted opengl_shader: `%s`\n", shader->name);

	// Removing from engine memory
	iso_hmap_remove(graphics->memory, shader->name);
	iso_str_delete(shader->name);
	iso_free(shader);
}

void iso_gl_shader_uniform_set(iso_graphics* graphics, iso_shader* shader, iso_uniform_def def) {
	iso_assert(shader, "Tried setting uniform from uninitiallized iso_shader\n");

	i32 u_loc = GLCall(glGetUniformLocation(shader->id, def.name));
	iso_assert(u_loc != -1, "`%s` uniform not found in shader `%s`.\n", def.name, shader->name);

	switch (def.type) {
		case ISO_UNIFORM_INT:
			GLCall(glUniform1i(u_loc, *((int*)def.data)));
			break;
		case ISO_UNIFORM_FLOAT:
			GLCall(glUniform1f(u_loc, *((float*) def.data)));
			break;
		case ISO_UNIFORM_VEC2:
			GLCall(glUniform2fv(u_loc, 1, (iso_vec2*) def.data));
			break;
		case ISO_UNIFORM_VEC3:
			GLCall(glUniform3fv(u_loc, 1, (iso_vec3*) def.data));
			break;
		case ISO_UNIFORM_VEC4:
			GLCall(glUniform4fv(u_loc, 1, (iso_vec4*) def.data));
			break;
		case ISO_UNIFORM_MAT4:
			GLCall(glUniformMatrix4fv(u_loc, 1, GL_TRUE, &((iso_mat4*) def.data)->m[0][0]));
			break;
		case ISO_UNIFORM_SAMPLER2D: {
			iso_uniform_sampler_def* sampler = def.data;
			GLCall(glUniform1iv(u_loc, sampler->count, sampler->samplers));
			break;
		}
		default:
			iso_assert(false, "Unknown ISO_GRAPHICS_UNIFORM_TYPE: %d\n", def.type);
			break;
	}
}

void iso_gl_shader_bind(iso_graphics* graphics, iso_shader* shader) {
	iso_assert(shader, "Tried binding from uninitiallized iso_shader\n");
	GLCall(glUseProgram(shader->id));
}

void iso_gl_shader_unbind(iso_graphics *graphics) {
	GLCall(glUseProgram(0));
}

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

