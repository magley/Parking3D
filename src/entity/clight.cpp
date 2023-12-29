#include "clight.h"
#include "entity.h"

void CLight::update_and_apply_position(Entity* self) {
	light.apply_position(shd, self->pos);
}

void CLight::set_active(bool active) {
	_active = active;
	light.apply_active(shd, _active);
}