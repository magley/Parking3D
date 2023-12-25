#pragma once
struct Entity;

struct CRamp {
	enum State {
		REST,
		RISE,
		RISE_WAIT,
		DROP,
	};

	int timer = 0;
	State state = State::REST;

	void update(Entity* self);
};