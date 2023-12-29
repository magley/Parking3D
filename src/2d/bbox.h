#pragma once

#include <glm/glm.hpp>

struct BBox {
	glm::vec2 pos;
	glm::vec2 size;

	BBox();
	BBox(glm::vec2 pos, glm::vec2 size);

	bool contains(glm::vec2 p) const;
	glm::vec2 bottom_right() const;
	BBox operator * (float other) const;
	BBox operator + (glm::vec2 other) const;

};