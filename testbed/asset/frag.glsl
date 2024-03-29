#version 440 core

layout (location = 0) out vec4 color;

in vec3 o_normal;
in vec2 o_tex_cord;

uniform sampler2D tex;

void main() {
	color = texture(tex, o_tex_cord) * vec4(1, 1, 1, 1);
}
