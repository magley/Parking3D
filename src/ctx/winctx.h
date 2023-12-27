#pragma once
#include "GLFW/glfw3.h"
#include "rend/camera.h"
#include "resource/res_mng.h"
#include "entity/entitymng.h"
#include "util/input.h"

struct WinCtx {
	GLFWwindow* win = nullptr;
	Camera cam;
	ResMng resmng;
	EntityMng entity;
	Input input;

	WinCtx() {};
};