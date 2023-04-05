#include "SoundProcessor.h"
#include <memory>
#include <cmath>

namespace cw::synth {
    std::unique_ptr<float[]> HarmonicSoundProcessor::process(int noSamples, float refFrequency, int midiNoteNumber) {
        // A4: midi no. 69, pitch 440 Hz
        float midiFreq = 440. * std::pow(2., (midiNoteNumber - 69.)/12.);
        return process(noSamples, midiFreq, refFrequency);
    }
    std::unique_ptr<float[]> HarmonicSoundProcessor::process(int noSamples, float frequency, float refFreq) {
        return process(noSamples, frequency/refFreq);

    }
    std::unique_ptr<float[]> HarmonicSoundProcessor::process(int noSamples, float playingFactor) {
        auto result = std::make_unique<float[]>(noSamples);
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

    void HarmonicSoundProcessor::setP(char paramId, float value) {
        // TODO: error catching!
        switch (paramId) {
            case 'A':
            {
                params.a = value;
                break;
            }
            case 'D':
            {
                params.d = value;
                break;
            }
            case 'S':
            {
                params.s = value;
                break;
            }
            case 'R':
            {
                params.r = value;
                break;
            }
        }
    }
} // namespace cw::synth