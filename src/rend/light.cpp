#include "light.h"

//--------------------------------------------------------------------------------------
// Public API.
//--------------------------------------------------------------------------------------

PointLight_Fields::PointLight_Fields() {}
PointLight_Fields::PointLight_Fields(float constant, float linear, float quadratic) :
	constant(constant),
	linear(linear),
	quadratic(quadratic) {}

SpotLight_Fields::SpotLight_Fields() {}
SpotLight_Fields::SpotLight_Fields(glm::vec3 direction, float cutOff, float outerCutOff) :
	direction(direction),
	cutOff(cutOff),
	outerCutOff(outerCutOff) {}

Light::Light() {}

Light::Light(Type type, std::string uniform_name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : 
	type(type),
	uniform_name(uniform_name),
	ambient(ambient),
	diffuse(diffuse),
	specular(specular) {}

void Light::apply_colors(Shader* shd) const {
	_apply_colors(shd, uniform_name);
}

void Light::apply_type_fields(Shader* shd) const {
	_apply_type_fields(shd, uniform_name);
}

void Light::apply_position(Shader* shd, const glm::vec3& pos) const {
	_apply_position(shd, uniform_name, pos);
}

void Light::apply_active(Shader* shd, bool is_active) const {
	_apply_active(shd, uniform_name, is_active);
}

//--------------------------------------------------------------------------------------
// Private funcs.
//--------------------------------------------------------------------------------------

void Light::_apply_colors(Shader* shd, const std::string& uniform_name) const {
	shd->set_vec3(uniform_name + ".ambient", ambient.x, ambient.y, ambient.z);
	shd->set_vec3(uniform_name + ".diffuse", diffuse.x, diffuse.y, diffuse.z);
	shd->set_vec3(uniform_name + ".specular", specular.x, specular.y, specular.z);
}

void Light::_apply_type_fields(Shader* shd, const std::string& uniform_name) const {
	switch (type) {
	case POINT_LIGHT:
		shd->set_float(uniform_name + ".constant", pointlight.constant);
		shd->set_float(uniform_name + ".linear", pointlight.linear);
		shd->set_float(uniform_name + ".quadratic", pointlight.quadratic);
		break;
	case SPOT_LIGHT:
		shd->set_float(uniform_name + ".cutOff", glm::cos(glm::radians(spotlight.cutOff)));
		shd->set_float(uniform_name + ".outerCutOff", glm::cos(glm::radians(spotlight.outerCutOff)));
		shd->set_vec3(uniform_name + ".direction", spotlight.direction.x, spotlight.direction.y, spotlight.direction.z);
		break;
	default:
		break;
	}
}

void Light::_apply_position(Shader* shd, const std::string& uniform_name, const glm::vec3& pos) const {
	shd->set_vec3(uniform_name + ".position", pos.x, pos.y, pos.z);
}

void Light::_apply_active(Shader* shd, const std::string& uniform_name, bool is_active) const {
	shd->set_int(uniform_name + ".is_active", is_active);
}