#ifndef __ISO_SHADER_H__
#define __ISO_SHADER_H__

#include "iso_util/iso_defines.h"
#include "iso_util/iso_str.h"


/*
 * @brief Enum for the creation of shader.
 */

typedef enum {
	ISO_SHADER_FROM_STR,
	ISO_SHADER_FROM_FILE
} iso_shader_load_type;


/*
 * @brief Enum for all the supported uniform type
 */

typedef enum {
	ISO_UNIFORM_INT,
	ISO_UNIFORM_FLOAT,
	ISO_UNIFORM_VEC2,
	ISO_UNIFORM_VEC3,
	ISO_UNIFORM_VEC4,
	ISO_UNIFORM_MAT4,
	ISO_UNIFORM_SAMPLER2D
} iso_uniform_type;


/*
 * @brief Shader defination structure
 * @mem name        = Name of the shader
 * @mem v_src       = Vertex shader string / Vertex shader file location
 * @mem f_src       = Fragment shader string / Fragment shader file location
 * @mem load_type   = Shader creation process type
 */

typedef struct {
	iso_str name;
	iso_str v_src;
	iso_str f_src;
	iso_shader_load_type load_type;
} iso_shader_def;


/*
 * @brief Shader structure
 * @mem id   = Shader id
 * @mem name = Name of the shader
 */

typedef struct {
	u32 id;
	iso_str name;
} iso_shader;


/*
 * @brief Struct that defines the shader sampler.
 * @mem count    = Total number of samplers
 * @mem samplers = Array of samplers
 */

typedef struct {
	i32 count;
	i32* samplers;
} iso_uniform_sampler_def;


/*
 * @brief Struct to define uniform
 * @mem name   = Name of the uniform in shader
 * @mem data   = Pointer to the uniform data
 * @mem type   = Type of uniform
 */

typedef struct {
	iso_str name;
	void* data;
	iso_uniform_type type;
} iso_uniform_def;


/*
 * @brief Function to create new shader
 * @param  def = iso_shader_def struct
 * @return Returns pointer to iso_shader
 */

ISO_API iso_shader* iso_shader_new(iso_shader_def def);


/*
 * @brief Function to delete shader
 * @param shader = Pointer to iso_shader
 */

ISO_API void iso_shader_delete(iso_shader* shader);


/*
 * @brief Function to set the uniform in shader
 * @param shader = Pointer to iso_shader
 * @param def    = iso_uniform_def struct
 */

ISO_API void iso_shader_uniform_set(iso_shader* shader, iso_uniform_def def);


/*
 * @brief Function to bind the shader
 * @param shader = Pointer to the iso_shader
 */

ISO_API void iso_shader_bind(iso_shader* shader);


/*
 * @brief Function to unbind the shader 
 */

ISO_API void iso_shader_unbind();

#endif //__ISO_SHADER_H__
