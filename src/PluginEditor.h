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

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "components/QuantumComponent.h"
#include "components/AddSynthComponent.h"
#include "components/ADSRComponent.h"

//==============================================================================
/**
*/
class NewProjectAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Slider::Listener
{
public:
    NewProjectAudioProcessorEditor (NewProjectAudioProcessor&);
    ~NewProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NewProjectAudioProcessor& audioProcessor;

    cw::synth::QuantumComponent quantumComponent;
    cw::synth::ADSRComponent adsrComponent;
    cw::synth::AddSynthComponent addSynthComponent;

    std::vector<std::unique_ptr<juce::Slider>> harmGains;
    juce::Slider aVal;
    juce::Slider dVal;
    juce::Slider sVal;
    juce::Slider rVal;

    // sizes
    const int EDITORWIDTH{ 500 };
    const int EDITORHEIGHT{ 550 };
    const int AREAMARGIN{ 20 };

    // add listener
    void sliderValueChanged(juce::Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};