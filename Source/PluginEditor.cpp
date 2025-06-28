/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainAudioProcessorEditor::GainAudioProcessorEditor (GainAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 450);

    gainSlider.setSliderStyle(juce::Slider::Rotary);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 20);
    gainSlider.setTextValueSuffix(" db");
    addAndMakeVisible(gainSlider);
    
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN", gainSlider);

    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.attachToComponent(&gainSlider, false);
    gainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(gainLabel);
}

GainAudioProcessorEditor::~GainAudioProcessorEditor()
{
}

//==============================================================================
void GainAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void GainAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    gainSlider.setBounds(0, 35, 400, 400);
}
