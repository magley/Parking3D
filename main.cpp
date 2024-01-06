#include <stdio.h>
#include <sstream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp>
#include <soloud.h>

#include "rend/model.h"
#include "rend/light.h"
#include "rend/mesh2d.h"
#include "2d/hud.h"
#include "2d/parking2d.h"

#include "subsystem/subsystem_audio.h"
#include "subsystem/subsystem_camera.h"
#include "subsystem/subsystem_entity.h"
#include "subsystem/subsystem_event.h"
#include "subsystem/subsystem_game.h"
#include "subsystem/subsystem_input.h"
#include "subsystem/subsystem_resource.h"
#include "subsystem/subsystem_window.h"

#include "util/log.h"

unsigned int fbo, texture;

void init_texture_framebuffer(int width, int height) {
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &texture);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void on_framebuffer_resize() {
	int width, height;
	glfwGetWindowSize(glo::win->win, &width, &height);
	glViewport(0, 0, width, height);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	on_framebuffer_resize();
}

int main(int argc, char** argv) {
	// Initialize engine subsystems.

	log("Initializing engine subsystems...");
	glo::win = new WinCtx();
	glo::audio = new AudioCore();
	glo::cam = new Camera();
	glo::entity = new EntityMng();
	glo::event = new EventMng();
	glo::game = new Game();
	glo::input = new Input();
	glo::res = new ResMng();

	// Initialize OpenGL and the window context.

	log("Creating a window...");
	glfwInit();
	glo::win->win = glfwCreateWindow(1280, 720, "Parking3D", nullptr, nullptr);
	glfwMakeContextCurrent(glo::win->win);
	glfwSetFramebufferSizeCallback(glo::win->win, framebuffer_size_callback);
	glfwSwapInterval(1);
	log("Initializing the OpenGL context...");
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	init_texture_framebuffer(800, 600);

	// Entities.

	log("Loading resources...");
	Model* mdl_car = glo::res->load_mdl("car.obj");
	Model* mdl_button = glo::res->load_mdl("sphere.obj");
	Model* mdl_ramp = glo::res->load_mdl("ramp.obj");
	Model* mdl_parking = glo::res->load_mdl("parking.obj");
	Model* mdl_house_window = glo::res->load_mdl("window.obj");
	Model* mdl_mini_screen = glo::res->load_mdl("mini_screen.obj");
	Model* mdl_human = glo::res->load_mdl("human_sitting.obj");
	Model* mdl_chair = glo::res->load_mdl("chair.obj");
	Model* mdl_ramp_base = glo::res->load_mdl("ramp_base.obj");
	Shader* basic3d = glo::res->load_shd("basic3d");
	Shader* basic2d = glo::res->load_shd("basic2d");
	Texture* tex_screen_framebuffer = new Texture(texture, 800, 600);
	Texture* tex_map = glo::res->load_tex("tex_map.png", 5);
	Texture* tex_crosshair = glo::res->load_tex("tex_crosshair.png");
	Texture* tex_pixel = glo::res->load_tex("tex_pixel.png");
	Texture* tex_parking_2d = glo::res->load_tex("tex_parking_2d.png");
	Texture* tex_help = glo::res->load_tex("tex_help.png");
	Mesh2D mdl_2d(Mesh2D::SQUARE);
	Mesh2D mdl_2d_circle(Mesh2D::CIRCLE);

	glm::mat4 VP = glo::cam->proj * glo::cam->view();
	basic3d->set_mat4("VP", &VP[0][0]);

	log("Creating entities...");

	Entity* btn_light = glo::entity->add(glm::vec3(-7.948277, 0.845271, -23.074425));
	btn_light->add(Component::MODEL);
	btn_light->model.mdl = mdl_button;
	btn_light->model.shd = basic3d;
	btn_light->add(Component::BUTTON);
	btn_light->button = CButton(Event::EVENT_TOGGLE_HOUSE_LIGHT, false);
	btn_light->button.radius = 0.15f;

	Entity* btn_ramp = glo::entity->add(glm::vec3(-7.948277, 0.845271, -21.074425));
	btn_ramp->add(Component::MODEL);
	btn_ramp->model.mdl = mdl_button;
	btn_ramp->model.shd = basic3d;
	btn_ramp->add(Component::BUTTON);
	btn_ramp->button = CButton(Event::EVENT_TOGGLE_RAMP, false);
	btn_ramp->button.radius = 0.15f;

	Entity* ramp = glo::entity->add(glm::vec3(-10.001070, 0.393314, -19.018282));
	ramp->add(Component::MODEL);
	ramp->model.mdl = mdl_ramp;
	ramp->model.shd = basic3d;
	ramp->add(Component::RAMP);
	ramp->sub(Event::EVENT_TOGGLE_RAMP);

	Entity* ramp_base = glo::entity->add(glm::vec3(-10.001070, 0.393314, -19.018282));
	ramp_base->add(Component::MODEL);
	ramp_base->model.mdl = mdl_ramp_base;
	ramp_base->model.shd = basic3d;

	Entity* parking = glo::entity->add(glm::vec3(0, 0, 0));
	parking->add(Component::MODEL);
	parking->model.mdl = mdl_parking;
	parking->model.shd = basic3d;
	parking->model.mdl->meshes[2].material.emission_map = tex_screen_framebuffer;
	parking->add(Component::AUDIO);
	parking->audio.play("ambient.ogg", true);

	Entity* house_window = glo::entity->add(glm::vec3(-8.4, 2.1, -21.3));
	house_window->add(Component::MODEL);
	house_window->model.mdl = mdl_house_window;
	house_window->model.shd = basic3d;
	house_window->model.transparent = true;

	Entity* cam_free = glo::entity->add(glm::vec3(-3, 6, 3));
	cam_free->add(Component::CAM);
	cam_free->cam = CCam(CCam::FREE, true);
	cam_free->add(Component::LIGHT);
	cam_free->light.shd = basic3d;
	cam_free->light.light = Light(Light::POINT_LIGHT, "pointLights[0]", { 0.05, 0.05, 0.1 }, { 0.4, 0.5, 0.7 }, { 0.65, 0.65, 0.65 });
	cam_free->light.light.pointlight = PointLight_Fields(1.0f, 0.022f, 0.0019f);
	cam_free->light.light.apply_colors(basic3d);
	cam_free->light.light.apply_type_fields(basic3d);
	cam_free->light.light.apply_active(basic3d, true);

	Entity* cam1 = glo::entity->add(glm::vec3(13.987226f, 14.390991f, -5.179083f));
	cam1->ang = glm::vec3(138.399963f, -41.799999f, 0.000000f);
	cam1->add(Component::CAM);
	cam1->cam = CCam(CCam::CCTV_SCANNER, false);
	cam1->add(Component::LIGHT);
	cam1->light.shd = basic3d;
	cam1->light.light = Light(Light::SPOT_LIGHT, "spotLights[0]", { 0.016, 0.022, 0.022 }, glm::vec3({ 0.2f, 0.25f, 0.3f }) * 2.5f, { 0.45f, 0.45f, 0.45f });
	cam1->light.light.spotlight = SpotLight_Fields({}, 6.0f, 22.0f);
	cam1->light.light.apply_colors(basic3d);
	cam1->light.light.apply_type_fields(basic3d);
	cam1->light.light.apply_active(basic3d, true);

	Entity* cam2 = glo::entity->add(glm::vec3(-9.133455f, 14.165318f, 13.735492f));
	cam2->ang = glm::vec3(-39.0f, -47.0f, 0.0f);
	cam2->add(Component::CAM);
	cam2->cam = CCam(CCam::CCTV_SCANNER, false);
	cam2->cam.timer = 45;
	cam2->add(Component::LIGHT);
	cam2->light.shd = basic3d;
	cam2->light.light = Light(Light::SPOT_LIGHT, "spotLights[1]", { 0.016, 0.022, 0.022 }, glm::vec3({ 0.2f, 0.25f, 0.3f }) * 2.5f, { 0.45f, 0.45f, 0.45f });
	cam2->light.light.spotlight = SpotLight_Fields({}, 6.0, 22.0);
	cam2->light.light.apply_colors(basic3d);
	cam2->light.light.apply_type_fields(basic3d);
	cam2->light.light.apply_active(basic3d, true);

	Entity* cam3 = glo::entity->add(glm::vec3(8.630467, 43.472813, 9.658076));
	cam3->ang = glm::vec3(00.0f, -89.000000, 0.000000);
	cam3->add(Component::CAM);
	cam3->cam = CCam(CCam::CCTV_TOPDOWN, false);

	Entity* cam4 = glo::entity->add(glm::vec3(-3.198956, 4.919804, -23.898209));
	cam4->ang = glm::vec3(140.599930, -36.199982, 0.000000);
	cam4->add(Component::CAM);
	cam4->cam = CCam(CCam::CCTV_STATIONARY, false);
	cam4->light.shd = basic3d;
	cam4->add(Component::LIGHT);
	cam4->light.light = Light(Light::POINT_LIGHT, "pointLights[1]", { 0.3, 0.3, 0.4 }, { 0.7f, 0.5f, 0.5f }, { 0.85f, 0.85f, 0.85f });
	cam4->light.light.pointlight = PointLight_Fields(1, 0.072f, 0.0049f);
	cam4->light.light.apply_colors(basic3d);
	cam4->light.light.apply_type_fields(basic3d);
	cam4->light.light.apply_active(basic3d, true);
	cam4->sub(Event::EVENT_TOGGLE_HOUSE_LIGHT);
	cam4->add(Component::AUDIO);
	cam4->audio.play_3d("light_buzz.ogg", cam4->pos, true);

	Entity* cam5 = glo::entity->add(glm::vec3(-4.666153, 3.037676, -20.620951));
	cam5->ang = glm::vec3(180, -20, 0);
	cam5->add(Component::CAM);
	cam5->cam = CCam(CCam::FREE_STATIONARY, false);

	Entity* mini_screen = glo::entity->add(cam5->pos + glm::vec3(-0.5, -0.85, 0));
	mini_screen->ang.z = 0;
	mini_screen->add(Component::MODEL);
	mini_screen->model.mdl = mdl_mini_screen;
	mini_screen->model.shd = basic3d;
	mini_screen->add(Component::MINISCREEN);

	Entity* human = glo::entity->add(cam5->pos + glm::vec3(0.5, -2.0, -0));
	human->add(Component::MODEL);
	human->model.mdl = mdl_human;
	human->model.shd = basic3d;

	Entity* chair = glo::entity->add(cam5->pos + glm::vec3(0.5, -2.0, -0));
	chair->add(Component::MODEL);
	chair->model.mdl = mdl_chair;
	chair->model.shd = basic3d;

	glo::game->parking_spots[0].pos = glm::vec3(13.286233, 0, 0.556348);
	glo::game->parking_spots[1].pos = glm::vec3(13.286233, 0, 8.956244);
	glo::game->parking_spots[2].pos = glm::vec3(13.286233, 0, 17.510393);
	glo::game->parking_spots[3].pos = glm::vec3(5.080829, 0, 0.556348);
	glo::game->parking_spots[4].pos = glm::vec3(5.080829, 0, 8.956244);
	glo::game->parking_spots[5].pos = glm::vec3(5.080829, 0, 17.510393);
	int point_lights_count = 0;
	int spot_lights_count = 0;

	Hud hud(&mdl_2d, tex_map);
	Parking2D parking2d(&mdl_2d, &mdl_2d_circle, tex_pixel, tex_parking_2d);

	glo::game->set_cam(1);

	while (!glfwWindowShouldClose(glo::win->win)) {
		glfwPollEvents();
		
		if (glo::game->lock_cursor) {
			glfwSetInputMode(glo::win->win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode(glo::win->win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		glo::input->update(glo::win->win);

		if (cam5->ang.y < -54) {
			glo::game->open_cam();
		}

		human->model.visible = glo::game->_cam_index != 5;

		if (glo::input->press(GLFW_KEY_SPACE)) {
			bool is_in_camera_mode = glo::game->_cam_index >= 1 && glo::game->_cam_index <= 4;
			if (is_in_camera_mode) {
				glo::game->close_cam();
			}
			else {
				glo::game->open_cam();
			}
		}

		if (glo::input->press(GLFW_KEY_N)) {
			glo::win->cars_transparent_2d = true;
		}
		if (glo::input->press(GLFW_KEY_B)) {
			glo::win->cars_transparent_2d = false;
		}
		if (glo::input->press(GLFW_KEY_COMMA)) {
			glo::event->pub(Event::EVENT_TOGGLE_RAMP);
		}
		if (glo::input->press(GLFW_KEY_PERIOD)) {
			glo::event->pub(Event::EVENT_TOGGLE_HOUSE_LIGHT);
		}
		if (glo::input->press(GLFW_KEY_N)) {
			glo::game->set_cam(glo::game->_cam_index - 1);
		}
		if (glo::input->press(GLFW_KEY_M)) {
			glo::game->set_cam(glo::game->_cam_index + 1);
		}

		Input& input = *glo::input;
		for (int i = 0; i < 6; i++) {
			Entity* car = nullptr;
			for (Entity* e : glo::entity->arr) {
				if (e->has(Component::CAR) && e->car.spot_index == i) {
					car = e;
					break;
				}
			}

			if (input.press(GLFW_KEY_1 + i)) {
				if (input.down(GLFW_KEY_LEFT_CONTROL) && car != nullptr) {
					glo::entity->destroy(car);
				}
				else if (input.down(GLFW_KEY_LEFT_SHIFT) && car != nullptr) {
					car->car.time_left = 20 * 60;
				}
				else if (car == nullptr) {
					car = glo::entity->add({ -15.601791, 0, -41.797455 });
					car->add(Component::MODEL);
					car->model.mdl = mdl_car;
					car->model.shd = basic3d;
					car->add(Component::CAR);
					car->car.init(i);
				}
			}
		}

		float dx = (float)(input.down(GLFW_KEY_T) - input.down(GLFW_KEY_Y));
		float dy = (float)(input.down(GLFW_KEY_G) - input.down(GLFW_KEY_H));
		float dz = (float)(input.down(GLFW_KEY_B) - input.down(GLFW_KEY_N));
		house_window->pos += glm::vec3(dx, dy, dz);

		if (glo::input->press(GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(glo::win->win, GLFW_TRUE);
		}

		if (glo::input->press(GLFW_KEY_F2)) {
			glo::win->wireframe ^= true;
			int m = glo::win->wireframe ? GL_LINE : GL_FILL;
			glPolygonMode(GL_FRONT_AND_BACK, m);
		}

		if (glo::input->press(GLFW_KEY_F3)) {
			glo::win->shaded ^= true;
			basic3d->set_int("u_unlit", !glo::win->shaded);
		}

		Camera& cam = *glo::cam;
		cam.update_proj();
		glm::mat4 VP = glo::cam->proj * glo::cam->view();
		basic3d->set_mat4("VP", &VP[0][0]);
		cam_free->light.light.apply_position(basic3d, cam_free->pos);
		basic3d->set_vec3("viewPos", cam.pos.x, cam.pos.y, cam.pos.z);

		glo::audio->update_3d_listener(cam.pos, cam.pos + cam.front, cam.up);

		if (glo::input->press(GLFW_KEY_Z)) {
			for (int i = 0; i < glo::entity->size(); i++) {
				Entity* e = glo::entity->arr[i];
				printf("%d [%f %f %f]  [%f %f %f]\n", i, e->pos.x, e->pos.y, e->pos.z, e->ang.x, e->ang.y, e->ang.z);
			}
		}

		for (int i = 0; i < glo::entity->size(); i++) {
			Entity* e = glo::entity->arr[i];
			if (e->has(Component::RAMP)) {
				e->ramp.update(e);
			}
		}

		point_lights_count = 0;
		spot_lights_count = 0;
		for (int i = 0; i < glo::entity->size(); i++) {
			Entity* e = glo::entity->arr[i];
			if (e->has(Component::LIGHT)) {
				e->light.update_and_apply_position(e);

				if (e->light.light.type == Light::POINT_LIGHT) point_lights_count++;
				if (e->light.light.type == Light::SPOT_LIGHT) spot_lights_count++;
			}
		}
		basic3d->set_int("pointLights_count", point_lights_count);
		basic3d->set_int("spotLights_count", spot_lights_count);

		for (int i = 0; i < glo::entity->size(); i++) {
			Entity* e = glo::entity->arr[i];
			if (e->has(Component::CAM)) {
				e->cam.update(e);
				e->cam.update_lights(e, basic3d);
			}
		}

		for (int i = 0; i < glo::entity->size(); i++) {
			Entity* e = glo::entity->arr[i];
			if (e->has(Component::BUTTON)) {
				e->button.update(e);
			}
		}

		for (int i = 0; i < glo::entity->size(); i++) {
			Entity* e = glo::entity->arr[i];
			if (e->has(Component::CAR)) {
				e->car.update(e);
			}
		}

		for (int i = 0; i < glo::entity->size(); i++) {
			Entity* e = glo::entity->arr[i];
			if (e->has(Component::MINISCREEN)) {
				e->miniscreen.update(e);
			}
		}

		hud.update();

		for (int i = 0; i < glo::entity->size(); i++) {
			Entity* e = glo::entity->arr[i];
			e->check_ev();
		}
		glo::event->consume();
		glo::game->update_noise();

		glo::entity->purge_destroyed();

		if (hud.help_visible == 2) {
			for (int i = 0; i < GLFW_KEY_LAST; i++) {
				if (glo::input->down(i)) {
					hud.help_visible = 0;
				}
			}
			if (glo::input->mdown(GLFW_MOUSE_BUTTON_LEFT)) {
				hud.help_visible = 0;
			}
		}
		else {
			hud.help_visible = glo::input->down(GLFW_KEY_F1) ? 1 : 0;
		}

		//---------------------------------------------------------------------------------------------
		// Render
		//---------------------------------------------------------------------------------------------

		//---------------------------------------------------
		// Render 3D scene to the framebuffer.

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		{
			int w, h;
			glfwGetWindowSize(glo::win->win, &w, &h);
			glViewport(0, 0, w, h);
		}


		basic3d->set_int("u_noise_seed", glo::game->noise.rand);
		basic3d->set_float("u_noise_intensity", glo::game->noise.intensity);
		basic3d->set_float("u_noise_seizure", glo::game->noise.seizure);
	
		std::vector<Entity*> entities_sorted = glo::entity->arr;
		std::sort(entities_sorted.begin(), entities_sorted.end(), [&cam](Entity* a, Entity* b) {
			int at = a->model.transparent;
			int bt = b->model.transparent;
			if (at < bt) return true;
			float d1 = glm::length(cam.pos - a->pos);
			float d2 = glm::length(cam.pos - b->pos);
			if (at == bt && !at) return d1 < d2;
			if (at == bt && at) return d1 > d2;
			return false;
		});
		for (int i = 0; i < entities_sorted.size(); i++) {
			Entity* e = entities_sorted[i];
			if (e->has(Component::MODEL)) {
				e->model.draw(e);
			}
		}

		//---------------------------------------------------
		// Render 2D scene to fbo.

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
		glViewport(0, 0, 800, 600);
		glDisable(GL_DEPTH_TEST); // TODO: Don't do this, but prevent 2D objects from hiding one another.
		parking2d.draw(basic2d);

		//---------------------------------------------------
		// Render HUD, crosshair and watermakr to the framebuffer.

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		int w, h;
		glfwGetWindowSize(glo::win->win, &w, &h);
		glViewport(0, 0, w, h);
		glm::mat4 proj = glm::ortho((float)0, (float)w, (float)h, (float)0, -10.0f, 10.0f);

		hud.draw(basic2d);
		if (glo::game->lock_cursor) {
			basic2d->set_mat4("u_proj", &proj[0][0]);
			basic2d->set_vec2("u_pos", w / 2.0f, h / 2.0f);
			basic2d->set_vec2("u_scale", (float)tex_crosshair->w, (float)tex_crosshair->h);
			basic2d->set_vec3("u_img_frame", 1, 1, 0);
			mdl_2d.draw(basic2d, tex_crosshair);
		}

		{
			Texture* tex_watermark = glo::res->load_tex("tex_watermark.png");
			basic2d->set_vec2("u_pos", 0, (float)h - tex_watermark->h);
			basic2d->set_vec2("u_scale", (float)tex_watermark->w, (float)tex_watermark->h);
			basic2d->set_vec3("u_img_frame", 1, 1, 0);
			mdl_2d.draw(basic2d, tex_watermark);
		}

		if (hud.help_visible) {
			int ww = glm::min(w, (int)tex_help->w);
			int hh = glm::min(h, (int)tex_help->h);
			basic2d->set_vec2("u_pos", ((float)w - ww) / 2.0f, ((float)h - hh) / 2.0f);
			basic2d->set_vec2("u_scale", (float)ww, (float)hh);
			basic2d->set_vec3("u_img_frame", 1, 1, 0);
			mdl_2d.draw(basic2d, tex_help);
		}

		glfwSwapBuffers(glo::win->win);
	}

	return 0;
}