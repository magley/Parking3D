#include "ccar.h"
#include "global.h"

Entity* _get_ramp() {
	Entity* ramp = nullptr;
	for (Entity* e : glo::wctx.entity.arr) {
		if (e->has(Component::RAMP)) {
			ramp = e;
			break;
		}
	}

	return ramp;
}

void CCar::init(int spot) {
	spot_index = spot;
	state = State::TOWARD_RAMP;

	Color_HSV crand = Color_HSV(
		rand() % 360,
		0.8,
		1.0
	);
	color = crand.to_color();
}

void CCar::update(Entity* self) {
	switch (state) {
	case TOWARD_RAMP:
		_update_toward_ramp(self);
		break;
	case AT_RAMP:
		_update_at_ramp(self);
		break;
	case TOWARD_SPOT:
		_update_toward_spot(self);
		break;
	case AT_SPOT:
		_update_at_spot(self);
		break;
	}
}

void CCar::_update_toward_ramp(Entity* self) {
	Entity* ramp = _get_ramp();
	if (ramp == nullptr) {
		return;
	}

	float spd = 0.2f;
	glm::vec3 ramppos = ramp->pos - glm::vec3(3, 0, 4);
	if (self->pos.x < ramppos.x) self->pos.x += spd;
	if (self->pos.x > ramppos.x) self->pos.x -= spd;
	if (self->pos.y < ramppos.y) self->pos.y += spd;
	if (self->pos.y > ramppos.y) self->pos.y -= spd;
	if (self->pos.z < ramppos.z) self->pos.z += spd;
	if (self->pos.z > ramppos.z) self->pos.z -= spd;

	if (glm::length(self->pos - ramppos) <= spd * 2) {
		state = State::AT_RAMP;
	}
}

void CCar::_update_at_ramp(Entity* self) {
	Entity* ramp = _get_ramp();
	if (ramp == nullptr) {
		return;
	}

	if (ramp->ramp.all_the_way_up) {
		state = State::TOWARD_SPOT;
	}
}

void CCar::_update_toward_spot(Entity* self) {
	const ParkingSpot spot = glo::game.parking_spots[spot_index];

	float spd = 0.2f;
	if (self->pos.x < spot.pos.x) self->pos.x += spd;
	if (self->pos.x > spot.pos.x) self->pos.x -= spd;
	if (self->pos.y < spot.pos.y) self->pos.y += spd;
	if (self->pos.y > spot.pos.y) self->pos.y -= spd;
	if (self->pos.z < spot.pos.z) self->pos.z += spd;
	if (self->pos.z > spot.pos.z) self->pos.z -= spd;

	if (glm::length(self->pos - spot.pos) <= spd) {
		state = State::AT_SPOT;
		time_left = 20 * 60;
	}
}

void CCar::_update_at_spot(Entity* self) {
	time_left--;
	if (time_left == 0) {
		glo::wctx.entity.destroy(self);
	}
}