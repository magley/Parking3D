#pragma once
#include <glm/glm.hpp>
#include <string>
#include "shader.h"

struct PointLight_Fields {
	float constant;
	float linear;
	float quadratic;

	PointLight_Fields();
	PointLight_Fields(float constant, float linear, float quadratic);
};

struct SpotLight_Fields {
	glm::vec3 direction;
	float cutOff;
	float outerCutOff;

	SpotLight_Fields();
	SpotLight_Fields(glm::vec3 direction, float cutOff, float outerCutOff);
};

struct Light {
	enum Type {
		POINT_LIGHT,
		SPOT_LIGHT
	};

	Type type;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	std::string uniform_name;

	Light(Type type, std::string uniform_name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	Light();

	union {
		PointLight_Fields pointlight;
		SpotLight_Fields spotlight;
	};

	void apply_colors(Shader* shd) const;
	void apply_type_fields(Shader* shd) const;
	void apply_position(Shader* shd, const glm::vec3& pos) const;
	void apply_active(Shader* shd, bool is_active) const;
private:
	void _apply_colors(Shader* shd, const std::string& uniform_name) const;
	void _apply_type_fields(Shader* shd, const std::string& uniform_name) const;
	void _apply_position(Shader* shd, const std::string& uniform_name, const glm::vec3& pos) const;
	void _apply_active(Shader* shd, const std::string& uniform_name, bool is_active) const;
};