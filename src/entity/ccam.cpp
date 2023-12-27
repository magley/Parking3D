#include "ccam.h"
#include "global.h"

void CCam::update(Entity* self) {
	switch (type) {
	case CCTV_SCANNER:
		break;
	case CCTV_TOPDOWN:
		break;
	case FREE:
		Input& input = glo::wctx.input;
		int input_y = input.down(GLFW_KEY_W) - input.down(GLFW_KEY_S);
		int input_x = input.down(GLFW_KEY_D) - input.down(GLFW_KEY_A);

		yaw += input.mouse_vel().x * 0.2f;
		pitch += input.mouse_vel().y * -0.2f;
		pitch = glm::clamp(pitch, -89.0f, 89.0f);

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(direction);

		glm::vec3 side = glm::normalize(glm::cross(front, up));
		self->pos += (input_x * 0.1f) * side;
		self->pos += (input_y * 0.1f) * front;
	}

	if (active) {
		Camera& cam = glo::wctx.cam;
		cam.pos = self->pos;
		cam.front = front;
		cam.up = up;
		cam.yaw = yaw;
		cam.pitch = pitch;
	}
}