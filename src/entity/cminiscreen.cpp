#include "cminiscreen.h"
#include "global.h"
#include <glm/gtc/matrix_transform.hpp>

void CMiniScreen::update(Entity* self) {
	const float angle_raised = 0;
	const float float_angle_lowered = 150;
	const float dang = 10;

	switch (state) {
	case GO_UP:
		if (angle > angle_raised) {
			angle -= dang;
		}
		if (angle <= angle_raised) {
			angle = angle_raised;
			state = RAISED;
			glo::game.set_cam(1);

			for (Entity* e : glo::wctx.entity.arr) {
				if (e->has(Component::CAM)) {
					if (e->cam.type == CCam::FREE_STATIONARY) {
						e->ang.y = -20;
					}
				}
			}
		}
		break;
	case GO_DOWN:
		if (angle < float_angle_lowered) {
			angle += dang;
		}
		if (angle >= float_angle_lowered) {
			angle = float_angle_lowered;
			state = LOWERED;
		}
		break;
	case LOWERED:
		break;
	case RAISED:
		break;
	}

	if (glo::game._cam_index == 5) {
		self->pos = (float)0.4 * glo::wctx.cam.front;
		self->pos += -0.7f * glo::wctx.cam.up;
		self->pos += glo::wctx.cam.pos;

		self->ang.z = -angle;
		self->ang.y = -glo::wctx.cam.yaw;
	}
}

void CMiniScreen::raise(Entity* self) {
	if (state == LOWERED) {
		state = GO_UP;
		glo::wctx.audio.play(glo::wctx.resmng.load_wav("cam_open.wav"));
	}
}

void CMiniScreen::lower(Entity* self) {
	if (state == RAISED) {
		state = GO_DOWN;
		glo::game.set_cam(5);
		glo::wctx.audio.play(glo::wctx.resmng.load_wav("cam_open.wav"));
	}
}