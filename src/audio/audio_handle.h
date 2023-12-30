#pragma once
#include <soloud.h>

struct AudioHandle {
	SoLoud::handle handle;

	AudioHandle();
	AudioHandle(SoLoud::handle handle);
};