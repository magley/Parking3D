#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

uniform mat4 u_proj;
uniform vec2 u_pos;
uniform vec2 u_scale;
uniform vec3 u_img_frame;

out vec2 f_uv;

void main()
{
	gl_Position = u_proj * vec4(pos * u_scale + u_pos, 0.0, 1.0);

	float frame_u = u_img_frame.x;
	float frame_v = u_img_frame.y;
	float frame = u_img_frame.z;

	f_uv.x = (uv.x + frame) * frame_u;
	f_uv.y = (uv.y) * frame_v;
}