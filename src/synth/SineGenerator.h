/**
 * Additive Synth - Experimental Synthesizer with some features to explore.
 *
 * Copyright (C) 2023 Christoph Wellm <christoph.wellm@creaflect.de>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the 
 * GNU General Public License version 3 as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without 
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License along with this program.  
 * If not, see <http://www.gnu.org/licenses/>.
 * 
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once

#include <cmath>
#include <memory>
#include <vector>

namespace cw::synth {

class SineGenerator {

	/* 
	* This class provides sinewaves which are suited to be used for an audio synthesizer. It relies on internal features
	* of JUCE, such that the design of this generator can be simplified in terms of DSP aspects. Therefore, the only used
	* parameters remaining are the sample rate, the total scaling factor and the reference frequency (at which the sine
	* wave is output). 
	*/

	public:
		SineGenerator(int sampleRate, float scalingFactor, float referenceFrequency) :
			sampleRate(sampleRate), scalingFactor(scalingFactor), referenceFrequency(referenceFrequency) {
			bufferPointer = std::make_unique<int>(0);
		};
		SineGenerator(){
			SineGenerator(44100, 1, 1);
		};

		void setSampleRate(int sampleRate) { this->sampleRate = sampleRate; }
		void setScalingFactor(float scalingFactor) { this->scalingFactor = scalingFactor; }
		void setReferenceFrequency(float referenceFrequency) { this->referenceFrequency = referenceFrequency; }
		void resetBufferPointer() { *bufferPointer = 0; }
		/*
		* This method generates one second of the sound and stores it in a vector, the size of which depends on the 
		* sample rate. 
		*/
		std::vector<float> generate();

	private:
		int sampleRate;
		float scalingFactor;
		float referenceFrequency;
		std::unique_ptr<int> bufferPointer;
};

}
