#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 tangent;

uniform mat4 M;
uniform mat4 M_inv;
uniform mat4 VP;

out vec2 f_uv;
out vec3 f_norm;
out vec3 fragPos;
out mat3 f_TBN;

void main() {
	gl_Position = VP * M * vec4(position, 1.0);
	f_uv = uv;
	f_norm = mat3(transpose(M_inv)) * normal;
	fragPos = vec3(M * vec4(position, 1.0));

	vec3 bitangent = cross(tangent, normal);
	vec3 T = normalize(vec3(M * vec4(tangent,   0.0)));
	vec3 N = normalize(vec3(M * vec4(normal,    0.0)));
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T); // = normalize(vec3(M * vec4(bitangent, 0.0)));
	f_TBN = mat3(T, B, N);
}