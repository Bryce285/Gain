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
    startTimer(25);

    gainSlider.setSliderStyle(juce::Slider::Rotary);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    gainSlider.setTextValueSuffix(" dB");
    addAndMakeVisible(gainSlider);
    
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN", gainSlider);

    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.attachToComponent(&gainSlider, false);
    gainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(gainLabel);

    peakLabel.setText("Peak: -inf dB", juce::dontSendNotification);
    peakLabel.setJustificationType(juce::Justification::centred);
    peakLabel.addMouseListener(this, true);
    addAndMakeVisible(peakLabel);

    clipWarning.setText("CLIP", juce::dontSendNotification);
    clipWarning.setJustificationType(juce::Justification::centred);
    clipWarning.setColour(juce::Label::textColourId, juce::Colours::dimgrey);
    clipWarning.addMouseListener(this, true);
    addAndMakeVisible(clipWarning);
}

GainAudioProcessorEditor::~GainAudioProcessorEditor()
{
}

//==============================================================================
void GainAudioProcessorEditor::timerCallback()
{
    peakDisplay = juce::Decibels::gainToDecibels(audioProcessor.currentPeak.load());
    repaint();
}

//==============================================================================
void GainAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(20.0f);

    if (peakDisplay == -100.0f) {
        peakLabel.setText("Peak: -inf dB", juce::dontSendNotification);
    }
    else {
        peakLabel.setText("Peak: " + juce::String(peakDisplay, 2) + " dB", juce::dontSendNotification);
    }

    if (audioProcessor.isClipping) {
        clipWarning.setColour(juce::Label::textColourId, juce::Colours::red);
    }
}

void GainAudioProcessorEditor::mouseDown(const juce::MouseEvent& e)
{
    if (e.eventComponent == &peakLabel || e.eventComponent == &clipWarning) {
        audioProcessor.isClipping = false;
        clipWarning.setColour(juce::Label::textColourId, juce::Colours::dimgrey);
        audioProcessor.currentPeak.store(0.0f);
    }
}

void GainAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    gainSlider.setBounds(0, 35, 400, 400);

    // TODO: Make these centered in editor window
    peakLabel.setBounds(10, 10, 100, 20);
    clipWarning.setBounds(330, 10, 80, 20);
}
