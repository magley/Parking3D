#pragma once
#include <glm/glm.hpp>

struct Camera {
	glm::vec3 pos    = glm::vec3(-0.5,  0.0,  2.0);
	glm::vec3 up     = glm::vec3( 0.0,  1.0,  0.0);
	glm::vec3 front  = glm::vec3( 0.0,  0.0, -1.0);

	glm::mat4 proj;

	Camera();
	glm::mat4 view() const;
};