#pragma once
#include "glm/glm.hpp"
#include "cmodel.h"
#include "cramp.h"
#include "ccam.h"
#include "clight.h"

enum Component {
	NONE  = 0,
	MODEL = 1,
	RAMP  = 2,
	CAM   = 4,
	LIGHT = 8,
};

struct Entity {
	glm::vec3 pos;
	glm::vec3 ang;

	unsigned c = Component::NONE;

	void add(Component component) { c |= component; }
	bool has(Component component) { return (c & component) == component;  }

	CModel model;
	CRamp ramp;
	CCam cam;
	CLight light;
};