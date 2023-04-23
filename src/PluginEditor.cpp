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

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "util/SoundProcessor.h"
#include <string>

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    setSize(EDITORWIDTH, EDITORHEIGHT);

    // harmonic gains
    addSynthComponent.setName("Harmonic Gains");
    this->addAndMakeVisible(addSynthComponent);
    addSynthComponent.addListener(this);

    for (int i = 0; i < NO_ADDSYNTH_VOICES; ++i) {
        addSynthComponent.setHarmGainsVal(i, p.paramHarmGains.at(i)->get());
    }

    // ADSR
    adsrComponent.setName("ADSR Parameters");
    this->addAndMakeVisible(adsrComponent);

    adsrComponent.addListener(this);
    adsrComponent.getComponent("A").setValue(p.paramA->get());
    adsrComponent.getComponent("D").setValue(p.paramD->get());
    adsrComponent.getComponent("S").setValue(p.paramS->get());
    adsrComponent.getComponent("R").setValue(p.paramR->get());

    // Quantum Angles
    quantumComponent.setName("Quantum Parameters");

    this->addAndMakeVisible(quantumComponent);
    quantumComponent.addListener(this);
    quantumComponent.getComponent("phi").setValue(p.paramPhi->get());
    quantumComponent.getComponent("theta").setValue(p.paramTheta->get());

    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::aqua);
    getLookAndFeel().setColour(juce::Slider::backgroundColourId, juce::Colours::black);
    getLookAndFeel().setColour(juce::Label::textColourId, juce::Colours::darkblue);
}


NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::white);
    g.setColour (juce::Colours::black);
}

void NewProjectAudioProcessorEditor::resized()
{
    auto area = this->getLocalBounds();
    area.reduce(AREAMARGIN, AREAMARGIN);

    addSynthComponent.setBounds(area.removeFromLeft(EDITORWIDTH/2.- AREAMARGIN));
    adsrComponent.setBounds(area.removeFromTop(EDITORHEIGHT / 2. - AREAMARGIN));
    quantumComponent.setBounds(area);
}

//==============================================================================
void NewProjectAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
    for (int i = 0; i < NO_ADDSYNTH_VOICES; ++i) {
        if (slider == &addSynthComponent.getHarmGainsComponent(i)) {
            audioProcessor.paramHarmGainsTarget.at(i) = slider->getValue();
        }
    }

    if (slider == &adsrComponent.getComponent("A")) {
        audioProcessor.paramATarget = slider->getValue();
    }

    if (slider == &adsrComponent.getComponent("D")) {
        audioProcessor.paramDTarget = slider->getValue();
    }

    if (slider == &adsrComponent.getComponent("S")) {
        audioProcessor.paramSTarget = slider->getValue();
    }
    if (slider == &adsrComponent.getComponent("R")) {
        audioProcessor.paramRTarget = slider->getValue();
    }

    if (slider == &quantumComponent.getComponent("phi")) {
        audioProcessor.paramPhiTarget = std::fmod(slider->getValue(), 2*juce::MathConstants<float>::pi);
    }

    if (slider == &quantumComponent.getComponent("theta")) {
        audioProcessor.paramThetaTarget = std::fmod(slider->getValue(), 2 * juce::MathConstants<float>::pi);
   }

}