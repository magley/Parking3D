#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp>
#include <soloud.h>

#include "level/level.h"
#include "util/log.h"
#include "rend/rendtarget.h"
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

int main(int argc, char** argv) {
	//---------------------------------------------------------------------------------------------
	// Initialize
	//---------------------------------------------------------------------------------------------

	log("Initializing engine subsystems...");
	glo::win = new WinCtx();
	glo::audio = new AudioCore();
	glo::cam = new Camera();
	glo::entity = new EntityMng();
	glo::event = new EventMng();
	glo::game = new Game();
	glo::input = new Input();
	glo::res = new ResMng();

	log("Creating the window...");
	glfwInit();
	glo::win->win = glfwCreateWindow(1280, 720, "Parking3D", nullptr, nullptr);
	glfwMakeContextCurrent(glo::win->win);
	glfwSwapInterval(1);
	log("Initializing the OpenGL context...");
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	RendTarget rt_default = RendTarget::init_default_rend_target();
	RendTarget rt_screen_2d = RendTarget(800, 600);

	int point_lights_count = 0;
	int spot_lights_count = 0;

	Mesh2D mdl_2d(Mesh2D::SQUARE);
	Mesh2D mdl_2d_circle(Mesh2D::CIRCLE);
	Texture* tex_map = glo::res->load_tex("tex_map.png", 5);
	Texture* tex_pixel = glo::res->load_tex("tex_pixel.png");
	Texture* tex_parking_2d = glo::res->load_tex("tex_parking_2d.png");
	Shader* basic2d = glo::res->load_shd("basic2d");
	Hud hud(&mdl_2d, tex_map);
	Parking2D parking2d(&mdl_2d, &mdl_2d_circle, tex_pixel, tex_parking_2d);

	init_level(&rt_screen_2d);

	glo::game->set_cam(1);
	glo::game->setup_cam_indices(0, 5);


	while (!glfwWindowShouldClose(glo::win->win)) {
		//---------------------------------------------------------------------------------------------
		// Update
		//---------------------------------------------------------------------------------------------

		//---------------------------------------------------
		// Pre-update.

		glfwPollEvents();
		int win_w, win_h;
		glo::win->get_size(&win_w, &win_h);
		glo::game->update_mouse_freedom();
		glo::input->update(glo::win->win);
		glo::game->update_game_inputs();
		glo::game->update_car_spawn();
		glo::game->update_noise();
		
		Camera& cam = *glo::cam;
		cam.update_proj();
		glm::mat4 VP = glo::cam->proj * glo::cam->view();
		Shader* basic3d = glo::res->load_shd("basic3d");
		basic3d->set_mat4("VP", &VP[0][0]);
		basic3d->set_vec3("viewPos", cam.pos.x, cam.pos.y, cam.pos.z);
		glo::audio->update_3d_listener(cam.pos, cam.pos + cam.front, cam.up);

		point_lights_count = 0;
		spot_lights_count = 0;

		//---------------------------------------------------
		// Update entities.

		for (int i = 0; i < glo::entity->size(); i++) {
			Entity* e = glo::entity->arr[i];
			e->update();

			if (e->has(Component::LIGHT)) {
				if (e->light.light.type == Light::POINT_LIGHT) point_lights_count++;
				if (e->light.light.type == Light::SPOT_LIGHT) spot_lights_count++;
			}
		}

		//---------------------------------------------------
		// Post-update.

		hud.update();

		glo::event->consume();
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

		basic3d->set_int("pointLights_count", point_lights_count);
		basic3d->set_int("spotLights_count", spot_lights_count);

		//---------------------------------------------------
		// Render 3D scene.

		rt_default.resize(win_w, win_h);
		rt_default.make_current();
		rt_default.clear(0, 0, 0);
		rt_default.set_depth_testing(true);

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
		// Render 2D scene.

		rt_screen_2d.make_current();
		rt_screen_2d.clear(0, 0, 0);
		rt_screen_2d.set_depth_testing(false);
		parking2d.draw(basic2d);

		//---------------------------------------------------
		// Render HUD, crosshair and watermark.

		rt_default.make_current();
		glm::mat4 proj = glm::ortho((float)0, (float)win_w, (float)win_h, (float)0, -10.0f, 10.0f);
		hud.draw(basic2d);

		if (glo::game->lock_cursor) {
			Texture* tex_crosshair = glo::res->load_tex("tex_crosshair.png");		
			basic2d->set_mat4("u_proj", &proj[0][0]);
			basic2d->set_vec2("u_pos", win_w / 2.0f, win_h / 2.0f);
			basic2d->set_vec2("u_scale", (float)tex_crosshair->w, (float)tex_crosshair->h);
			basic2d->set_vec3("u_img_frame", 1, 1, 0);
			mdl_2d.draw(basic2d, tex_crosshair);
		}

		{
			Texture* tex_watermark = glo::res->load_tex("tex_watermark.png");
			basic2d->set_vec2("u_pos", 0, (float)win_h - tex_watermark->h);
			basic2d->set_vec2("u_scale", (float)tex_watermark->w, (float)tex_watermark->h);
			basic2d->set_vec3("u_img_frame", 1, 1, 0);
			mdl_2d.draw(basic2d, tex_watermark);
		}

		if (hud.help_visible) {
			Texture* tex_help = glo::res->load_tex("tex_help.png");
			int ww = glm::min(win_w, (int)tex_help->w);
			int hh = glm::min(win_h, (int)tex_help->h);
			basic2d->set_vec2("u_pos", ((float)win_w - ww) / 2.0f, ((float)win_h - hh) / 2.0f);
			basic2d->set_vec2("u_scale", (float)ww, (float)hh);
			basic2d->set_vec3("u_img_frame", 1, 1, 0);
			mdl_2d.draw(basic2d, tex_help);
		}

		glfwSwapBuffers(glo::win->win);
	}

	return 0;
}