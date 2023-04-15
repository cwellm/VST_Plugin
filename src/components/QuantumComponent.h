#pragma once

#include <JuceHeader.h>
#include <string>

namespace cw::synth {

struct QPhi : public juce::Slider {};
struct QTheta : public juce::Slider {};

class QuantumComponent : public juce::Component {

	public:
		QuantumComponent();
		void paint(juce::Graphics& g) override;
		void resized() override;
		juce::Slider& getComponent(std::string);
		void addListener(juce::Slider::Listener*);


	private:
		QPhi phi;
		QTheta theta;
		juce::Label phiLabel;
		juce::Label thetaLabel;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (QuantumComponent)

};

} // namespace cw::synth