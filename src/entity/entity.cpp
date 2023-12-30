#include "entity.h"
#include "global.h"

void Entity::check_ev() {
	EventMng& evmng = glo::wctx.event;
	for (int i = 0; i < EVENT_size; i++) {
		if (evmng.active[i] && event_sub[i]) {
			on_event((Event)i);
		}
	}
}

void Entity::on_event(Event ev) {
	switch (ev) {
	case EVENT_TOGGLE_HOUSE_LIGHT:
		light.set_active(!light._active);
		audio.pause(!light._active);
		return;
	case EVENT_TOGGLE_RAMP:
		ramp.toggle();
		return;
	}
}