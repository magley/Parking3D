#include "game.h"

#include "subsystem/subsystem_audio.h"
#include "subsystem/subsystem_entity.h"
#include "subsystem/subsystem_resource.h"
#include "subsystem/subsystem_window.h"
#include "subsystem/subsystem_input.h"
#include "subsystem/subsystem_event.h"
#include "level/level.h"

void Game::setup_cam_indices(int free_cam_index, int down_cam_index) {
	this->free_cam_index = free_cam_index;
	this->down_cam_index = down_cam_index;
}

std::vector<Entity*> Game::get_camera_entities() {
	std::vector<Entity*> entities;
	for (Entity* e : glo::entity->arr) {
		if (e->has(Component::CAM)) {
			entities.push_back(e);
		}
	}

	return entities;
}

void Game::set_cam(int index) {
	int prev_cam_index = curr_cam_index;
	const auto cam_entities = get_camera_entities();

	for (int i = 0; i < cam_entities.size(); i++) {
		Entity* e = cam_entities[i];
		e->cam.active = (i == index);
	}

	curr_cam_index = index;

	// Toggle cursor lock.

	Entity* e = cam_entities[curr_cam_index];
	if (e->cam.active == true) {
		lock_cursor = e->cam.type == CCam::FREE || e->cam.type == CCam::FREE_STATIONARY;
	}

	// Effects

	if (is_cctv_cam(curr_cam_index)) {
		noise.seizure_min = 0;
		noise.intensity = 2.0;

		WavSample* snd_cam_switch = glo::res->load_wav("cam_switch.wav");
		glo::audio->play(snd_cam_switch);
		if (!is_cctv_cam(prev_cam_index)) {
			open_cam();
		}
	}
	else {
		noise.intensity = 0;
		if (is_cctv_cam(prev_cam_index)) {
			close_cam();
		}
	}
}

void Game::next_cam(bool previous) {
	const auto cam_entities = get_camera_entities();
	const int k = previous ? -1 : 1;
	int new_index = curr_cam_index;

	while (true) {
		new_index = (new_index + k + cam_entities.size()) % cam_entities.size();

		if (!allow_fun_cams) {
			if (new_index == free_cam_index || new_index == down_cam_index) {
				continue;
			}
		}
		break;
	}

	set_cam(new_index);
}

bool Game::is_cctv_cam(int index) {
	return index != free_cam_index && index != down_cam_index;
}

void Game::update_noise() {
	if (noise.seizure_min < 50) {
		noise.seizure_min += 1;
	}

	if (noise.intensity > 0.2) {
		noise.intensity -= 0.04f;
	}

	noise.rand = rand() / 9;
	noise.seizure = (rand() % (noise.seizure_min - noise.seizure_max)) + (float)noise.seizure_min;
}

void Game::open_cam() {
	Entity* mini_screen = nullptr;
	for (Entity* e : glo::entity->arr) {
		if (e->has(Component::MINISCREEN)) {
			mini_screen = e;
			break;
		}
	}

	if (mini_screen == nullptr) {
		return;
	}

	mini_screen->miniscreen.raise(mini_screen);
}

void Game::close_cam() {
	Entity* mini_screen = nullptr;
	for (Entity* e : glo::entity->arr) {
		if (e->has(Component::MINISCREEN)) {
			mini_screen = e;
			break;
		}
	}

	if (mini_screen == nullptr) {
		return;
	}

	mini_screen->miniscreen.lower(mini_screen);
}

void Game::update_mouse_freedom() {
	if (lock_cursor) {
		glfwSetInputMode(glo::win->win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else {
		glfwSetInputMode(glo::win->win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Game::update_game_inputs() {
	if (glo::input->press(GLFW_KEY_SPACE) && allow_fun_cams) {
		if (is_cctv_cam(curr_cam_index)) {
			close_cam();
		}
		else if (curr_cam_index == down_cam_index) {
			open_cam();
		}
	}
	if (glo::input->press(GLFW_KEY_BACKSPACE)) {
		allow_fun_cams ^= true;
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

	if (glo::input->press(GLFW_KEY_SEMICOLON)) {
		next_cam(true);
	}

	if (glo::input->press(GLFW_KEY_APOSTROPHE)) {
		next_cam(false);
	}

	if (glo::input->press(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(glo::win->win, GLFW_TRUE);
	}

	if (glo::input->press(GLFW_KEY_F2)) {
		glo::win->wireframe ^= true;
		int m = glo::win->wireframe ? GL_LINE : GL_FILL;
		glPolygonMode(GL_FRONT_AND_BACK, m);
	}

	Shader* basic3d = glo::res->load_shd("basic3d");
	if (glo::input->press(GLFW_KEY_F3)) {
		glo::win->shaded ^= true;
		basic3d->set_int("u_unlit", !glo::win->shaded);
	}
}

void Game::update_car_spawn() {
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
				car = spawn_car(i);
			}
		}
	}

}