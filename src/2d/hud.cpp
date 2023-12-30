#include "hud.h"
#include "global.h"
#include <glm/gtc/matrix_transform.hpp>

Hud::Hud(Mesh2D* mesh2d, Texture* map_tex) {
	mesh_2d = mesh2d;
	map.tex = map_tex;
	init();
}

void Hud::init() {
	map.cam_buttons.push_back(BBox({ 19 * scale, 324 * scale }, { 110 * scale, 66 * scale }));
	map.cam_buttons.push_back(BBox({ 364 * scale, 36 * scale }, { 100 * scale, 65 * scale }));
	map.cam_buttons.push_back(BBox({ 203 * scale, 153 * scale }, { 107 * scale, 73 * scale }));
	map.cam_buttons.push_back(BBox({ 171 * scale, 417 * scale }, { 98 * scale, 58 * scale }));
}

void Hud::update() {
	int w, h;
	glfwGetWindowSize(glo::wctx.win, &w, &h);
	map.pos.x = w - (map.tex->w * scale + 64);
	map.pos.y = h - (map.tex->h * scale + 64);

	Input& input = glo::wctx.input;

	if (input.mpress(GLFW_MOUSE_BUTTON_LEFT)) {
		for (int i = 0; i < map.cam_buttons.size(); i++) {
			BBox bbox = map.cam_buttons[i] + map.pos;
			if (bbox.contains(input.mp_curr)) {
				glo::game.set_cam(i + 1); // 0 is debug free camera.
				map.button_flash_timer = 0; // Looks better if it's green when you click it.
			}
		}
	}

	map.button_flash_timer++;
	if (map.button_flash_timer > 70) {
		map.button_flash_timer = 0;
	}
}

void Hud::draw(Shader* shd) {
	int w, h;
	glfwGetWindowSize(glo::wctx.win, &w, &h);
	glfwGetWindowSize(glo::wctx.win, &w, &h);
	glm::mat4 proj = glm::ortho((float)0, (float)w, -(float)h, (float)0, -100.0f, 100.0f);

	float hud_x = w - (map.tex->w * scale + 64);
	float hud_y = h - (map.tex->h * scale + 64);

	int frame = (map.button_flash_timer / 35 == 1) ? 0 : glo::game._cam_index;

	shd->set_mat4("u_proj", &proj[0][0]);
	shd->set_vec2("u_pos", hud_x, -hud_y);
	shd->set_vec2("u_scale", map.tex->w * scale, map.tex->h * scale);
	shd->set_vec3("u_img_frame", map.tex->w / (float)map.tex->fullw, map.tex->h / (float)map.tex->fullh, frame);
	shd->set_vec4("u_color", 1, 1, 1, 1);
	mesh_2d->draw(shd, map.tex);
}