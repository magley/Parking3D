#include <stdio.h>
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <sstream>

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

	Model* mdl_car = glo::wctx.resmng.load_mdl("car.obj");
	Model* mdl_ramp = glo::wctx.resmng.load_mdl("ramp.obj");
	Model* mdl_parking = glo::wctx.resmng.load_mdl("parking.obj");
	Shader* basic3d = glo::wctx.resmng.load_shd("basic3d");

	glm::mat4 VP = glo::wctx.cam.proj * glo::wctx.cam.view();
	basic3d->set_mat4("VP", &VP[0][0]);


	Entity* car = glo::wctx.entity.add(glm::vec3(2, 0, 0));
	car->add(Component::MODEL);
	car->model.mdl = mdl_car;
	car->model.shd = basic3d;

	Entity* ramp = glo::wctx.entity.add(glm::vec3(0, 0, 0));
	ramp->add(Component::MODEL);
	ramp->model.mdl = mdl_ramp;
	ramp->model.shd = basic3d;
	ramp->add(Component::RAMP);
	ramp->ramp.state = CRamp::State::RISE;

	Entity* parking = glo::wctx.entity.add(glm::vec3(0, 0, 0));
	parking->add(Component::MODEL);
	parking->model.mdl = mdl_parking;
	parking->model.shd = basic3d;

	Entity* cam_free = glo::wctx.entity.add(glm::vec3(-3, 1, 3));
	cam_free->add(Component::CAM);
	cam_free->cam = CCam(CCam::FREE, true);

	Entity* cam1 = glo::wctx.entity.add(glm::vec3(13.987226, 14.390991, -5.179083));
	cam1->ang = glm::vec3(138.399963, -41.799999, 0.000000);
	cam1->add(Component::CAM);
	cam1->cam = CCam(CCam::CCTV_SCANNER, false);

	Entity* cam2 = glo::wctx.entity.add(glm::vec3(-9.133455, 14.165318, 13.735492));
	cam2->ang = glm::vec3(-39.000072, -46.999989, 0.000000);
	cam2->add(Component::CAM);
	cam2->cam = CCam(CCam::CCTV_SCANNER, false);

	int cam_index = 0;

	basic3d->set_vec3("pointLights[0].ambient", 0.2f, 0.25f, 0.3f);
	basic3d->set_vec3("pointLights[0].diffuse", 0.2f, 0.25f, 0.3f);
	basic3d->set_vec3("pointLights[0].specular", 0.25f, 0.25f, 0.25f);
	basic3d->set_float("pointLights[0].constant", 1.0f);
	basic3d->set_float("pointLights[0].linear", 0.022f);
	basic3d->set_float("pointLights[0].quadratic", 0.0019f);

	basic3d->set_vec3("spotLights[0].ambient", 0.006, 0.012, 0.012);
	basic3d->set_vec3("spotLights[0].diffuse", 0.2f, 0.25f, 0.3f);
	basic3d->set_vec3("spotLights[0].specular", 0.25f, 0.25f, 0.25f);
	basic3d->set_float("spotLights[0].cutOff", glm::cos(glm::radians(6.0f)));
	basic3d->set_float("spotLights[0].outerCutOff", glm::cos(glm::radians(22.0f)));

	basic3d->set_vec3("spotLights[1].ambient", 0.006, 0.012, 0.012);
	basic3d->set_vec3("spotLights[1].diffuse", 0.2f, 0.25f, 0.3f);
	basic3d->set_vec3("spotLights[1].specular", 0.25f, 0.25f, 0.25f);
	basic3d->set_float("spotLights[1].cutOff", glm::cos(glm::radians(6.0f)));
	basic3d->set_float("spotLights[1].outerCutOff", glm::cos(glm::radians(22.0f)));

	basic3d->set_int("pointLights_count", 1);
	basic3d->set_int("spotLights_count", 2);

	while (!glfwWindowShouldClose(glo::wctx.win)) {
		glfwPollEvents();

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
		glm::mat4 VP = glo::wctx.cam.proj * glo::wctx.cam.view();
		basic3d->set_mat4("VP", &VP[0][0]);
		basic3d->set_vec3("pointLights[0].position", cam_free->pos.x, cam_free->pos.y, cam_free->pos.z);
		basic3d->set_vec3("viewPos", cam.pos.x, cam.pos.y, cam.pos.z);

		if (glo::wctx.input.press(GLFW_KEY_SPACE)) {
			std::vector<int> cam_entity_index;
			for (int i = 0; i < glo::wctx.entity.size(); i++) {
				Entity* e = glo::wctx.entity.arr[i];
				if (e->has(Component::CAM)) {
					cam_entity_index.push_back(i);
				}
			}

			cam_index++;
			cam_index = cam_index % cam_entity_index.size();
	
			int cam_entity_target_index = cam_entity_index[cam_index];
			for (int i = 0; i < cam_entity_index.size(); i++) {
				int index = cam_entity_index[i];
				Entity* e = glo::wctx.entity.arr[index];
				e->cam.active = (index == cam_entity_target_index);
			}
		}

		if (glo::wctx.input.press(GLFW_KEY_Z)) {
			for (int i = 0; i < glo::wctx.entity.size(); i++) {
				Entity* e = glo::wctx.entity.arr[i];
				printf("%d [%f %f %f]  [%f %f %f]\n", i, e->pos.x, e->pos.y, e->pos.z, e->ang.x, e->ang.y, e->ang.z);
			}
		}

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < glo::wctx.entity.size(); i++) {
			Entity* e = glo::wctx.entity.arr[i];
			if (e->has(Component::RAMP)) {
				e->ramp.update(e);
			}
		}

		for (int i = 0, spotlight = 0; i < glo::wctx.entity.size(); i++) {
			Entity* e = glo::wctx.entity.arr[i];
			if (e->has(Component::CAM)) {
				e->cam.update(e);

				if (e->cam.type == CCam::CCTV_SCANNER) {
					std::stringstream ss;
					ss << "spotLights[" << spotlight << "]";
					std::string s = ss.str();
						
					glm::vec3 front = e->cam.get_front();

					basic3d->set_vec3((s + ".position").c_str(), e->pos.x, e->pos.y, e->pos.z);
					basic3d->set_vec3((s + ".direction").c_str(), front.x, front.y, front.z);
					spotlight++;
				}
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