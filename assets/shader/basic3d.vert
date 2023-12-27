#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 M;
uniform mat4 M_inv;
uniform mat4 VP;

out vec2 f_uv;
out vec3 f_norm;
out vec3 fragPos;

void main() {
	gl_Position = VP * M * vec4(position, 1.0);
	f_uv = uv;
	f_norm = mat3(transpose(M_inv)) * normal;
	fragPos = vec3(M * vec4(position, 1.0));
}