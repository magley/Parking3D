#pragma once
struct Entity;

struct CRamp {
	enum State {
		RISE,
		DROP,
	};

	State state = State::DROP;

	void update(Entity* self);
	void toggle();
};