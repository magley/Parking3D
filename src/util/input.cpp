#include "input.h"
#include "GLFW/glfw3.h"


void Input::update(GLFWwindow* win) {
	for (int i = 0; i < GLFW_KEY_LAST; i++) {
		kb_prev[i] = kb_curr[i];
		kb_curr[i] = glfwGetKey(win, i);
	}

	double mx, my;
	glfwGetCursorPos(win, &mx, &my);

	mp_prev = mp_curr;
	mp_curr = glm::vec2(mx, my);
}

bool Input::down(int scancode) const {
	return kb_curr[scancode];
}

bool Input::up(int scancode) const {
	return !kb_curr[scancode] && kb_prev[scancode];
}

bool Input::press(int scancode) const {
	return kb_curr[scancode] && !kb_prev[scancode];
}

glm::vec2 Input::mouse_vel() const {
	return glm::vec2(mp_curr.x - mp_prev.x, mp_curr.y - mp_prev.y);
}