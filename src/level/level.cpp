#pragma once

#include "level.h"

#include "subsystem/subsystem_audio.h"
#include "subsystem/subsystem_camera.h"
#include "subsystem/subsystem_entity.h"
#include "subsystem/subsystem_event.h"
#include "subsystem/subsystem_game.h"
#include "subsystem/subsystem_input.h"
#include "subsystem/subsystem_resource.h"
#include "subsystem/subsystem_window.h"

#include "util/log.h"
#include "rend/rendtarget.h"

Entity* spawn_car(int car_index) {
	Model* mdl_car = glo::res->load_mdl("car.obj");
	Shader* basic3d = glo::res->load_shd("basic3d");
	Entity* car = glo::entity->add({ -15.601791, 0, -41.797455 });
	car->add(Component::MODEL);
	car->model.mdl = mdl_car;
	car->model.shd = basic3d;
	car->add(Component::CAR);
	car->car.init(car_index);
	return car;
}

void init_level(RendTarget* rt_screen_2d) {
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
	Model* mdl_camera = glo::res->load_mdl("camera.obj");
	Shader* basic3d = glo::res->load_shd("basic3d");
	Texture* tex_screen_framebuffer = new Texture(rt_screen_2d->texture, 800, 600);

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
	//cam1->add(Component::MODEL);
	//cam1->model.mdl = mdl_camera;
	//cam1->model.shd = basic3d;
	//cam1->model.transparent = true;

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
	//cam2->add(Component::MODEL);
	//cam2->model.mdl = mdl_camera;
	//cam2->model.shd = basic3d;
	//cam2->model.transparent = true;

	Entity* cam3 = glo::entity->add(glm::vec3(8.630467, 43.472813, 9.658076));
	cam3->ang = glm::vec3(00.0f, -89.000000, 0.000000);
	cam3->add(Component::CAM);
	cam3->cam = CCam(CCam::CCTV_TOPDOWN, false);
	//cam3->add(Component::MODEL);
	//cam3->model.mdl = mdl_camera;
	//cam3->model.shd = basic3d;
	//cam3->model.transparent = true;

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
	//cam4->add(Component::MODEL);
	//cam4->model.mdl = mdl_camera;
	//cam4->model.shd = basic3d;
	//cam4->model.transparent = true;

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
}