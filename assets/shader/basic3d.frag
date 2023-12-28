#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;

	sampler2D diffuse_map;
	sampler2D specular_map;
	sampler2D emission_map;
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

//uniform sampler2D texture0;
uniform Material material0;
uniform Light light0;

uniform vec3 viewPos;

float when_gt(float x, float y) {
	return max(sign(x - y), 0.0);
}

void main() {
	vec4 tex_diffuse = texture(material0.diffuse_map, f_uv);
	vec4 tex_specular = texture(material0.specular_map, f_uv);
	vec4 tex_emission = texture(material0.emission_map, f_uv);


	// Ambient.
	vec3 ambient = light0.ambient * material0.ambient * tex_diffuse.rgb;

	// Diffuse.
	vec3 norm = normalize(f_norm);
	vec3 lightDir = normalize(light0.position - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * (light0.diffuse * material0.diffuse) * tex_diffuse.rgb;

	// Specular.
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32) * when_gt(material0.shininess, 0);
	vec3 specular = spec * (light0.specular * material0.specular) * tex_specular.rgb;

	// Final.
	vec3 finalCol = (ambient + diffuse + specular);
	col = vec4(finalCol, 1.0) + tex_emission;
}