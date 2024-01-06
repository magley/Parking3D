#include "event.h"

void EventMng::consume() {
	for (int i = 0; i < EVENT_size; i++) {
		active[i] = false;
	}
}

void EventMng::pub(Event e) {
	active[e] = true;
}