#include "hud.h"
#include <glm/gtc/matrix_transform.hpp>

#include "subsystem/subsystem_entity.h"
#include "subsystem/subsystem_game.h"
#include "subsystem/subsystem_input.h"
#include "subsystem/subsystem_resource.h"
#include "subsystem/subsystem_window.h"

Hud::Hud(Mesh2D* mesh2d, Texture* map_tex) {
	mesh_2d = mesh2d;
	map.tex = map_tex;
	scale = 0.75;
	init();
}

void Hud::init() {
	map.cam_buttons.push_back(CamButton(BBox({ 19 * scale, 324 * scale  }, { 110 * scale, 66 * scale }), 1));
	map.cam_buttons.push_back(CamButton(BBox({ 364 * scale, 36 * scale  }, { 100 * scale, 65 * scale }), 2));
	map.cam_buttons.push_back(CamButton(BBox({ 203 * scale, 153 * scale }, { 107 * scale, 73 * scale }), 3));
	map.cam_buttons.push_back(CamButton(BBox({ 171 * scale, 417 * scale }, { 98 * scale, 58 * scale  }), 4));
}

void Hud::update() {
	int w, h;
	glfwGetWindowSize(glo::win->win, &w, &h);
	map.pos.x = w - (map.tex->w * scale + 64);
	map.pos.y = h - (map.tex->h * scale + 64);

	Input& input = *glo::input;

	if (input.mpress(GLFW_MOUSE_BUTTON_LEFT)) {
		if (glo::game->is_cctv_cam(glo::game->curr_cam_index)) {
			for (int i = 0; i < map.cam_buttons.size(); i++) {
				BBox bbox = map.cam_buttons[i].bbox + map.pos;
				if (bbox.contains(input.mp_curr)) {
					glo::game->set_cam(map.cam_buttons[i].cam_index);
					map.button_flash_timer = 0;
				}
			}

			if (glo::game->allow_fun_cams && glo::game->is_cctv_cam(glo::game->curr_cam_index)) {
				Texture* tex_cam_hud = glo::res->load_tex("tex_cam_hud.png");
				float cam_toggle_x = (w - tex_cam_hud->w) / 2.0f;
				float cam_toggle_y = (h - tex_cam_hud->h - 32.0f);

				BBox bbox = BBox({ cam_toggle_x , cam_toggle_y }, { tex_cam_hud->w, tex_cam_hud->h });
				if (bbox.contains(input.mp_curr)) {
					glo::game->close_cam();
				}
			}
		}
	}

	map.button_flash_timer++;
	if (map.button_flash_timer > 70) {
		map.button_flash_timer = 0;
	}
}

void Hud::draw(Shader* shd) {
	if (!glo::game->is_cctv_cam(glo::game->curr_cam_index)) {
		return;
	}
	
	int w, h;
	glfwGetWindowSize(glo::win->win, &w, &h);
	glfwGetWindowSize(glo::win->win, &w, &h);
	glm::mat4 proj = glm::ortho((float)0, (float)w, (float)h, (float)0, -10.0f, 10.0f);

	float hud_x = w - (map.tex->w * scale + 64);
	float hud_y = h - (map.tex->h * scale + 64);
	int frame = (map.button_flash_timer / 35 == 1) ? 0 : glo::game->curr_cam_index;

	shd->set_mat4("u_proj", &proj[0][0]);
	shd->set_vec2("u_pos", hud_x, hud_y);
	shd->set_vec2("u_scale", map.tex->w * scale, map.tex->h * scale);
	shd->set_vec3("u_img_frame", map.tex->w / (float)map.tex->fullw, map.tex->h / (float)map.tex->fullh, (float)frame);
	shd->set_vec4("u_color", 1, 1, 1, 1);
	mesh_2d->draw(shd, map.tex);

	Texture* tex_cam_hud = glo::res->load_tex("tex_cam_hud.png");
	float cam_toggle_x = (w - tex_cam_hud->w) / 2.0f;
	float cam_toggle_y = (h - tex_cam_hud->h - 32.0f);
	shd->set_mat4("u_proj", &proj[0][0]);
	shd->set_vec2("u_pos", cam_toggle_x, cam_toggle_y);
	shd->set_vec2("u_scale", tex_cam_hud->w * scale, tex_cam_hud->h * scale);
	shd->set_vec3("u_img_frame", 1, 1, 0);
	shd->set_vec4("u_color", 1, 1, 1, 1);
	mesh_2d->draw(shd, tex_cam_hud);
}