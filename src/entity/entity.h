#pragma once
#include "glm/glm.hpp"
#include "cmodel.h"
#include "cramp.h"
#include "ccam.h"
#include "clight.h"
#include "event.h"
#include "cbutton.h"
#include "ccar.h"
#include "caudio.h"
#include "cminiscreen.h"

enum Component {
	NONE		= 0,
	MODEL		= 1,
	RAMP		= 2,
	CAM			= 4,
	LIGHT		= 8,
	BUTTON		= 16,
	CAR			= 32,
	AUDIO		= 64,
	MINISCREEN	= 128
};

struct Entity {
	glm::vec3 pos;
	glm::vec3 ang;

	unsigned c = Component::NONE;
	bool destroyed = false;

	void add(Component component) { c |= component; }
	bool has(Component component) { return (c & component) == component;  }

	CModel model;
	CRamp ramp;
	CCam cam;
	CLight light;
	CButton button;
	CCar car;
	CAudio audio;
	CMiniScreen miniscreen;

	bool event_sub[EVENT_size] = { false };
	void sub(Event ev) { event_sub[ev] = true; }
	void unsub(Event ev) { event_sub[ev] = false; }
	void check_ev();
	void on_event(Event ev);

	void update();
};