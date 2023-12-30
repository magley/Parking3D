#include "caudio.h"
#include "global.h"

void CAudio::play(WavSample* sample, bool loop) {
	handle = glo::wctx.audio.play(sample, loop);
}

void CAudio::play(const char* fname, bool loop) {
	WavSample* sample = glo::wctx.resmng.load_wav(fname);
	play(sample, loop);
}

void CAudio::play_3d(WavSample* sample, glm::vec3 pos, bool loop) {
	handle = glo::wctx.audio.play_3d(sample, pos, loop);
}

void CAudio::play_3d(const char* fname, glm::vec3 pos, bool loop) {
	WavSample* sample = glo::wctx.resmng.load_wav(fname);
	play_3d(sample, pos, loop);
}

void CAudio::pause(bool should_pause) {
	glo::wctx.audio.pause(handle, should_pause);
}

void CAudio::stop() {
	glo::wctx.audio.stop(handle);
}