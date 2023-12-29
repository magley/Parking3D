#pragma once

#include "rend/light.h"
struct Entity;

struct CLight {
	Light light;
	bool _active = true;
	Shader* shd = nullptr;

	void update_and_apply_position(Entity* self);
	void set_active(bool is_active);
};