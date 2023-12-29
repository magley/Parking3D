#include "cbutton.h"
#include "global.h"

CButton::CButton() {}

CButton::CButton(Event ev, bool pushed):
	ev(ev),
	pushed(pushed) {}

void CButton::update(Entity* self) {
	bool is_pushed = is_pushed_right_now(self);

	if (!is_pushed) {
		return;
	}

	glo::wctx.event.pub(ev);
}

bool CButton::is_pushed_right_now(Entity* self) const {
	if (!glo::wctx.input.mpress(GLFW_MOUSE_BUTTON_LEFT)) {
		return false;
	}

	Entity* free_cam = get_free_cam_if_active();
	if (free_cam == nullptr) {
		return false;
	}

	glm::vec3 A = free_cam->pos;
	glm::vec3 B = A + free_cam->cam.get_front();
	glm::vec3 P = self->pos;

	glm::vec3 AB = glm::normalize(B - A);
	glm::vec3 AP = P - A;
	glm::vec3 Pproj = A + glm::dot(AB, AP) * AB;

	float dist = glm::distance(P, Pproj);
	if (dist <= radius) {
		return true;
	}
	return false;
}

Entity* CButton::get_free_cam_if_active() const {
	for (int i = 0; i < glo::wctx.entity.arr.size(); i++) {
		Entity* e = glo::wctx.entity.arr[i];

		if (e->has(Component::CAM) && e->cam.type == CCam::FREE && e->cam.active) {
			return e;
		}
	}
	return nullptr;
}