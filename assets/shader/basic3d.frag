#version 330 core

in vec2 f_uv;
in vec3 f_norm;
in vec3 fragPos;

out vec4 col;
uniform sampler2D texture0;

uniform vec3 ambientCol;
uniform vec3 lightPos;
uniform vec3 lightCol;
uniform vec3 viewPos;

float when_gt(float x, float y) {
	return max(sign(x - y), 0.0);
}

void main() {
	vec4 tex_color = texture(texture0, f_uv);

	// Ambient.
	float ambientStrength = 0.5;
	vec3 ambient = ambientStrength * ambientCol;

	// Diffuse.
	float diffuseStrength = 0.5;
	vec3 norm = normalize(f_norm);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0) * diffuseStrength;
	vec3 diffuse = diff * lightCol;

	// Specular.
	float specularStrength = 0.5 * diffuseStrength;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(viewDir, halfwayDir), 0.0), 32) * when_gt(diff, 0);
	vec3 specular = specularStrength * spec * lightCol;

	// Final.
	vec3 finalCol = (ambient + diffuse + specular) * tex_color.rgb;
	col = vec4(finalCol, tex_color.a);
}