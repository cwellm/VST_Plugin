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

#include <vector>
#include <memory>
#include <JuceHeader.h>

#define NO_ADDSYNTH_VOICES 16

namespace cw::synth {
    
struct SoundParameters {
    float a; // currently not in use
    float d; // currently not in use
    float s; // currently not in use
    float r; // currently not in use
    float harmonicGain[NO_ADDSYNTH_VOICES]; // 16 harmonics for each audio input
};

class HarmonicSoundProcessor {
    /*
    * This class is responsible for processing an input sound in the form of a variable-length vector. It will feed it 
    * to a synth, possibly applying an ADSR curve, and possibly applying some harmonic gain.
    */
    public:
        HarmonicSoundProcessor(const std::vector<float>& sound, const int& sampleRate): sound(sound),
            sampleRate(sampleRate) {
            params = { 0, 0, 0, 0, {0} };
            params.harmonicGain[0] = 1;
            continuousPos = std::make_unique<float[]>(NO_ADDSYNTH_VOICES);
            resetPos();
        };

        // Sets the harmonic gain parameter at the given position and applies some smoothing, where convenient.
        void setHarmGain(int, float);
        // Sets the sample rate.
        void setSampleRate(int sampleRate) {
            this->sampleRate = sampleRate;
        }
        /* Processes the sound at the given frequency, relative to the reference frequency, and writes the result to an
        * output array. The reference frequency is the frequency at which the original sound is meant to play, for a 
        * given sample rate. The size of the original sound vector signifies its original 'recording' sample rate. 
        */
        std::vector<float> process(int, float, float);
        /* Processes the sound for the given MIDI note number, relative to the reference frequency, and writes the result 
         * to an output array. The reference frequency is the frequency at which the original sound is meant to play, for 
         * a given sample rate.
         */
        std::vector<float> process(int, float, int);
        /* Processes the sound and writes the result to an output array. A relative playing speed different from one
         * controls the effective frequency, if the concept of 'frequency' makes sense for the sound.
         */
        std::vector<float> process(int, float);
        // Resetting the position pointers. 
        void resetPos() {
            for (int i = 0; i < NO_ADDSYNTH_VOICES; ++i) {
                continuousPos[i] = 0;
            }
        }

    private:
        SoundParameters params;
        std::vector<float> sound;
        std::unique_ptr<float[]> continuousPos;
        int sampleRate;
};

} // namespace cw::synth