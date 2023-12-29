#pragma once
#include "GLFW/glfw3.h"
#include "rend/camera.h"
#include "resource/res_mng.h"
#include "entity/entitymng.h"
#include "util/input.h"
#include "entity/event.h"

struct WinCtx {
	GLFWwindow* win = nullptr;
	Camera cam;
	ResMng resmng;
	EntityMng entity;
	EventMng event;
	Input input;

	bool wireframe = false;
	bool shaded = true;

	WinCtx() {};
};