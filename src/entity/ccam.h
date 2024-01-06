#pragma once

#include "glm/glm.hpp"

struct Entity;
struct Shader;

struct CCam {
	enum Type {
		CCTV_SCANNER,
		CCTV_TOPDOWN,
		CCTV_STATIONARY,
		FREE_STATIONARY,
		FREE
	};
	
	Type type = Type::CCTV_SCANNER;
	bool active = false;

	CCam() {}
	CCam(Type type, bool active) : type(type), active(active) {}
	void update(Entity* self);
	void update_lights(Entity* self, Shader* shd);
	glm::vec3 get_front() const;
	int timer = 0;
private:
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 front = glm::vec3(0.0, 0.0, -1.0);

	float sway_root_ang = 0;
	float sway = 0;
	int sway_dir = 1;

	float pitch = 0;
	float yaw = 0;

	void update_pitch_yaw_from_ang(Entity* self);
	void update_front_from_pitch_yaw(Entity* self);

	void _update_cctv_scanner(Entity* self);
	void _update_cctv_topdown(Entity* self);
	void _update_cctv_stationary(Entity* self);
	void _update_stationary(Entity* self);
	void _update_free(Entity* self);
};
