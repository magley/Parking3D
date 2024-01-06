#pragma once
#include <glm/glm.hpp>

struct Camera {
	glm::vec3 pos    = glm::vec3(-0.5,  0.0,  4.0);
	glm::vec3 up     = glm::vec3( 0.0,  1.0,  0.0);
	glm::vec3 front  = glm::vec3( 0.0,  0.0, -1.0);

	float yaw = 0;
	float pitch = 0;

	bool proj_is_perspective = true;
	glm::mat4 proj = glm::mat4(1.0f);

	void update_proj();
	glm::mat4 view() const;
};