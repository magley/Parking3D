#pragma once
#include "GLFW/glfw3.h"
#include "rend/camera.h"

struct WinCtx {
	GLFWwindow* win = nullptr;
	Camera cam;

	WinCtx() {};
};