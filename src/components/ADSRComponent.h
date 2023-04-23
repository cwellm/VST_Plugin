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

#pragma once

#include <string>
#include <JuceHeader.h>

namespace cw::synth {

class ADSRComponent : public juce::Component /*, public juce::Slider::Listener*/ {

	public:
		ADSRComponent();
		void paint(juce::Graphics& g) override;
		void resized() override;

		juce::Slider& getComponent(std::string);
		void addListener(juce::Slider::Listener*);

	private:
		juce::Slider A;
		juce::Slider D;
		juce::Slider S;
		juce::Slider R;

		juce::Label aLabel;
		juce::Label dLabel;
		juce::Label sLabel;
		juce::Label rLabel;

		int TEXTMARGIN{ 40 };

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRComponent)

	};

} // namespace cw::synth