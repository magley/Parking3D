#include "cramp.h"
#include "entity.h"
#include "global.h"

void CRamp::update(Entity* self) {
	const float RISE_SPEED = 90 / 60.0f;
	const float DROP_SPEED = 90 / 60.0f;
	const float ANG_MAX = 0.0f;
	const float ANG_MIN = -90.0f;

	float& rot = self->ang.z;

	switch (state) {
	case RISE:
		rot += RISE_SPEED;
		if (rot >= ANG_MAX) {
			rot = ANG_MAX;
		}
		break;
	case DROP:
		rot -= DROP_SPEED;
		if (rot <= ANG_MIN) {
			rot = ANG_MIN;
		}
		break;
	}
}

void CRamp::toggle() {
	if (state == RISE) {
		state = DROP;
	}
	else {
		state = RISE;
	}

	glo::wctx.audio.play(glo::wctx.resmng.load_wav("ramp.wav"));
}