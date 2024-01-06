#pragma once
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

struct Input {
	int kb_curr[GLFW_KEY_LAST];
	int kb_prev[GLFW_KEY_LAST];

	int mb_curr[3];
	int mb_prev[3];
	glm::vec2 mp_curr;
	glm::vec2 mp_prev;
	void update(GLFWwindow* win);
	bool down(int scancode) const;
	bool up(int scancode) const;
	bool press(int scancode) const;
	bool mdown(int button) const;
	bool mup(int button) const;
	bool mpress(int button) const;
	glm::vec2 mouse_vel() const;
};