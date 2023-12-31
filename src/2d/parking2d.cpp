#include "parking2d.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "rend/color.h"

#include "subsystem/subsystem_entity.h"
#include "subsystem/subsystem_window.h"

Entity* get_car(int spot_index) {
	Entity* car = nullptr;
	for (Entity* e : glo::entity->arr) {
		if (e->has(Component::CAR) && e->car.spot_index == spot_index) {
			car = e;
			break;
		}
	}
	return car;
}

void draw_circle(float x, float y, float r, Color color, Mesh2D* mesh, Shader* shd, Texture* tex_pixel) {
	shd->set_vec2("u_pos", x, y);
	shd->set_vec2("u_scale", r, r);
	shd->set_vec3("u_img_frame", 1, 1, 0);
	shd->set_vec4("u_color", color.r, color.g, color.b, color.a);
	mesh->draw(shd, tex_pixel);
}

void draw_rect(float x, float y, float w, float h, Color color, Mesh2D* mesh, Shader* shd, Texture* tex_pixel) {
	shd->set_vec2("u_pos", x, y);
	shd->set_vec2("u_scale", w, h);
	shd->set_vec3("u_img_frame", 1, 1, 0);
	shd->set_vec4("u_color", color.r, color.g, color.b, color.a);
	mesh->draw(shd, tex_pixel);
}

Parking2D::Parking2D(Mesh2D* mesh_2d, Mesh2D* mesh_circle, Texture* tex_pixel, Texture* tex_parking_2d) :
	mesh_2d(mesh_2d),
	mesh_circle(mesh_circle),
	tex_pixel(tex_pixel),
	tex_parking_2d(tex_parking_2d) {
}

void Parking2D::draw(Shader* shd) {
	float base_x = 300;
	float base_y = 100;

	glm::mat4 proj = glm::ortho((float)0, (float)800, (float)600, (float)0, -10.0f, 10.0f);
	shd->set_mat4("u_proj", &proj[0][0]);
	shd->set_vec2("u_pos", base_x, base_y);
	shd->set_vec2("u_scale", (float)tex_parking_2d->w, (float)tex_parking_2d->h);
	shd->set_vec3("u_img_frame", 1, 1, 0);
	shd->set_vec4("u_color", 1, 1, 1, 1);
	mesh_2d->draw(shd, tex_parking_2d);

	for (int i = 0; i < 6; i++) {
		Entity* car = get_car(i);
		float rect_w = 60;
		float rect_h = 120;
		float rect_x = base_x + (i % 3) * (rect_w * 2);
		float rect_y = base_y + (i / 3) * (rect_h * 2) + 15;
		float circle_r = 12;
		float circle_x = rect_x + (rect_w / 2) + (circle_r / 2);
		float circle_y = rect_y + rect_h + 28;

		if (car != nullptr) {
			float progress = car->car.time_left / (20 * 60.0f); // TODO: Magic constant.

			Color car_rect_color = car->car.color;
			car_rect_color.a = glo::win->cars_transparent_2d ? 0.5f : 1.0f;

			draw_rect(rect_x, rect_y, rect_w, rect_h, car_rect_color, mesh_2d, shd, tex_pixel);
			draw_rect(rect_x, rect_y - 16, rect_w * progress, 12, Color(0, 1, 0.45f), mesh_2d, shd, tex_pixel);
			draw_circle(circle_x, circle_y, circle_r, Color(1, 0, 0), mesh_circle, shd, tex_pixel);
		} else {
			draw_circle(circle_x, circle_y, circle_r, Color(0, 1, 0), mesh_circle, shd, tex_pixel);
		}
	}

	shd->set_vec4("u_color", 1, 1, 1, 1);
}