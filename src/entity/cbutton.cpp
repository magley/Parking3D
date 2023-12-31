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
	glo::wctx.audio.play(glo::wctx.resmng.load_wav("button1.wav"));
}

glm::vec3 screen_coords_raycast(double xpos, double ypos, glm::mat4 view, glm::mat4 projection) {
	int winw, winh;
	glfwGetWindowSize(glo::wctx.win, &winw, &winh);
	float x = (2.0f * xpos) / winw - 1.0f;
	float y = 1.0f - (2.0f * ypos) / winh;
	float z = 1.0f;
	glm::vec3 ray_nds = glm::vec3(x, y, z);
	glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, ray_nds.z, 1.0f);
	glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, ray_eye.z, 0.0f);
	glm::vec4 inv_ray_wor = (glm::inverse(view) * ray_eye);
	glm::vec3 ray_wor = glm::vec3(inv_ray_wor.x, inv_ray_wor.y, inv_ray_wor.z);
	ray_wor = glm::normalize(ray_wor);
	return ray_wor;
}

bool CButton::is_pushed_right_now(Entity* self) const {
	if (!glo::wctx.input.mpress(GLFW_MOUSE_BUTTON_LEFT)) {
		return false;
	}

	Entity* free_cam = get_free_cam_if_active();
	if (free_cam == nullptr) {
		return false;
	}

	glm::vec3 front = free_cam->cam.get_front();
	if (free_cam->cam.type != CCam::FREE && free_cam->cam.type != CCam::FREE_STATIONARY) {
		glm::vec3 mcurr = glm::vec3(glo::wctx.input.mp_curr, 1);
		front = screen_coords_raycast(mcurr.x, mcurr.y, glo::wctx.cam.view(), glo::wctx.cam.proj);
	}

	glm::vec3 A = free_cam->pos;
	glm::vec3 B = A + front;
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
	// TODO: Rename func.
	for (int i = 0; i < glo::wctx.entity.arr.size(); i++) {
		Entity* e = glo::wctx.entity.arr[i];

		if (e->has(Component::CAM) && e->cam.active) {
			return e;
		}
	}
	return nullptr;
}