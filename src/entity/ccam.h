#pragma once

#include "glm/glm.hpp"

struct Entity;

struct CCam {
	enum Type {
		CCTV_SCANNER,
		CCTV_TOPDOWN,
		FREE
	};
	
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 front = glm::vec3(0.0, 0.0, -1.0);

	float yaw = 0;
	float pitch = 0;
	Type type = Type::CCTV_SCANNER;
	bool active = false;

	CCam() {}
	CCam(Type type, bool active) : type(type), active(active) {}

	void update(Entity* self);
};