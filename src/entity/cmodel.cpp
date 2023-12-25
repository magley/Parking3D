#include "cmodel.h"
#include "entity.h"
#include <glm/gtc/matrix_transform.hpp>

void CModel::draw(Entity* self) {
	glm::mat4 model = glm::mat4(1.0);

	model = glm::translate(model, self->pos); // 'axis' in glm::rotate() is in model-space so that's why we translate first.
	model = glm::rotate(model, glm::radians(self->ang.x), glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(self->ang.y), glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, glm::radians(self->ang.z), glm::vec3(0.0, 0.0, 1.0));

	shd->set_mat4("M", &model[0][0]);
	mdl->draw(shd);
}