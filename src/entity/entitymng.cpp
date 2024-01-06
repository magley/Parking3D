#include "entitymng.h"

namespace glo {
	EntityMng* entity = nullptr;
}

Entity* EntityMng::add(glm::vec3 pos) {
	Entity* e = new Entity();
	e->pos = pos;
	arr.push_back(e);
	return e;
}

void EntityMng::clear() {
	for (auto& e : arr) {
		free(e);
	}
	arr.clear();
}

size_t EntityMng::size() const {
	return arr.size();
}

void EntityMng::destroy(Entity* e) {
	e->destroyed = true;
}

void EntityMng::purge_destroyed() {
	std::vector<Entity*> after_purge;
	for (Entity* e : arr) {
		if (e->destroyed) {
			delete e;
			continue;
		}
		after_purge.push_back(e);
	}

	arr = after_purge;
}