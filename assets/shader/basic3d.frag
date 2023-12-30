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

struct PointLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	bool is_active;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	bool is_active;

	vec3  direction;
    float cutOff;
	float outerCutOff;
};

in vec2 f_uv;
in vec3 f_norm;
in vec3 fragPos;

layout(location = 0) out vec4 col;

uniform Material material0;
uniform vec3 viewPos;

uniform PointLight pointLights[4];
uniform int pointLights_count;
uniform SpotLight spotLights[2];
uniform int spotLights_count;

uniform int u_noise_seed;
uniform float u_noise_intensity;
uniform float u_noise_seizure; // 200 is ok, 20 is minimum.

uniform vec3 u_tint;

uniform bool u_unlit;

float noise(vec2 p, int seed) {
	float t = seed + 123.;
	float ta = t*.654321;
	float tb = t*(ta*.123456);
	return fract(sin(p.x*ta+p.y*tb)*5678.);
}

float when_gt(float x, float y) {
	return max(sign(x - y), 0.0);
}

vec3 calc_point_light(PointLight light) {
	// Texture maps.
	vec4 tex_diffuse = texture(material0.diffuse_map, f_uv) * vec4(u_tint, 1);;
	vec4 tex_specular = texture(material0.specular_map, f_uv);

	// Ambient.
	vec3 ambient = light.ambient * material0.ambient * tex_diffuse.rgb;

	// Diffuse.
	vec3 norm = normalize(f_norm);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * (light.diffuse * material0.diffuse) * tex_diffuse.rgb;

	// Specular.
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32) * when_gt(material0.shininess, 0);
	vec3 specular = spec * (light.specular * material0.specular) * tex_specular.rgb;

	// Attenuation.
	float lightd = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * lightd + light.quadratic * (lightd * lightd));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 calc_spot_light(SpotLight light) {
	// Texture maps.
	vec4 tex_diffuse = texture(material0.diffuse_map, f_uv) * vec4(u_tint, 1);;
	vec4 tex_specular = texture(material0.specular_map, f_uv);

	// Ambient.
	vec3 ambient = light.ambient * material0.ambient * tex_diffuse.rgb;

	// Diffuse.
	vec3 norm = normalize(f_norm);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * (light.diffuse * material0.diffuse) * tex_diffuse.rgb;

	// Specular.
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32) * when_gt(material0.shininess, 0);
	vec3 specular = spec * (light.specular * material0.specular) * tex_specular.rgb;

	// Spotlight.
	float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;

	return (ambient + diffuse + specular);
}

void main() {
	vec3 final_col;
	vec4 tex_diffuse = texture(material0.diffuse_map, f_uv) * vec4(u_tint, 1);
	vec4 tex_emission = texture(material0.emission_map, f_uv);

	if (u_unlit) {
		col = tex_diffuse + tex_emission;
	} else {
		for (int i = 0; i < pointLights_count; i++) {
			if (!pointLights[i].is_active) {
				continue;
			}
			vec3 c = calc_point_light(pointLights[i]);
			final_col += c;
		}

		for (int i = 0; i < spotLights_count; i++) {
			if (!spotLights[i].is_active) {
				continue;
			}
			vec3 c = calc_spot_light(spotLights[i]);
			final_col += c;
		}
	
	
		
		tex_emission *= tex_emission.a;
		col = vec4(final_col, tex_diffuse.a) + tex_emission;

		// Noise
		float r = noise(gl_FragCoord.xy / u_noise_seizure, u_noise_seed);
		col += (vec4(r, r, r, 0) * u_noise_intensity);
	}
}