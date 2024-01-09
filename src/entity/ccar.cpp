#include "ccar.h"

#include "subsystem/subsystem_entity.h"
#include "subsystem/subsystem_game.h"

static Entity* _get_ramp() {
	Entity* ramp = nullptr;
	for (Entity* e : glo::entity->arr) {
		if (e->has(Component::RAMP)) {
			ramp = e;
			break;
		}
	}

	return ramp;
}

static std::vector<Entity*> get_cars() {
	std::vector<Entity*> car_entities;
	for (Entity* e : glo::entity->arr) {
		if (e->has(Component::CAR)) {
			car_entities.push_back(e);
		}
	}
	return car_entities;
}

void CCar::init(int spot) {
	spot_index = spot;
	state = State::TOWARD_RAMP;

	Color_HSV crand = Color_HSV(
		rand() % 360,
		0.8,
		1.0
	);
	color = crand.to_color();


	Entity* ramp = _get_ramp();
	if (ramp != nullptr) {
		path.current = 0;
		path.p.clear();
		path.p.push_back(ramp->pos + glm::vec3(-4, 0, -3.5));
	}
	else {
		printf("No ramp detected. Cannot initialize CCar!");
		exit(1);
	}
}

void CCar::update(Entity* self) {
	switch (state) {
	case TOWARD_RAMP:
		_update_toward_ramp(self);
		break;
	case AT_RAMP:
		_update_at_ramp(self);
		break;
	case TOWARD_SPOT:
		_update_toward_spot(self);
		break;
	case AT_SPOT:
		_update_at_spot(self);
		break;
	}
}

void CCar::_update_toward_ramp(Entity* self) {
	_follow_path(self);
	if (path.current >= path.p.size()) {
		state = State::AT_RAMP;
	}
}

void CCar::_update_at_ramp(Entity* self) {
	Entity* ramp = _get_ramp();
	if (ramp == nullptr) {
		return;
	}

	if (ramp->ramp.all_the_way_up) {
		state = State::TOWARD_SPOT;
		_set_path(self);
	}
}

void CCar::_set_path(Entity* self) {
	path.p.clear();
	path.current = 0;

	switch (spot_index) {
	case 0:
		path.p.push_back(glm::vec3(-14, self->pos.y, -7));
		path.p.push_back(glm::vec3(20, self->pos.y, -7));
		path.p.push_back(glm::vec3(20, self->pos.y, 0.8));
		path.p.push_back(glm::vec3(13, self->pos.y, 0.8));
		break;
	case 1:
		path.p.push_back(glm::vec3(-14, self->pos.y, -7));
		path.p.push_back(glm::vec3(20, self->pos.y, -7));
		path.p.push_back(glm::vec3(20, self->pos.y, 9.6));
		path.p.push_back(glm::vec3(13, self->pos.y, 9.6));
		break;
	case 2:
		path.p.push_back(glm::vec3(-14, self->pos.y, -7));
		path.p.push_back(glm::vec3(20, self->pos.y, -7));
		path.p.push_back(glm::vec3(20, self->pos.y, 18.4));
		path.p.push_back(glm::vec3(13, self->pos.y, 18.4));
		break;
	case 3:
		path.p.push_back(glm::vec3(-14, self->pos.y, -7));
		path.p.push_back(glm::vec3(-14, self->pos.y, 0.5));
		path.p.push_back(glm::vec3(5, self->pos.y, 0.5));
		break;
	case 4:
		path.p.push_back(glm::vec3(-14, self->pos.y, -7));
		path.p.push_back(glm::vec3(-14, self->pos.y, 9));
		path.p.push_back(glm::vec3(5, self->pos.y, 9));
		break;
	case 5:
		path.p.push_back(glm::vec3(-14, self->pos.y, -7));
		path.p.push_back(glm::vec3(-14, self->pos.y, 17.5));
		path.p.push_back(glm::vec3(5, self->pos.y, 17.5));
	default:
		break;
	}
}

void CCar::_follow_path(Entity* self) {
	if (path.current >= path.p.size()) {
		return;
	}

	const glm::vec3& target = path.p[path.current];
	float spd = 0.2f;

	// If there is a car "in front" of you, don't move.
	// Spheres are used as colliders (good enough).

	const float the_cos = glm::cos(glm::radians(self->ang.y));
	const float the_sin = glm::sin(glm::radians(self->ang.y));
	const glm::vec3 forward = { the_sin, 0, the_cos };
	const float cld_dist = 3.45;
	const glm::vec3 cld_pos = self->pos - (forward * cld_dist);
	const float radius = 2;
	for (Entity* car : get_cars()) {
		if (car == self) {
			continue;
		}

		if (glm::distance(cld_pos, car->pos) <= 2 * radius) {
			// If A waits for B, then B mustn't wait for A. So
			// if our "collision" determines that B *should*
			// wait for A, check if A's waiting chain contains
			// B. If it does, than A is (indirectly) waiting for
			// B, so B will not wait for A.

			bool should_wait_for_this_guy = true;
			Entity* e = car->car.waiting_for_car;
			while (e != nullptr) {
				if (e == self) {
					should_wait_for_this_guy = false;
					break;
				}

				e = e->car.waiting_for_car;
			}

			if (should_wait_for_this_guy) {
				waiting_for_car = car;
				return;
			}
		}
	}
	waiting_for_car = nullptr;

	// Follow path and rotate.

	if (self->pos.x < target.x) self->pos.x += spd;
	if (self->pos.x > target.x) self->pos.x -= spd;
	if (self->pos.y < target.y) self->pos.y += spd;
	if (self->pos.y > target.y) self->pos.y -= spd;
	if (self->pos.z < target.z) self->pos.z += spd;
	if (self->pos.z > target.z) self->pos.z -= spd;

	if (glm::length(self->pos - target) <= spd * 2) {
		path.current++;
	}

	// Rotate car to face the movement direction.

	float arctan = atan2f(target.z - self->pos.z, -(target.x - self->pos.x));
	float deg = glm::degrees(arctan);
	float desired_ang_y = deg + 90;
	self->ang.y = desired_ang_y; // TODO: Interpolate angles
}

void CCar::_update_toward_spot(Entity* self) {
	_follow_path(self);
	if (path.current >= path.p.size()) {
		state = State::AT_SPOT;
		time_left = 20 * 60;
	}
}

void CCar::_update_at_spot(Entity* self) {
	time_left--;
	if (time_left == 0) {
		glo::entity->destroy(self);
	}
}