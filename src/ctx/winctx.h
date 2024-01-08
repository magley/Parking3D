#pragma once
#include "GLFW/glfw3.h"

struct WinCtx {
	GLFWwindow* win          = nullptr;
	bool wireframe           = false;
	bool shaded              = true;
	bool cars_transparent_2d = false;
	void get_size(int* w, int* h);

	WinCtx() {};
};