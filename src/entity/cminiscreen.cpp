#include "cminiscreen.h"

#include <glm/gtc/matrix_transform.hpp>
#include "subsystem/subsystem_audio.h"
#include "subsystem/subsystem_camera.h"
#include "subsystem/subsystem_entity.h"
#include "subsystem/subsystem_game.h"
#include "subsystem/subsystem_resource.h"

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
			glo::game->set_cam(1);

			for (Entity* e : glo::entity->arr) {
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

	if (glo::game->curr_cam_index == glo::game->down_cam_index) {
		self->pos = (float)0.4 * glo::cam->front;
		self->pos += -0.7f * glo::cam->up;
		self->pos += glo::cam->pos;

		self->ang.z = -angle;
		self->ang.y = -glo::cam->yaw;
	}
}

void CMiniScreen::raise(Entity* self) {
	if (state == LOWERED) {
		state = GO_UP;
		glo::audio->play(glo::res->load_wav("cam_open.wav"));
	}
}

void CMiniScreen::lower(Entity* self) {
	if (state == RAISED) {
		state = GO_DOWN;
		glo::game->set_cam(5);
		glo::audio->play(glo::res->load_wav("cam_open.wav"));
	}
}