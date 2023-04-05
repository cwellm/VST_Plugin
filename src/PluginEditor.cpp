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

    for (int i = 0; i < NO_ADDSYNTH_VOICES; ++i) {
        harmGains.push_back(std::make_unique<juce::Slider>());
    }

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(500, 550);

    // harmonic gains
    for (int i = 0; i < NO_ADDSYNTH_VOICES; ++i) {
        (*harmGains.at(i)).setSliderStyle(juce::Slider::LinearBar);
        (*harmGains.at(i)).setRange(0.0, 1.0, 0.02);
        (*harmGains.at(i)).setValue(p.paramHarmGains.at(i)->get());
        (*harmGains.at(i)).setName("HGain " + std::to_string(i));
        this->addAndMakeVisible(harmGains.at(i).get());
        (*harmGains.at(i)).addListener(this);
    }

    // ADSR
    aVal.setSliderStyle(juce::Slider::LinearBar);
    aVal.setRange(0.0, 1.0, 0.01);
    aVal.setValue(p.paramA->get());
    aVal.setName("Attack");
    this->addAndMakeVisible(aVal);
    aVal.addListener(this);

    dVal.setSliderStyle(juce::Slider::LinearBar);
    dVal.setRange(0.0, 1.0, 0.01);
    dVal.setValue(p.paramD->get());
    dVal.setName("Attack");
    this->addAndMakeVisible(dVal);
    dVal.addListener(this);

    sVal.setSliderStyle(juce::Slider::LinearBar);
    sVal.setRange(0.0, 1.0, 0.01);
    sVal.setValue(p.paramS->get());
    sVal.setName("Attack");
    this->addAndMakeVisible(sVal);
    sVal.addListener(this);

    rVal.setSliderStyle(juce::Slider::LinearBar);
    rVal.setRange(0.0, 1.0, 0.01);
    rVal.setValue(p.paramR->get());
    rVal.setName("Attack");
    this->addAndMakeVisible(rVal);
    rVal.addListener(this);

    // Quantum Angles
    phiVal.setSliderStyle(juce::Slider::LinearBar);
    phiVal.setRange(0.0, juce::MathConstants<float>::pi * 2, 0.01);
    phiVal.setValue(p.paramPhi->get());
    phiVal.setName("Phi");
    this->addAndMakeVisible(phiVal);
    phiVal.addListener(this);

    thetaVal.setSliderStyle(juce::Slider::LinearBar);
    thetaVal.setRange(0.0, juce::MathConstants<float>::pi * 2, 0.01);
    thetaVal.setValue(p.paramTheta->get());
    thetaVal.setName("Theta");
    this->addAndMakeVisible(thetaVal);
    thetaVal.addListener(this);
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
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void NewProjectAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    for (int i = 0; i < NO_ADDSYNTH_VOICES; ++i) {
        (*harmGains.at(i)).setBounds(20, i*30 + 20, 200, 20);
    }

    aVal.setBounds(250, 50, 150, 20);
    dVal.setBounds(250, 110, 150, 20);
    sVal.setBounds(250, 170, 150, 20);
    rVal.setBounds(250, 230, 150, 20);

    phiVal.setBounds(250, 290, 150, 20);
    thetaVal.setBounds(250, 350, 150, 20);
}

//==============================================================================
void NewProjectAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
    for (int i = 0; i < NO_ADDSYNTH_VOICES; ++i) {
        if (slider == harmGains.at(i).get()) {
            *audioProcessor.paramHarmGains.at(i) = slider->getValue();
        }
    }

    if (slider == &aVal) {
        *audioProcessor.paramA = slider->getValue();
    }

    if (slider == &dVal) {
        *audioProcessor.paramD = slider->getValue();
    }

    if (slider == &sVal) {
        *audioProcessor.paramS = slider->getValue();
    }
    if (slider == &rVal) {
        *audioProcessor.paramR = slider->getValue();
    }

    if (slider == &phiVal) {
        *audioProcessor.paramPhi = slider->getValue();
    }

    if (slider == &thetaVal) {
        *audioProcessor.paramTheta = slider->getValue();
    }

}