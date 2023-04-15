#pragma once

#include <JuceHeader.h>

namespace cw::synth {

class AddSynthComponent : public juce::Component /*, public juce::Slider::Listener*/ {

	public:
		AddSynthComponent();
		void paint(juce::Graphics& g) override;
		void resized() override;
		double getHarmGainsVal(int harm) { return harmGains.at(harm)->getValue(); }
		void setHarmGainsVal(int harm, double value) { harmGains.at(harm)->setValue(value); }
		juce::Slider& const getHarmGainsComponent(int harm) { return *harmGains.at(harm); }
		void addListener(juce::Slider::Listener*);

	private:
		std::vector<std::unique_ptr<juce::Slider>> harmGains;
		std::vector<std::unique_ptr<juce::Label>> harmGainLabels;
		const int TEXTMARGIN{ 40 };

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AddSynthComponent)

	};

} // namespace cw::synth
