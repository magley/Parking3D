#include "audio_handle.h"

AudioHandle::AudioHandle() {
}

AudioHandle::AudioHandle(SoLoud::handle handle) 
	: handle(handle) {
}