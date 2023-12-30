#pragma once
#include "rend/color.h"

struct Entity;

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

	void init(int spot);
	void update(Entity* self);
private:
	void _update_toward_ramp(Entity* self);
	void _update_at_ramp(Entity* self);
	void _update_toward_spot(Entity* self);
	void _update_at_spot(Entity* self);
};