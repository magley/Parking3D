#pragma once

struct Entity;

struct CMiniScreen {
	enum State {
		RAISED,
		GO_UP,
		GO_DOWN,
		LOWERED
	};

	State state = State::RAISED;
	float angle = 0;

	void update(Entity* self);
	void raise(Entity* self);
	void lower(Entity* self);
};