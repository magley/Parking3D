#pragma once
#include "glm/glm.hpp"
#include "cmodel.h"
#include "cramp.h"

enum Component {
	NONE  = 0,
	MODEL = 1,
	RAMP  = 2,
	CAR   = 4,
};

struct Entity {
	glm::vec3 pos;
	glm::vec3 ang;

	unsigned c = Component::NONE;

	void add(Component component) { c |= component; }
	bool has(Component component) { return (c & component) == component;  }

	CModel cmodel;
	CRamp cramp;
};