#include "ccam.h"
#include "global.h"
#include "rend/shader.h"

void CCam::update(Entity* self) {
	switch (type) {
		case CCTV_SCANNER: {
			const int SWAYMAX = 40;
			const float SWAYSPD = 0.25f;
			const int SWAYRESTTIMER = 90;

			if (sway < +SWAYMAX && sway_dir > 0) {
				sway += SWAYSPD;
			}
			if (sway > -SWAYMAX && sway_dir < 0) {
				sway -= SWAYSPD;
			}
			if (fabsf(sway - SWAYMAX * sway_dir) <= SWAYSPD && timer == 0) {
				sway = SWAYMAX * sway_dir;
				timer = SWAYRESTTIMER;
			}
			if (timer > 0) {
				timer--;

				if (timer == 0) {
					sway_dir *= -1;
				}
			}

			yaw = self->ang.x + sway;
			pitch = self->ang.y;
			update_front_from_pitch_yaw(self);
			break; 
		}
		case CCTV_TOPDOWN: {
			update_pitch_yaw_from_ang(self);
			update_front_from_pitch_yaw(self);
			break;
		}
		case FREE: {
			Input& input = glo::wctx.input;
			int input_y = input.down(GLFW_KEY_W) - input.down(GLFW_KEY_S);
			int input_x = input.down(GLFW_KEY_D) - input.down(GLFW_KEY_A);


			int winw, winh;
			glfwGetWindowSize(glo::wctx.win, &winw, &winh);

			glm::vec2 mousevel = input.mouse_vel();
			if (glm::abs(mousevel.x) < winw - 16) {
				self->ang.x += mousevel.x * 0.2f;
			}
			if (glm::abs(mousevel.y) < winh - 16) {
				self->ang.y += mousevel.y * -0.2f;
				self->ang.y = glm::clamp(self->ang.y, -89.0f, 89.0f);
			}

			update_pitch_yaw_from_ang(self);
			update_front_from_pitch_yaw(self);
			glm::vec3 side = glm::normalize(glm::cross(front, up));

			self->pos += (input_x * 0.6f) * side;
			self->pos += (input_y * 0.6f) * front;
		}
	}

	if (active) {
		Camera& cam = glo::wctx.cam;
		cam.pos = self->pos;
		cam.front = front;
		cam.up = up;
		cam.yaw = yaw;
		cam.pitch = pitch;
		cam.proj_is_perspective = (type != CCTV_TOPDOWN);
	}
}

void CCam::update_lights(Entity* self, Shader* shd) {
	if (type == CCam::CCTV_SCANNER) {
		Light& l = self->light.light;

		if (l.type == Light::SPOT_LIGHT) {
			l.spotlight.direction = get_front();
			l.apply_type_fields(shd);
		}
	}
}

glm::vec3 CCam::get_front() const {
	return front;
}

void CCam::update_pitch_yaw_from_ang(Entity* self) {
	yaw = self->ang.x;
	pitch = self->ang.y;
}

void CCam::update_front_from_pitch_yaw(Entity* self) {
	glm::vec3 direction = glm::vec3(
		cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
		sin(glm::radians(pitch)),
		sin(glm::radians(yaw)) * cos(glm::radians(pitch))
	);
	front = glm::normalize(direction);
}