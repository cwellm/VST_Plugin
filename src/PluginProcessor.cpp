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

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <string>

//==============================================================================
NewProjectAudioProcessor::NewProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    additiveSynth = std::make_unique<cw::synth::AdditiveSynth>();
    for (int i = 0; i < NO_ADDSYNTH_VOICES; ++i) {
        if (i == 0) {
            paramHarmGains.push_back(new juce::AudioParameterFloat("harmonic" + std::to_string(i), "Harmonic " + std::to_string(i), 0.0, 1.0, 1.0));
            paramHarmGainsTarget.push_back(1.f);
        }
        else {
            paramHarmGains.push_back(new juce::AudioParameterFloat("harmonic" + std::to_string(i), "Harmonic " + std::to_string(i), 0.0, 1.0, 0.0));
            paramHarmGainsTarget.push_back(0.f);
        }
    }

    // todo: init parameters and tell the harmGains in the SoundProcessor of it!
    for (auto it = paramHarmGains.begin(); it != paramHarmGains.end(); ++it) {
        addParameter(*it);
    }

    // ADSR
    addParameter(paramA = new juce::AudioParameterFloat("attack", "Attack", 0.0, 1.0, 0.0));
    addParameter(paramD = new juce::AudioParameterFloat("delay", "Delay", 0.0, 1.0, 0.5));
    addParameter(paramS = new juce::AudioParameterFloat("sustain", "Sustain", 0.0, 1.0, 0.5));
    addParameter(paramR = new juce::AudioParameterFloat("release", "Release", 0.0, 1.0, 0.1));

    addParameter(paramPhi = new juce::AudioParameterFloat("phi", "Phi", 0.0, 2 * juce::MathConstants<float>::pi, 0.0));
    addParameter(paramTheta = new juce::AudioParameterFloat("theta", "Theta", 0.0, 2 * juce::MathConstants<float>::pi, 0.0));

    // set initial target values
    paramATarget = paramA->get();
    paramDTarget = paramD->get();
    paramSTarget = paramS->get();
    paramRTarget = paramR->get();
    paramPhiTarget = paramPhi->get();
    paramThetaTarget = paramTheta->get();

    auto itTarget = paramHarmGainsTarget.begin();
    for (auto it = paramHarmGains.begin(); it != paramHarmGains.end(); ++it) {
        *itTarget = (*it)->get();
    }

}

NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
}

//==============================================================================
const juce::String NewProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NewProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NewProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NewProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NewProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void NewProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    additiveSynth->prepareToPlay(samplesPerBlock, sampleRate);
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void NewProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NewProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void NewProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // +++++++++++++++++++++ setting parameters ++++++++++++++++++++
    // Parameter smoothing is applied. The target values are denoted by ending with *Target, the current values are 
    // given by the param values themselves. The current values are iteratively updated with a ratio given by the 
    // variables smRat*. 

    if ((std::abs(paramATarget - paramA->get())) > smRatEpsilon) {
        *paramA = paramA->get() + (paramATarget - paramA->get())*smRatADSR;
    }

    if ((std::abs(paramDTarget - paramD->get())) > smRatEpsilon) {
        *paramD = paramD->get() + (paramDTarget - paramD->get()) * smRatADSR;
    }

    if ((std::abs(paramSTarget - paramS->get())) > smRatEpsilon) {
        *paramS = paramS->get() + (paramSTarget - paramS->get()) * smRatADSR;
    }

    if ((std::abs(paramRTarget - paramR->get())) > smRatEpsilon) {
        *paramR = paramR->get() + (paramRTarget - paramR->get()) * smRatADSR;
    }

    if ((std::abs(paramPhiTarget - paramPhi->get())) > smRatEpsilon) {
        *paramPhi = std::fmod(paramPhi->get() + (paramPhiTarget - paramPhi->get()) * smRatQuantum,
            2 * juce::MathConstants<float>::pi);
    }

    if ((std::abs(paramThetaTarget - paramTheta->get())) > smRatEpsilon) {
        *paramTheta = std::fmod(paramTheta->get() + (paramThetaTarget - paramTheta->get()) * smRatQuantum,
            2 * juce::MathConstants<float>::pi);
    }

    auto voices = additiveSynth->getVoices();
    for (auto voice : voices) {
        for (int i = 0; i < NO_ADDSYNTH_VOICES; ++i) {
            if ((std::abs(paramHarmGainsTarget.at(i) - paramHarmGains.at(i)->get())) > smRatEpsilon) {
                *paramHarmGains.at(i) = paramHarmGains.at(i)->get() + (paramHarmGainsTarget.at(i) 
                    - paramHarmGains.at(i)->get()) * smRatHarm;
                voice->getHarmProcessor()->setHarmGain(i, paramHarmGains.at(i)->get());
            }
        }
        voice->setAdsrParameters(paramA->get(), paramD->get(), paramS->get(), paramR->get());
        voice->setPhi(paramPhi->get());
        voice->setTheta(paramTheta->get());
    }


    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    //for (int channel = 0; channel < totalNumInputChannels; ++channel)
    //{
    //    auto* channelData = buffer.getWritePointer (channel);  
    //}

    // +++++++++++++++++++++ do processing ++++++++++++++++++++
    additiveSynth->setMidiBuffer(midiMessages);
    additiveSynth->getNextAudioBlock(AudioSourceChannelInfo(&buffer, 0, buffer.getNumSamples()));
}

//==============================================================================
bool NewProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor (*this);
}

//==============================================================================
void NewProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void NewProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}
