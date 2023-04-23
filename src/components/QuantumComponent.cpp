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

#include "QuantumComponent.h"

#include <JuceHeader.h>
#include <string>

namespace cw::synth {

QuantumComponent::QuantumComponent() {
    phi.setSliderStyle(juce::Slider::Rotary);
    phi.setName("QPhi");
    phi.setRotaryParameters(juce::Slider::RotaryParameters{ 0.0f, juce::MathConstants<float>::pi * 2, false });
    phi.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(phi);

    theta.setSliderStyle(juce::Slider::Rotary);
    theta.setName("QTheta");
    theta.setRotaryParameters(juce::Slider::RotaryParameters{ 0.0f, juce::MathConstants<float>::pi * 2, false });
    theta.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(theta);

    thetaLabel.setFont(juce::Font(20.0f, juce::Font::bold));
    thetaLabel.setText("QTheta", juce::dontSendNotification);
    thetaLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(thetaLabel);

    phiLabel.setFont(juce::Font(20.0f, juce::Font::bold));
    phiLabel.setText("QPhi", juce::dontSendNotification);
    phiLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(phiLabel);
}

void QuantumComponent::paint(juce::Graphics& g) {
    auto area = this->getLocalBounds();
    g.setFont(20);
    g.drawText("Quantum Angles", area.removeFromTop(20), juce::Justification::centred);
}

void QuantumComponent::resized() {
    auto area = this->getLocalBounds();
    area.removeFromTop(30);

    auto phiArea = area.removeFromLeft(area.getWidth() / 2);
    auto thetaArea = area;

    phiArea.reduce(phiArea.getWidth() * 0.1, phiArea.getWidth() * 0.1);
    thetaArea.reduce(thetaArea.getWidth() * 0.1, thetaArea.getWidth() * 0.1);

    auto phiLabelArea = phiArea.removeFromTop(20);
    auto thetaLabelArea = thetaArea.removeFromTop(20);

    phiLabel.setBounds(phiLabelArea);
    thetaLabel.setBounds(thetaLabelArea);

    phiArea.translate(0, -30); // knobs closer at text
    phi.setBounds(phiArea);

    thetaArea.translate(0, -30); // knobs closer at text
    theta.setBounds(thetaArea);
}

void QuantumComponent::addListener(juce::Slider::Listener* listener) {
    phi.addListener(listener);
    theta.addListener(listener);
}

juce::Slider& QuantumComponent::getComponent(std::string name) {
    if (name == "phi") {
        return phi;
    }
    else if (name == "theta") {
        return theta;
    }
    throw std::string{"Unknown Name."};
}

} // namespace cw::synth