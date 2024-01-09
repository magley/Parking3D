#pragma once
#include "rend/color.h"
#include <vector>
#include <glm/vec3.hpp>

struct Entity;

struct Path {
	std::vector<glm::vec3> p;
	int current = 0;
};

struct CCar {
	enum State {
		TOWARD_RAMP,
		AT_RAMP,
		TOWARD_SPOT,
		AT_SPOT,
	};

	Color color;
	int spot_index = -1;
	int time_left = 0;
	State state = State::TOWARD_RAMP;
	Path path;

	void init(int spot);
	void update(Entity* self);
private:
	Entity* waiting_for_car = nullptr; 

	void _follow_path(Entity* self);
	void _update_toward_ramp(Entity* self);
	void _update_at_ramp(Entity* self);
	void _update_toward_spot(Entity* self);
	void _update_at_spot(Entity* self);
	void _set_path(Entity* self);
};