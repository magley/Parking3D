#include "cramp.h"
#include "entity.h"

void CRamp::update(Entity* self) {
	const float RISE_SPEED = 1.0f;
	const float DROP_SPEED = 1.0f;
	const int RISE_WAIT_TICKS = 2 * 60;
	const float ANG_MAX = 90.0f;
	const float ANG_MIN = 0.0f;

	float& rot = self->ang.x;

	switch (state) {
	case REST:
		timer--;
		if (timer <= 0) {
			state = RISE;
		}
		break;
	case RISE:
		rot += RISE_SPEED;
		if (rot >= ANG_MAX) {
			rot = ANG_MAX;
			state = RISE_WAIT;
			timer = RISE_WAIT_TICKS;
		}
		break;
	case RISE_WAIT:
		timer--;
		if (timer <= 0) {
			state = DROP;
		}
		break;
	case DROP:
		rot -= DROP_SPEED;
		if (rot <= ANG_MIN) {
			rot = ANG_MIN;
			state = REST;
			timer = RISE_WAIT_TICKS;
		}
		break;
	}
}