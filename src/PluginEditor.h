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