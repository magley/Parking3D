#version 330 core

in vec2 f_uv;
out vec4 col;
uniform sampler2D texture0;

void main() {
	col = texture(texture0, f_uv);
}