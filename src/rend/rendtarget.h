#pragma once

struct RendTarget {
	unsigned fbo         = 0;
	unsigned texture     = 0;
	bool is_default      = false;
	int w                = 0;
	int h                = 0;

	static RendTarget init_default_rend_target();
	RendTarget(int w, int h);
	void make_current();
	void resize(int w, int h);
	void clear(float r, float g, float b);
	void set_depth_testing(bool is_enabled);
private:
	RendTarget();
};