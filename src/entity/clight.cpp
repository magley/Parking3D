#include "clight.h"
#include "entity.h"

void CLight::update_and_apply_position(Entity* self, Shader* shd) {
	light.apply_position(shd, self->pos);
}