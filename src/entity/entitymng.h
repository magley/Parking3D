#pragma once
#include "entity.h"
#include <vector>

struct EntityMng {
	std::vector<Entity*> arr;

	Entity* add(glm::vec3 pos);
	void clear();
	size_t size() const;
};