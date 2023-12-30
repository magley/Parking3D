#pragma once
#include <soloud_wav.h>

struct WavSample {
	SoLoud::Wav sample;

	WavSample();
	WavSample(const char* fname);
};