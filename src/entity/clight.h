#pragma once

#include "rend/light.h"
struct Entity;

struct CLight {
	Light light;

	void update_and_apply_position(Entity* self, Shader* shd);
};