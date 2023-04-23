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

#include "ADSRComponent.h"

#include <JuceHeader.h>

namespace cw::synth {

ADSRComponent::ADSRComponent() {
    A.setSliderStyle(juce::Slider::LinearBar);
    A.setRange(0.0, 1.0, 0.01);
    A.setName("Attack");
    addAndMakeVisible(A);

    D.setSliderStyle(juce::Slider::LinearBar);
    D.setRange(0.0, 1.0, 0.01);
    D.setName("Delay");
    addAndMakeVisible(D);

    S.setSliderStyle(juce::Slider::LinearBar);
    S.setRange(0.0, 1.0, 0.01);
    S.setName("Sustain");
    addAndMakeVisible(S);

    R.setSliderStyle(juce::Slider::LinearBar);
    R.setRange(0.0, 1.0, 0.01);
    R.setName("Sustain");
    addAndMakeVisible(R);

    aLabel.setFont(juce::Font(12.0f, juce::Font::bold));
    aLabel.setText("Attack", juce::dontSendNotification);
    aLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(aLabel);

    dLabel.setFont(juce::Font(12.0f, juce::Font::bold));
    dLabel.setText("Delay", juce::dontSendNotification);
    dLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(dLabel);

    sLabel.setFont(juce::Font(12.0f, juce::Font::bold));
    sLabel.setText("Sustain", juce::dontSendNotification);
    sLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(sLabel);

    rLabel.setFont(juce::Font(12.0f, juce::Font::bold));
    rLabel.setText("Release", juce::dontSendNotification);
    rLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(rLabel);
}

juce::Slider& ADSRComponent::getComponent(std::string name) {
    if (name == "A") {
        return A;
    }
    else if (name == "D") {
        return D;
    }
    else if (name == "S") {
        return S;
    }
    else if (name == "R") {
        return R;
    }
    else {
        throw std::string{ "Unknown Name." };
    }
}

void ADSRComponent::addListener(juce::Slider::Listener* listener) {
    A.addListener(listener);
    D.addListener(listener);
    S.addListener(listener);
    R.addListener(listener);
}

void ADSRComponent::paint(juce::Graphics& g) {
    auto area = this->getLocalBounds();

    g.setFont(20);
    g.drawText("Envelope Parameters", area.removeFromTop(TEXTMARGIN / 2), juce::Justification::centredTop);


}

void ADSRComponent::resized() {
    auto area = this->getLocalBounds();
    area.removeFromTop(TEXTMARGIN);

    float singleHeight = area.getHeight() / 8.;

    aLabel.setBounds(area.removeFromTop((int)singleHeight));
    A.setBounds(area.removeFromTop((int)singleHeight));
    dLabel.setBounds(area.removeFromTop((int)singleHeight));
    D.setBounds(area.removeFromTop((int)singleHeight));
    sLabel.setBounds(area.removeFromTop((int)singleHeight));
    S.setBounds(area.removeFromTop((int)singleHeight));
    rLabel.setBounds(area.removeFromTop((int)singleHeight));
    R.setBounds(area.removeFromTop((int)singleHeight));
}

} // namespace cw::synth