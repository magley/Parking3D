#include <stdio.h>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "global.h"

#include "rend/model.h"

int main(int argc, char** argv) {
	glfwInit();
	glo::wctx.win = glfwCreateWindow(640, 480, "Parking3D", nullptr, nullptr);
	glfwMakeContextCurrent(glo::wctx.win);
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	Model mdl("assets/models/TestModel.obj");
	Shader basic3d("assets/shader/basic3d.vert", "assets/shader/basic3d.frag");

	while (!glfwWindowShouldClose(glo::wctx.win)) {
		glfwPollEvents();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mdl.draw(basic3d);

		glfwSwapBuffers(glo::wctx.win);
	}

	return 0;
}