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

#include "AddSynthComponent.h"

#include <JuceHeader.h>
#include "../synth/AdditiveSynth.h"

namespace cw::synth {

AddSynthComponent::AddSynthComponent() {
    for (int i = 0; i < NO_ADDSYNTH_VOICES; ++i) {
        harmGains.push_back(std::make_unique<juce::Slider>());
        harmGainLabels.push_back(std::make_unique<juce::Label>());
    }

    for (int i = 0; i < NO_ADDSYNTH_VOICES; ++i) {
        (*harmGains.at(i)).setSliderStyle(juce::Slider::LinearBar);
        (*harmGains.at(i)).setRange(0.0, 1.0, 0.02);
        (*harmGains.at(i)).setName("HGain " + std::to_string(i+1));
        addAndMakeVisible(harmGains.at(i).get());

        (*harmGainLabels.at(i)).setFont(juce::Font(10.0f, juce::Font::bold));
        (*harmGainLabels.at(i)).setText("Harmonic " + std::to_string(i+1), juce::dontSendNotification);
        (*harmGainLabels.at(i)).setJustificationType(juce::Justification::left);
        addAndMakeVisible(harmGainLabels.at(i).get());
    }
}

void AddSynthComponent::addListener(juce::Slider::Listener* listener) {
    for (int i = 0; i < NO_ADDSYNTH_VOICES; ++i) {
        (*harmGains.at(i)).addListener(listener);
    }
}

void AddSynthComponent::paint(juce::Graphics& g) {

    auto area = this->getLocalBounds();
    g.setFont(20);
    g.drawText("Harmonic Gains", area.removeFromTop(TEXTMARGIN/2), juce::Justification::centredTop);
}

void AddSynthComponent::resized() {
    float visibleRatio = 0.4;

    auto area = this->getLocalBounds();
    area.removeFromRight(10);
    area.removeFromTop(TEXTMARGIN);

    float singleHeight = area.getHeight() / (float)(NO_ADDSYNTH_VOICES);
    float visibleHeight = singleHeight * visibleRatio;
    float labelHeight = singleHeight * (1 - visibleRatio);

    for (int i = 0; i < NO_ADDSYNTH_VOICES; ++i) {
        (*harmGainLabels.at(i)).setBounds(area.removeFromTop((int)labelHeight));
        (*harmGains.at(i)).setBounds(area.removeFromTop((int)visibleHeight));
    }

}

} // namespace cw::synth

