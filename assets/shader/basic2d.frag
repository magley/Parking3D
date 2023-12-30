#version 330 core

layout(location = 0) out vec4 fragColor;

in vec2 f_uv;
uniform sampler2D tex;
uniform vec4 u_color;

void main() {
	fragColor = texture(tex, f_uv) * u_color;
}