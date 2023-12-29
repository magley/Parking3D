#include <stdio.h>
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>

#include "global.h"
#include "rend/model.h"
#include "rend/light.h"
#include "rend/mesh2d.h"
#include "2d/hud.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

void wrap_mouse_if_needed() {
	double mxpos, mypos; // Get mouse position, relative to window
	glfwGetCursorPos(glo::wctx.win, &mxpos, &mypos);

	int width, height;   // Get dimensions of window
	glfwGetWindowSize(glo::wctx.win, &width, &height);

	if (mxpos > width) {
		glfwSetCursorPos(glo::wctx.win, 0, mypos);
	}
	else if (mxpos < 0) {
		glfwSetCursorPos(glo::wctx.win, width, mypos);
	}
	if (mypos > height) {
		glfwSetCursorPos(glo::wctx.win, mxpos, 0);
	}
	else if (mypos < 0) {
		glfwSetCursorPos(glo::wctx.win, mxpos, height);
	}
}

int main(int argc, char** argv) {
	glfwInit();
	glo::wctx.win = glfwCreateWindow(1280, 720, "Parking3D", nullptr, nullptr);
	glfwMakeContextCurrent(glo::wctx.win);
	//glfwSetInputMode(glo::wctx.win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(glo::wctx.win, framebuffer_size_callback);
	glfwSwapInterval(1);
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Model* mdl_car = glo::wctx.resmng.load_mdl("car.obj");
	Model* mdl_ramp = glo::wctx.resmng.load_mdl("ramp.obj");
	Model* mdl_parking = glo::wctx.resmng.load_mdl("parking.obj");
	Shader* basic3d = glo::wctx.resmng.load_shd("basic3d");
	Shader* basic2d = glo::wctx.resmng.load_shd("basic2d");

	glm::mat4 VP = glo::wctx.cam.proj * glo::wctx.cam.view();
	basic3d->set_mat4("VP", &VP[0][0]);

	Entity* car = glo::wctx.entity.add(glm::vec3(2, 0, 0));
	car->add(Component::MODEL);
	car->model.mdl = mdl_car;
	car->model.shd = basic3d;

	Entity* ramp = glo::wctx.entity.add(glm::vec3(-9.001070, 0.393314, -18.018282));
	ramp->add(Component::MODEL);
	ramp->model.mdl = mdl_ramp;
	ramp->model.shd = basic3d;
	ramp->add(Component::RAMP);
	ramp->ramp.state = CRamp::State::RISE;

	Entity* parking = glo::wctx.entity.add(glm::vec3(0, 0, 0));
	parking->add(Component::MODEL);
	parking->model.mdl = mdl_parking;
	parking->model.shd = basic3d;

	Entity* cam_free = glo::wctx.entity.add(glm::vec3(-3, 6, 3));
	cam_free->add(Component::CAM);
	cam_free->cam = CCam(CCam::FREE, true);
	cam_free->add(Component::LIGHT);
	cam_free->light.light = Light(Light::POINT_LIGHT, "pointLights[0]", { 0.2, 0.25, 0.3 }, { 0.2, 0.25, 0.3 }, { 0.25, 0.25, 0.25 });
	cam_free->light.light.pointlight = PointLight_Fields(1, 0.022, 0.0019);
	cam_free->light.light.apply_colors(basic3d);
	cam_free->light.light.apply_type_fields(basic3d);

	Entity* cam1 = glo::wctx.entity.add(glm::vec3(13.987226, 14.390991, -5.179083));
	cam1->ang = glm::vec3(138.399963, -41.799999, 0.000000);
	cam1->add(Component::CAM);
	cam1->cam = CCam(CCam::CCTV_SCANNER, false);
	cam1->add(Component::LIGHT);
	cam1->light.light = Light(Light::SPOT_LIGHT, "spotLights[0]", { 0.006, 0.012, 0.012 }, { 0.2f, 0.25f, 0.3f }, { 0.25f, 0.25f, 0.25f });
	cam1->light.light.spotlight = SpotLight_Fields({}, 6.0, 22.0);
	cam1->light.light.apply_colors(basic3d);
	cam1->light.light.apply_type_fields(basic3d);

	Entity* cam2 = glo::wctx.entity.add(glm::vec3(-9.133455, 14.165318, 13.735492));
	cam2->ang = glm::vec3(-39.000072, -46.999989, 0.000000);
	cam2->add(Component::CAM);
	cam2->cam = CCam(CCam::CCTV_SCANNER, false);
	cam2->add(Component::LIGHT);
	cam2->light.light = Light(Light::SPOT_LIGHT, "spotLights[1]", { 0.006, 0.012, 0.012 }, { 0.2f, 0.25f, 0.3f }, { 0.25f, 0.25f, 0.25f });
	cam2->light.light.spotlight = SpotLight_Fields({}, 6.0, 22.0);
	cam2->light.light.apply_colors(basic3d);
	cam2->light.light.apply_type_fields(basic3d);

	Entity* cam3 = glo::wctx.entity.add(glm::vec3(6.630467, 47.472813, 7.158076));
	cam3->ang = glm::vec3(90.0f, -89.000000, 0.000000);
	cam3->add(Component::CAM);
	cam3->cam = CCam(CCam::CCTV_TOPDOWN, false);

	Entity* cam4 = glo::wctx.entity.add(glm::vec3(-3.198956, 4.919804, -23.898209));
	cam4->ang = glm::vec3(140.599930, -36.199982, 0.000000);
	cam4->add(Component::CAM);
	cam4->cam = CCam(CCam::CCTV_SCANNER, false);
	cam4->add(Component::LIGHT);
	cam4->light.light = Light(Light::POINT_LIGHT, "pointLights[1]", { 0.93f, 0.5f, 0.29f }, { 0.33f, 0.1f, 0.0f }, { 0.25f, 0.25f, 0.25f });
	cam4->light.light.pointlight = PointLight_Fields(1, 0.12f, 0.0019f);
	cam4->light.light.apply_colors(basic3d);
	cam4->light.light.apply_type_fields(basic3d);

	int point_lights_count = 0;
	int spot_lights_count = 0;

	Texture* tex_map = glo::wctx.resmng.load_tex("tex_map.png", 5);
	Mesh2D mdl_2d(Mesh2D::SQUARE);

	Hud hud(&mdl_2d, tex_map);

	while (!glfwWindowShouldClose(glo::wctx.win)) {
		glfwPollEvents();
		wrap_mouse_if_needed();

		glo::wctx.input.update(glo::wctx.win);

		if (glo::wctx.input.press(GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(glo::wctx.win, GLFW_TRUE);
		}

		if (glo::wctx.input.press(GLFW_KEY_1)) {
			glo::wctx.wireframe ^= true;
			int m = glo::wctx.wireframe ? GL_LINE : GL_FILL;
			glPolygonMode(GL_FRONT_AND_BACK, m);
		}

		Camera& cam = glo::wctx.cam;
		cam.update_proj();
		glm::mat4 VP = glo::wctx.cam.proj * glo::wctx.cam.view();
		basic3d->set_mat4("VP", &VP[0][0]);
		cam_free->light.light.apply_position(basic3d, cam_free->pos);
		basic3d->set_vec3("viewPos", cam.pos.x, cam.pos.y, cam.pos.z);

		if (glo::wctx.input.press(GLFW_KEY_SPACE)) {
			glo::game.set_cam(glo::game._cam_index + 1);
		}

		if (glo::wctx.input.press(GLFW_KEY_Z)) {
			for (int i = 0; i < glo::wctx.entity.size(); i++) {
				Entity* e = glo::wctx.entity.arr[i];
				printf("%d [%f %f %f]  [%f %f %f]\n", i, e->pos.x, e->pos.y, e->pos.z, e->ang.x, e->ang.y, e->ang.z);
			}
		}

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < glo::wctx.entity.size(); i++) {
			Entity* e = glo::wctx.entity.arr[i];
			if (e->has(Component::RAMP)) {
				e->ramp.update(e);
			}
		}

		point_lights_count = 0;
		spot_lights_count = 0;
		for (int i = 0; i < glo::wctx.entity.size(); i++) {
			Entity* e = glo::wctx.entity.arr[i];
			if (e->has(Component::LIGHT)) {
				e->light.update_and_apply_position(e, basic3d);

				if (e->light.light.type == Light::POINT_LIGHT) point_lights_count++;
				if (e->light.light.type == Light::SPOT_LIGHT) spot_lights_count++;
			}
		}
		basic3d->set_int("pointLights_count", point_lights_count);
		basic3d->set_int("spotLights_count", spot_lights_count);

		for (int i = 0; i < glo::wctx.entity.size(); i++) {
			Entity* e = glo::wctx.entity.arr[i];
			if (e->has(Component::CAM)) {
				e->cam.update(e);
				e->cam.update_lights(e, basic3d);
			}
		}
	
		for (int i = 0; i < glo::wctx.entity.size(); i++) {
			Entity* e = glo::wctx.entity.arr[i];
			if (e->has(Component::MODEL)) {
				e->model.draw(e);
			}
		}

		hud.update();
		hud.draw(basic2d);

		glfwSwapBuffers(glo::wctx.win);
	}

	return 0;
}