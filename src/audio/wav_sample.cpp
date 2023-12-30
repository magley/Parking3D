#include "wav_sample.h"

WavSample::WavSample() {

}

WavSample::WavSample(const char* fname) {
	sample.load(fname);
}