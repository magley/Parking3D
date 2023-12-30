#pragma once

#include "audio/wav_sample.h"
#include "audio/audio_handle.h"
#include <glm/glm.hpp>

struct CAudio {
	AudioHandle handle; // It's up to you to keep track of things.

	void play(WavSample* sample, bool loop = false);
	void play(const char* fname, bool loop = false);
	void play_3d(WavSample* sample, glm::vec3 pos, bool loop = false);
	void play_3d(const char* fname, glm::vec3 pos, bool loop = false);
	void pause(bool should_pause);
	void stop();
};