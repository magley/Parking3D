#pragma once
#include <vector>

enum Event {
	EVENT_TOGGLE_HOUSE_LIGHT = 0,
	EVENT_TOGGLE_RAMP = 1,
	EVENT_size
};

struct EventMng {
	bool active[EVENT_size] = { false };

	void consume();
	void pub(Event ev);
};

namespace glo {
	extern EventMng* event;
}