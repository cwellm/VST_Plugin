#include "SineGenerator.h"

#include <vector>

#define PI_CONST 3.141592654

namespace cw::synth {

	std::vector<float> SineGenerator::generate() {
		std::vector<float> result;
		float next_val = 0;
		for (int i = 0; i < this->sampleRate; ++i) {
			next_val = std::sin(2.*PI_CONST*i*this->referenceFrequency/this->sampleRate);
			result.push_back(this->scalingFactor*next_val);
		}
		return result;
	}

}