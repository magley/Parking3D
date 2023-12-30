#pragma once
struct Entity;

struct CRamp {
	enum State {
		RISE,
		DROP,
	};

	State state = State::DROP;
	bool all_the_way_up = false;

	void update(Entity* self);
	void toggle();
};