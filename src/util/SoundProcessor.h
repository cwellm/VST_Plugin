#pragma once

#include <vector>
#include <memory>
#include <JuceHeader.h>

#define NO_ADDSYNTH_VOICES 16

namespace cw::synth {
    
struct SoundParameters {
    float a;
    float d;
    float s;
    float r;
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
        // Sets the ADSR parameter given to the given value and applies some smoothing, where convenient.
        void setP(char, float);
        // Sets all ADSR parameters at once
        void setPs(float a, float d, float s, float r) {
            setP('A', a);
            setP('D', d);
            setP('S', s);
            setP('R', r);
        }
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