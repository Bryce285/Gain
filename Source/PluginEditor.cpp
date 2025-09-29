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

    gainSlider.setLookAndFeel(&customLnF);
    gainSlider.setSliderStyle(juce::Slider::Rotary);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    gainSlider.setTextValueSuffix(" dB");
    gainSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour(textColour));
    gainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xFF141414));
    gainSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xFF141414));
    addAndMakeVisible(gainSlider);
    
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN", gainSlider);

    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.attachToComponent(&gainSlider, false);
    gainLabel.setJustificationType(juce::Justification::centred);
    gainLabel.setColour(juce::Label::textColourId, juce::Colour(textColour));
    addAndMakeVisible(gainLabel);

    peakHeader.setText("Peak", juce::dontSendNotification);
    peakHeader.setJustificationType(juce::Justification::centred);
    peakHeader.setColour(juce::Label::textColourId, juce::Colour(textColour));
    addAndMakeVisible(peakHeader);

    peakLabel.setText("-Inf dB", juce::dontSendNotification);
    peakLabel.setJustificationType(juce::Justification::centredBottom);
    peakLabel.addMouseListener(this, true);
    peakLabel.setColour(juce::Label::textColourId, juce::Colour(textColour));
    peakLabel.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF141414));
    addAndMakeVisible(peakLabel);

    clipWarning.setText("Clip", juce::dontSendNotification);
    clipWarning.setJustificationType(juce::Justification::centredTop);
    clipWarning.addMouseListener(this, true);
    clipWarning.setColour(juce::Label::textColourId, juce::Colour(textColour));
    addAndMakeVisible(clipWarning);
}

GainAudioProcessorEditor::~GainAudioProcessorEditor()
{
    gainSlider.setLookAndFeel(nullptr);
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
    g.fillAll (juce::Colour(0xFF222222));

    g.setColour(juce::Colour(textColour));
    g.setFont(20.0f);

    if (peakDisplay == -100.0f) {
        peakLabel.setText("-Inf dB", juce::dontSendNotification);
    }
    else {
        peakLabel.setText(juce::String(peakDisplay, 2) + " dB", juce::dontSendNotification);
    }

    auto clipLEDBounds = juce::Rectangle<float>(317, 414, 15, 15);
    float ledDiameter = 15.0f;
    juce::Rectangle<float> ledArea(clipLEDBounds.getCentreX() - ledDiameter * 0.5f,
        clipLEDBounds.getCentreY() - ledDiameter * 0.5f,
        ledDiameter, ledDiameter);
    bool ledOn;

    if (audioProcessor.isClipping) {
        ledOn = true;
    }
    else {
        ledOn = false;
    }

    juce::Colour ledColour = ledOn ? juce::Colours::red : juce::Colours::darkgrey;

    if (ledOn) {
        // Glow layer
        //g.setColour(ledColour.withAlpha(0.3f));
        //g.fillEllipse(ledArea.expanded(4.0f));

        // Base layer
        g.setColour(ledColour);
        g.fillEllipse(ledArea);

        // Outline layer
        g.setColour(juce::Colour(0xFF222222));
        g.drawEllipse(ledArea, 3.0f);

        g.setColour(juce::Colours::darkgrey);
        g.drawEllipse(ledArea, 2.0f);
    }
    else {
        // Base layer
        g.setColour(ledColour);
        g.fillEllipse(ledArea);

        // Outline layer
        g.setColour(juce::Colour(0xFF222222));
        g.drawEllipse(ledArea, 3.0f);

        g.setColour(juce::Colours::darkgrey);
        g.drawEllipse(ledArea, 2.0f);
    }
}

void GainAudioProcessorEditor::mouseDown(const juce::MouseEvent& e)
{
    if (e.eventComponent == &peakLabel || e.eventComponent == &clipWarning || e.eventComponent == &peakHeader) {
        audioProcessor.isClipping = false;
        audioProcessor.currentPeak.store(0.0f);
    }
}

void GainAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    gainSlider.setBounds(50, 50, 300, 325);
    peakHeader.setBounds(50, 390, 50, 20);
    peakLabel.setBounds(50, 412, 50, 16);
    clipWarning.setBounds(300, 390, 50, 35);
}
