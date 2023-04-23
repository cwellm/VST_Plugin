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

#pragma once

#include <JuceHeader.h>
#include "synth/AdditiveSynth.h"
#include <vector>

//==============================================================================
/**
*/
class NewProjectAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    NewProjectAudioProcessor();
    ~NewProjectAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //============================ Parameters ==================================================
    std::vector<juce::AudioParameterFloat*> paramHarmGains;
    juce::AudioParameterFloat* paramA;
    juce::AudioParameterFloat* paramD;
    juce::AudioParameterFloat* paramS;
    juce::AudioParameterFloat* paramR;
    juce::AudioParameterFloat* paramPhi;
    juce::AudioParameterFloat* paramTheta;

    std::vector<float> paramHarmGainsTarget;
    float paramATarget;
    float paramDTarget;
    float paramSTarget;
    float paramRTarget;
    float paramPhiTarget;
    float paramThetaTarget;

private:
    std::unique_ptr<cw::synth::AdditiveSynth> additiveSynth;

    const float smRatADSR = 0.2;
    const float smRatHarm = 0.2;
    const float smRatQuantum = 0.2;
    const float smRatEpsilon = 1e-4;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessor)
};
