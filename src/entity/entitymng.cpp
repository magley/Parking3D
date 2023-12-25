#include "entitymng.h"

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