#include "bbox.h"

BBox::BBox() {

}

BBox::BBox(glm::vec2 pos, glm::vec2 size): pos(pos), size(size) {

}

bool BBox::contains(glm::vec2 p) const {
	return p.x >= pos.x && p.x <= pos.x + size.x
		&& p.y >= pos.y && p.y <= pos.y + size.y;
}

glm::vec2 BBox::bottom_right() const {
	return pos + size;
}

BBox BBox::operator * (float other) const {
	return BBox(pos, size * other);
}

BBox BBox::operator + (glm::vec2 other) const {
	return BBox(pos + other, size);
}