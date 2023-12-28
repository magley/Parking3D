#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 f_uv;
in vec3 f_norm;
in vec3 fragPos;

out vec4 col;

uniform sampler2D texture0;
uniform Material material0;
uniform Light light0;

uniform vec3 viewPos;

float when_gt(float x, float y) {
	return max(sign(x - y), 0.0);
}

void main() {
	vec4 tex_color = texture(texture0, f_uv);

	// Ambient.
	vec3 ambient = light0.ambient * material0.ambient;

	// Diffuse.
	vec3 norm = normalize(f_norm);
	vec3 lightDir = normalize(light0.position - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * (light0.diffuse * material0.diffuse);

	// Specular.
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(viewDir, halfwayDir), 0.0), 32) * when_gt(material0.shininess, 0);
	vec3 specular = spec * (light0.specular * material0.specular);

	// Final.
	vec3 finalCol = (ambient + diffuse + specular) * tex_color.rgb;
	col = vec4(finalCol, tex_color.a);
}