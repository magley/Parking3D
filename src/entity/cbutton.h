#pragma once
#include "event.h"

struct Entity;

struct CButton {
	bool pushed = false;

	Event ev;
	float radius = 1.0f;

	CButton();
	CButton(Event ev, bool pushed);
	
	void update(Entity* self);
	bool is_pushed_right_now(Entity* self) const;
	Entity* get_free_cam_if_active() const;
};