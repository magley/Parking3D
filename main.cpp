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
	glfwSwapInterval(1);
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);

	Model* mdl = glo::wctx.resmng.load_mdl("ramp.obj");
	Shader* basic3d = glo::wctx.resmng.load_shd("basic3d");


	glm::mat4 VP = glo::wctx.cam.proj * glo::wctx.cam.view();
	basic3d->set_mat4("VP", &VP[0][0]);

	while (!glfwWindowShouldClose(glo::wctx.win)) {
		glfwPollEvents();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		mdl->draw(*basic3d);

		glfwSwapBuffers(glo::wctx.win);
	}

	return 0;
}