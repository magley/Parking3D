#pragma once
#include "GLFW/glfw3.h"
#include "rend/camera.h"
#include "resource/res_mng.h"

struct WinCtx {
	GLFWwindow* win = nullptr;
	Camera cam;
	ResMng resmng;

	WinCtx() {};
};