#version 440 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 tex_cord;

out vec4 o_color;
out vec2 o_tex_cord;

uniform mat4 mvp;

void main() {
	o_color = color;
	o_tex_cord = tex_cord;
	gl_Position = mvp * vec4(position, 1.0f);
}
