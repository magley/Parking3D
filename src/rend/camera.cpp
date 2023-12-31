#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "global.h"

void Camera::update_proj() {
	int w, h;
	glfwGetWindowSize(glo::wctx.win, &w, &h);

	if (proj_is_perspective) {
		proj = glm::perspective(glm::radians(60.0f), w / (float)h, 0.1f, 100.0f);
	}
	else {
		float scale = 10.0f;
		float aspect = (float)w / h;
		proj = glm::ortho(-aspect * scale, aspect * scale, -scale, scale, -100.0f, 100.0f);
	}
}

glm::mat4 Camera::view() const {
	return glm::lookAt(pos, pos + front, up);
}