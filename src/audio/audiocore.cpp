#include "audiocore.h"

AudioCore::AudioCore() {
	core.init();
}

AudioHandle AudioCore::play(WavSample* wav, bool loop) {
	auto handle = core.play(wav->sample);
	core.setLooping(handle, loop);
	return AudioHandle(handle);
}

AudioHandle AudioCore::play_3d(WavSample* wav, glm::vec3 pos, bool loop) {
	auto handle = core.play3d(wav->sample, pos.x, pos.y, pos.z);
	core.setLooping(handle, loop);
	return AudioHandle(handle);
}

void AudioCore::update_3d_listener(glm::vec3 pos, glm::vec3 at, glm::vec3 up) {
	core.set3dListenerPosition(pos.x, pos.y, pos.z);
	core.set3dListenerAt(at.x, at.y, at.z);
	core.set3dListenerUp(up.x, up.y, up.z);
	core.update3dAudio();
}

void AudioCore::pause(AudioHandle handle, bool pause) {
	core.setPause(handle.handle, pause);
}

void AudioCore::stop(AudioHandle handle) {
	core.stop(handle.handle);
}