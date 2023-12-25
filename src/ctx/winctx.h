#pragma once
#include "GLFW/glfw3.h"
#include "rend/camera.h"
#include "resource/res_mng.h"
#include "entity/entitymng.h"

struct WinCtx {
	GLFWwindow* win = nullptr;
	Camera cam;
	ResMng resmng;
	EntityMng entity;

	WinCtx() {};
};