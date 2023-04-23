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

#include "SoundProcessor.h"
#include <memory>
#include <cmath>

namespace cw::synth {
    std::vector<float> HarmonicSoundProcessor::process(int noSamples, float refFrequency, int midiNoteNumber) {
        // A4: midi no. 69, pitch 440 Hz
        float midiFreq = 440. * std::pow(2., (midiNoteNumber - 69.)/12.);
        return process(noSamples, midiFreq, refFrequency);
    }
    std::vector<float> HarmonicSoundProcessor::process(int noSamples, float frequency, float refFreq) {
        return process(noSamples, frequency/refFreq);

    }
    std::vector<float> HarmonicSoundProcessor::process(int noSamples, float playingFactor) {
        auto result = std::vector<float>(noSamples);
        float a_0, a_1; // linear interpolation: y_f = a_1*x_f + a_0
        int pos_0, pos_1;

        for (int samp = 0; samp < noSamples; ++samp) {
            result[samp] = 0;
            
            for (int harm = 0; harm < NO_ADDSYNTH_VOICES; ++harm) {
                // determine interpolation values
                pos_0 = (int)continuousPos[harm] % sound.size();
                pos_1 = (pos_0 + 1) % sound.size();
                a_1 = sound.at(pos_1) - sound.at(pos_0);
                a_0 = (float)pos_0 * (sound.at(pos_0) - sound.at(pos_1)) + sound.at(pos_0);

                // add the interpolated value of the current harmonic times its gain
                result[samp] += (a_1 * continuousPos[harm] + a_0) * params.harmonicGain[harm];

                // increase the position pointer according to the speed and harmonic
                continuousPos[harm] += sound.size() / sampleRate * playingFactor * (harm + 1);
                continuousPos[harm] = std::fmod(continuousPos[harm], (float)sound.size());
            } 
            result[samp] /= NO_ADDSYNTH_VOICES;
        }

        return result;
    }

    void HarmonicSoundProcessor::setHarmGain(int noHarmonic, float value) {
        // TODO: error catching!
        params.harmonicGain[noHarmonic] = value;
    }
} // namespace cw::synth