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