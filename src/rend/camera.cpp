#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {
	// TODO: Externalize window W and H.
	proj = glm::perspective(glm::radians(65.0f), 640.0f / 480.0f, 0.1f, 100.0f);
}

glm::mat4 Camera::view() const {
	return glm::lookAt(pos, pos + front, up);
}