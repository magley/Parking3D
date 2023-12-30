#pragma once
#include <glm/glm.hpp>

struct Entity;

struct ParkingSpot {
	glm::vec3 pos;
	Entity* used_by = nullptr;
	int time_left = 0;
};