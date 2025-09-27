/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/

// Create a custom look and feel which will be used for the gain slider
class CustomLnF : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle,
        float rotaryEndAngle, juce::Slider& slider) override
    {
        auto diameter = std::min(width, height);
        auto bounds = juce::Rectangle<float>(x, y, diameter, diameter).reduced(25);
        auto reducedRadius = (diameter / 2.0f) - 15;
        auto center = bounds.getCentre();
        auto rotaryCenterAngle = (rotaryStartAngle + rotaryEndAngle) / 2;

        g.setColour(juce::Colours::darkgrey);
        g.fillEllipse(bounds);

        juce::Path backgroundArc;
        backgroundArc.addCentredArc(center.x, center.y, reducedRadius, reducedRadius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
        g.setColour(juce::Colours::darkslategrey);
        g.strokePath(backgroundArc, juce::PathStrokeType(10.0f));

        juce::Path arc;
        auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
        arc.addCentredArc(center.x, center.y, reducedRadius, reducedRadius, 0.0f, rotaryCenterAngle, angle, true);
        g.setColour(juce::Colours::orange);
        g.strokePath(arc, juce::PathStrokeType(4.0f));

        juce::Path p;
        auto pointerLength = reducedRadius - 10;
        auto pointerThickness = 4.0f;
        p.addRectangle(-pointerThickness * 0.5f, -pointerLength, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(bounds.getCentreX(), bounds.getCentreY()));
        g.fillPath(p);
    }
};

class GainAudioProcessorEditor : public juce::AudioProcessorEditor,
    private juce::Timer
{
public:
    GainAudioProcessorEditor (GainAudioProcessor&);
    ~GainAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& e) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GainAudioProcessor& audioProcessor;
    void timerCallback() override;

    juce::Slider gainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;
    CustomLnF customLnF;
    
    juce::Label gainLabel;
    juce::Label peakHeader;
    juce::Label peakLabel;
    juce::Label clipWarning;
    juce::Component clipLED;

    float peakDisplay = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainAudioProcessorEditor)
};
