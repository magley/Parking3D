#include <stdio.h>
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "global.h"
#include "rend/model.h"

int main(int argc, char** argv) {
	glfwInit();
	glo::wctx.win = glfwCreateWindow(640, 480, "Parking3D", nullptr, nullptr);
	glfwMakeContextCurrent(glo::wctx.win);
	glfwSetInputMode(glo::wctx.win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSwapInterval(1);
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);

	Model* mdl_ramp = glo::wctx.resmng.load_mdl("ramp.obj");
	Model* mdl_car = glo::wctx.resmng.load_mdl("car.obj");
	Shader* basic3d = glo::wctx.resmng.load_shd("basic3d");

	glm::mat4 VP = glo::wctx.cam.proj * glo::wctx.cam.view();
	basic3d->set_mat4("VP", &VP[0][0]);

	Entity* ramp = glo::wctx.entity.add(glm::vec3(0, 0, 0));
	ramp->add(Component::MODEL);
	ramp->model.mdl = mdl_ramp;
	ramp->model.shd = basic3d;
	ramp->add(Component::RAMP);
	ramp->ramp.state = CRamp::State::RISE;

	Entity* car = glo::wctx.entity.add(glm::vec3(2, 0, 0));
	car->add(Component::MODEL);
	car->model.mdl = mdl_car;
	car->model.shd = basic3d;

	Entity* cam_free = glo::wctx.entity.add(glm::vec3(0, 0, 3));
	cam_free->add(Component::CAM);
	cam_free->cam = CCam(CCam::FREE, true);


	while (!glfwWindowShouldClose(glo::wctx.win)) {
		glfwPollEvents();

		glo::wctx.input.update(glo::wctx.win);

		if (glo::wctx.input.press(GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(glo::wctx.win, GLFW_TRUE);
		}

		Camera& cam = glo::wctx.cam;
		glm::mat4 VP = glo::wctx.cam.proj * glo::wctx.cam.view();
		basic3d->set_mat4("VP", &VP[0][0]);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < glo::wctx.entity.size(); i++) {
			Entity* e = glo::wctx.entity.arr[i];
			if (e->has(Component::RAMP)) {
				e->ramp.update(e);
			}
		}

		for (int i = 0; i < glo::wctx.entity.size(); i++) {
			Entity* e = glo::wctx.entity.arr[i];
			if (e->has(Component::CAM)) {
				e->cam.update(e);
			}
		}
	
		for (int i = 0; i < glo::wctx.entity.size(); i++) {
			Entity* e = glo::wctx.entity.arr[i];
			if (e->has(Component::MODEL)) {
				e->model.draw(e);
			}
		}

		glfwSwapBuffers(glo::wctx.win);
	}

	return 0;
}