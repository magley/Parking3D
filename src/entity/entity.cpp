#include "entity.h"

#include "subsystem/subsystem_event.h"
#include "subsystem/subsystem_resource.h"

void Entity::check_ev() {
	EventMng& evmng = *glo::event;
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

void Entity::update() {
	if (has(Component::RAMP)) {
		ramp.update(this);
	}
	if (has(Component::LIGHT)) {
		light.update_and_apply_position(this);

	}
	if (has(Component::CAM)) {
		cam.update(this);

		Shader* shd = glo::res->load_shd("basic3d");
		cam.update_lights(this, shd);
	}
	if (has(Component::BUTTON)) {
		button.update(this);
	}
	if (has(Component::CAR)) {
		car.update(this);
	}
	if (has(Component::MINISCREEN)) {
		miniscreen.update(this);
	}

	check_ev();
}