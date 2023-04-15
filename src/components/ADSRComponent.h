#pragma once

#include <string>
#include <JuceHeader.h>

namespace cw::synth {

class ADSRComponent : public juce::Component /*, public juce::Slider::Listener*/ {

	public:
		ADSRComponent();
		void paint(juce::Graphics& g) override;
		void resized() override;

		juce::Slider& const getComponent(std::string);
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