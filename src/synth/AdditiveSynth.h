#pragma once

#include <JuceHeader.h>
#include "../util/SoundProcessor.h"
#include "SineGenerator.h"
#include "QuantumEffects.h"

#define ADDSYNTH_MAXPOLYPHONY 8

namespace cw::synth {

struct AddSynthSound : public juce::SynthesiserSound
    {
        AddSynthSound() {}

        bool appliesToNote(int) override { return true; }
        bool appliesToChannel(int) override { return true; }
    };

//===================================================================================

struct AddSynthVoice : public juce::SynthesiserVoice
{
    AddSynthVoice() {
        SineGenerator generator{ 44100, 1.0, 1.0 };
        harmProcessor = std::make_shared<HarmonicSoundProcessor>(generator.generate(), 44100);
        adsrCurve = juce::ADSR();
    }

    bool canPlaySound(juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<AddSynthSound*> (sound) != nullptr;
    }

    void startNote(int midiNoteNumber, float velocity,
        juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        this->midiNoteNumber = midiNoteNumber;
        harmProcessor->resetPos();
        harmProcessor->setSampleRate(getSampleRate());

        // for testing...
        currentAngle = 0.0;
        level = velocity * 0.15;
        tailOff = 0.0;

        adsrCurve.noteOn();

        auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        auto cyclesPerSample = cyclesPerSecond / getSampleRate();

        angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
    }

    void stopNote(float /*velocity*/, bool allowTailOff) override
    {
        if (allowTailOff)
        {
            if (tailOff == 0.0) {
                tailOff = 1.0;
            }
        }
        else
        {
            clearCurrentNote();
            rotator.clearBuffer();
            angleDelta = 0.0;
        }

        adsrCurve.noteOff();
    }

    void pitchWheelMoved(int) override {}
    void controllerMoved(int, int) override {}

    void renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        auto synthesizedOutput = harmProcessor->process(numSamples, 1., this->midiNoteNumber);
        auto quantumTransformedOutput = rotator.spinRotate(std::array<std::vector<float>, 2>{synthesizedOutput,
            synthesizedOutput});
        if (tailOff > 0.0) {
            for (int sampleNo = 0; sampleNo < quantumTransformedOutput[0].size(); ++sampleNo) {
                for (auto i = outputBuffer.getNumChannels(); --i >= 0;) {
                    auto currentSample = quantumTransformedOutput[i].at(sampleNo) * 0.1 * adsrCurve.getNextSample();
                    outputBuffer.addSample(i, startSample, currentSample);
                }
                currentAngle += angleDelta;
                tailOff = adsrCurve.getNextSample();
                ++startSample;
                if (tailOff <= 0.005)
                {
                    clearCurrentNote();
                    rotator.clearBuffer();
                    angleDelta = 0.0;       
                    break;
                }
            }
        }
        else {
            for (int sampleNo = 0; sampleNo < quantumTransformedOutput[0].size(); ++sampleNo) {
                for (auto i = outputBuffer.getNumChannels(); --i >= 0;) {
                    auto currentSample = quantumTransformedOutput[i].at(sampleNo) * 0.1 * adsrCurve.getNextSample();
                    outputBuffer.addSample(i, startSample, currentSample);
                }
                currentAngle += angleDelta;
                ++startSample;
            }
        }
    }

    std::shared_ptr<HarmonicSoundProcessor> getHarmProcessor() {
        return harmProcessor;
    }

    void setAdsrParameters(float a, float d, float s, float r) {
        // a in seconds, d in seconds, s level, r in seconds
        adsrCurve.setParameters(juce::ADSR::Parameters(a*2, d, s, r*3));
    }

    void setPhi(float phi) {
        rotator.setPhi(phi);
    }

    void setTheta(float theta) {
        rotator.setTheta(theta);
    }

    private:
        std::shared_ptr<HarmonicSoundProcessor> harmProcessor;
        int midiNoteNumber;
        // for testing...
        double currentAngle = 0.0, angleDelta = 0.0, level = 0.0, tailOff = 0.0;
        juce::ADSR adsrCurve;
        Spin3Rotation rotator{};
};

//===================================================================================

class AdditiveSynth : public juce::AudioSource {
    public:
        AdditiveSynth()
        {
            for (auto i = 0; i < ADDSYNTH_MAXPOLYPHONY; ++i)
                synth.addVoice(new AddSynthVoice());

            synth.addSound(new AddSynthSound());
        }

        void setUsingSineWaveSound()
        {
            synth.clearSounds();
        }

        void setMidiBuffer(juce::MidiBuffer midiBuffer) {
            incomingMidiBuffer = midiBuffer;
        }

        void prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate) override
        {
            synth.setCurrentPlaybackSampleRate(sampleRate); // [3]
        }

        void releaseResources() override {}

        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
        {
            bufferToFill.clearActiveBufferRegion();

            synth.renderNextBlock(*bufferToFill.buffer, incomingMidiBuffer,
                bufferToFill.startSample, bufferToFill.numSamples);
        }

        std::vector<AddSynthVoice*> getVoices() {
            std::vector<AddSynthVoice*> voices;
            for (int i = 0; i < synth.getNumVoices(); ++i) {
                voices.push_back(dynamic_cast<AddSynthVoice*>(synth.getVoice(i)));
            }
            return voices;
        }

    private:
        juce::Synthesiser synth;
        juce::MidiBuffer incomingMidiBuffer;
};

//===================================================================================



} // namespace cw::synth