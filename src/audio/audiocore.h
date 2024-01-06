#pragma once
#include <soloud.h>
#include "wav_sample.h"
#include <glm/glm.hpp>
#include "audio_handle.h"

struct AudioCore {
	SoLoud::Soloud core;
	AudioCore();
	AudioHandle play(WavSample* wav, bool loop = false);
	AudioHandle play_3d(WavSample* wav, glm::vec3 pos, bool loop = false);
	void pause(AudioHandle handle, bool pause);
	void stop(AudioHandle handle);
	void update_3d_listener(glm::vec3 pos, glm::vec3 at, glm::vec3 up);
};

namespace glo {
	extern AudioCore* audio;
}