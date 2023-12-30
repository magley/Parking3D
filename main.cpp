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

#include <soloud.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

int main(int argc, char** argv) {
	glfwInit();
	glo::wctx.win = glfwCreateWindow(1280, 720, "Parking3D", nullptr, nullptr);
	glfwMakeContextCurrent(glo::wctx.win);
	glfwSetFramebufferSizeCallback(glo::wctx.win, framebuffer_size_callback);
	glfwSwapInterval(1);
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Model* mdl_car = glo::wctx.resmng.load_mdl("car.obj");
	Model* mdl_button = glo::wctx.resmng.load_mdl("sphere.obj");
	Model* mdl_ramp = glo::wctx.resmng.load_mdl("ramp.obj");
	Model* mdl_parking = glo::wctx.resmng.load_mdl("parking.obj");
	Shader* basic3d = glo::wctx.resmng.load_shd("basic3d");
	Shader* basic2d = glo::wctx.resmng.load_shd("basic2d");

	glm::mat4 VP = glo::wctx.cam.proj * glo::wctx.cam.view();
	basic3d->set_mat4("VP", &VP[0][0]);

	Entity* btn_light = glo::wctx.entity.add(glm::vec3(-7.948277, 0.845271, -23.074425));
	btn_light->add(Component::MODEL);
	btn_light->model.mdl = mdl_button;
	btn_light->model.shd = basic3d;
	btn_light->add(Component::BUTTON);
	btn_light->button = CButton(Event::EVENT_TOGGLE_HOUSE_LIGHT, false);
	btn_light->button.radius = 0.15f;

	Entity* btn_ramp = glo::wctx.entity.add(glm::vec3(-7.948277, 0.845271, -21.074425));
	btn_ramp->add(Component::MODEL);
	btn_ramp->model.mdl = mdl_button;
	btn_ramp->model.shd = basic3d;
	btn_ramp->add(Component::BUTTON);
	btn_ramp->button = CButton(Event::EVENT_TOGGLE_RAMP, false);
	btn_ramp->button.radius = 0.15f;

	Entity* ramp = glo::wctx.entity.add(glm::vec3(-9.001070, 0.393314, -18.018282));
	ramp->add(Component::MODEL);
	ramp->model.mdl = mdl_ramp;
	ramp->model.shd = basic3d;
	ramp->add(Component::RAMP);
	ramp->sub(Event::EVENT_TOGGLE_RAMP);

	Entity* parking = glo::wctx.entity.add(glm::vec3(0, 0, 0));
	parking->add(Component::MODEL);
	parking->model.mdl = mdl_parking;
	parking->model.shd = basic3d;

	Entity* cam_free = glo::wctx.entity.add(glm::vec3(-3, 6, 3));
	cam_free->add(Component::CAM);
	cam_free->cam = CCam(CCam::FREE, true);
	cam_free->add(Component::LIGHT);
	cam_free->light.shd = basic3d;
	cam_free->light.light = Light(Light::POINT_LIGHT, "pointLights[0]", { 0.05, 0.05, 0.1 }, { 0.2, 0.25, 0.3 }, { 0.25, 0.25, 0.25 });
	cam_free->light.light.pointlight = PointLight_Fields(1, 0.022, 0.0019);
	cam_free->light.light.apply_colors(basic3d);
	cam_free->light.light.apply_type_fields(basic3d);
	cam_free->light.light.apply_active(basic3d, true);

	Entity* cam1 = glo::wctx.entity.add(glm::vec3(13.987226, 14.390991, -5.179083));
	cam1->ang = glm::vec3(138.399963, -41.799999, 0.000000);
	cam1->add(Component::CAM);
	cam1->cam = CCam(CCam::CCTV_SCANNER, false);
	cam1->add(Component::LIGHT);
	cam1->light.shd = basic3d;
	cam1->light.light = Light(Light::SPOT_LIGHT, "spotLights[0]", { 0.016, 0.022, 0.022 }, { 0.2f, 0.25f, 0.3f }, { 0.25f, 0.25f, 0.25f });
	cam1->light.light.spotlight = SpotLight_Fields({}, 6.0, 22.0);
	cam1->light.light.apply_colors(basic3d);
	cam1->light.light.apply_type_fields(basic3d);
	cam1->light.light.apply_active(basic3d, true);

	Entity* cam2 = glo::wctx.entity.add(glm::vec3(-9.133455, 14.165318, 13.735492));
	cam2->ang = glm::vec3(-39.000072, -46.999989, 0.000000);
	cam2->add(Component::CAM);
	cam2->cam = CCam(CCam::CCTV_SCANNER, false);
	cam2->add(Component::LIGHT);
	cam2->light.shd = basic3d;
	cam2->light.light = Light(Light::SPOT_LIGHT, "spotLights[1]", { 0.016, 0.022, 0.022 }, { 0.2f, 0.25f, 0.3f }, { 0.25f, 0.25f, 0.25f });
	cam2->light.light.spotlight = SpotLight_Fields({}, 6.0, 22.0);
	cam2->light.light.apply_colors(basic3d);
	cam2->light.light.apply_type_fields(basic3d);
	cam2->light.light.apply_active(basic3d, true);

	Entity* cam3 = glo::wctx.entity.add(glm::vec3(6.630467, 47.472813, 7.158076));
	cam3->ang = glm::vec3(00.0f, -89.000000, 0.000000);
	cam3->add(Component::CAM);
	cam3->cam = CCam(CCam::CCTV_TOPDOWN, false);

	Entity* cam4 = glo::wctx.entity.add(glm::vec3(-3.198956, 4.919804, -23.898209));
	cam4->ang = glm::vec3(140.599930, -36.199982, 0.000000);
	cam4->add(Component::CAM);
	cam4->cam = CCam(CCam::CCTV_SCANNER, false);
	cam4->light.shd = basic3d;
	cam4->add(Component::LIGHT);
	cam4->light.light = Light(Light::POINT_LIGHT, "pointLights[1]", { 0.05, 0.05, 0.1 }, { 0.53f, 0.2f, 0.2f }, { 0.75f, 0.75f, 0.75f });
	cam4->light.light.pointlight = PointLight_Fields(1, 0.022f, 0.0019f);
	cam4->light.light.apply_colors(basic3d);
	cam4->light.light.apply_type_fields(basic3d);
	cam4->light.light.apply_active(basic3d, true);
	cam4->sub(Event::EVENT_TOGGLE_HOUSE_LIGHT);
	cam4->add(Component::AUDIO);
	cam4->audio.play_3d("light_buzz.mp3", cam4->pos, true);

	Entity* car[6];

	for (int i = 0; i < 6; i++) {
		car[i] = glo::wctx.entity.add(glm::vec3(2, 0, 0));
		car[i]->add(Component::MODEL);
		car[i]->model.mdl = mdl_car;
		car[i]->model.shd = basic3d;
		car[i]->add(Component::CAR);
		car[i]->car.color = Color(rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f);
		car[i]->car.spot = -1;
	}

	glo::game.parking_spots[0].pos = glm::vec3(13.286233, 0, 0.556348);
	glo::game.parking_spots[1].pos = glm::vec3(13.286233, 0, 8.956244);
	glo::game.parking_spots[2].pos = glm::vec3(13.286233, 0, 17.510393);
	glo::game.parking_spots[3].pos = glm::vec3(5.080829, 0, 0.556348);
	glo::game.parking_spots[4].pos = glm::vec3(5.080829, 0, 8.956244);
	glo::game.parking_spots[5].pos = glm::vec3(5.080829, 0, 17.510393);

	int point_lights_count = 0;
	int spot_lights_count = 0;

	glo::game.set_cam(1);

	Texture* tex_map = glo::wctx.resmng.load_tex("tex_map.png", 5);
	Texture* tex_crosshair = glo::wctx.resmng.load_tex("tex_crosshair.png");
	Mesh2D mdl_2d(Mesh2D::SQUARE);

	Hud hud(&mdl_2d, tex_map);

	int seizure_min = 250;
	int seizure_max = 500;
	float noise_intensity = 0.2;

	while (!glfwWindowShouldClose(glo::wctx.win)) {
		glfwPollEvents();
		
		if (glo::game.lock_cursor) {
			glfwSetInputMode(glo::wctx.win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode(glo::wctx.win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		glo::wctx.input.update(glo::wctx.win);


		/////////////////////////////////////////////////

		Input& input = glo::wctx.input;
		for (int i = 0; i < 6; i++) {
			if (input.press(GLFW_KEY_1 + i)) {
				if (input.down(GLFW_KEY_LEFT_CONTROL)) {
					glo::game.parking_spots[i].time_left = 0;
				}
				else if (input.down(GLFW_KEY_LEFT_SHIFT)) {
					glo::game.parking_spots[i].time_left = 20 * 60;
				}
				else {
					glo::game.parking_spots[i].time_left = 20 * 60;
					glo::game.parking_spots[i].used_by = car[i];
					car[i]->car.spot = i;
					car[i]->pos = glo::game.parking_spots[i].pos;
				}
			}

			glo::game.parking_spots[i].time_left -= 1;

			if (glo::game.parking_spots[i].time_left <= 0) {
				glo::game.parking_spots[i].used_by = nullptr;
				car[i]->car.spot = -1;
				car[i]->pos = glm::vec3(0, 999, 0);
			}
		}

		/////////////////////////////////////////////////

		if (glo::wctx.input.press(GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(glo::wctx.win, GLFW_TRUE);
		}

		if (glo::wctx.input.press(GLFW_KEY_F1)) {
			glo::wctx.wireframe ^= true;
			int m = glo::wctx.wireframe ? GL_LINE : GL_FILL;
			glPolygonMode(GL_FRONT_AND_BACK, m);
		}

		if (glo::wctx.input.press(GLFW_KEY_F2)) {
			glo::wctx.shaded ^= true;
			basic3d->set_int("u_unlit", !glo::wctx.shaded);
		}

		Camera& cam = glo::wctx.cam;
		cam.update_proj();
		glm::mat4 VP = glo::wctx.cam.proj * glo::wctx.cam.view();
		basic3d->set_mat4("VP", &VP[0][0]);
		cam_free->light.light.apply_position(basic3d, cam_free->pos);
		basic3d->set_vec3("viewPos", cam.pos.x, cam.pos.y, cam.pos.z);

		glo::wctx.audio.update_3d_listener(cam.pos, cam.pos + cam.front, cam.up);

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
				e->light.update_and_apply_position(e);

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
			if (e->has(Component::BUTTON)) {
				e->button.update(e);
			}
		}

		hud.update();

		for (int i = 0; i < glo::wctx.entity.size(); i++) {
			Entity* e = glo::wctx.entity.arr[i];
			e->check_ev();
		}
		glo::wctx.event.consume();
		glo::game.update_noise();

		//////////////////////////////////////////////////////////
		// Render
		//////////////////////////////////////////////////////////

		basic3d->set_int("u_noise_seed", glo::game.noise.rand);
		basic3d->set_float("u_noise_intensity", glo::game.noise.intensity);
		basic3d->set_float("u_noise_seizure", glo::game.noise.seizure);
	
		for (int i = 0; i < glo::wctx.entity.size(); i++) {
			Entity* e = glo::wctx.entity.arr[i];
			if (e->has(Component::MODEL)) {
				e->model.draw(e);
			}
		}

		hud.draw(basic2d);

		// Draw crosshair.
		if (glo::game.lock_cursor) {
			int w, h;
			glfwGetWindowSize(glo::wctx.win, &w, &h);
			glfwGetWindowSize(glo::wctx.win, &w, &h);
			glm::mat4 proj = glm::ortho((float)0, (float)w, -(float)h, (float)0, -100.0f, 100.0f);

			basic2d->set_mat4("u_proj", &proj[0][0]);
			basic2d->set_vec2("u_pos", w / 2, -h /2);
			basic2d->set_vec2("u_scale", tex_crosshair->w, tex_crosshair->h);
			basic2d->set_vec3("u_img_frame", 1, 1, 0);
			mdl_2d.draw(basic2d, tex_crosshair);
		}

		glfwSwapBuffers(glo::wctx.win);
	}

	return 0;
}