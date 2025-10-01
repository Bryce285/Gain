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

// Create a custom look and feel for the GUI elements
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
        g.setColour(juce::Colour(0xFF3B3B3B));
        g.strokePath(backgroundArc, juce::PathStrokeType(10.0f));

        juce::Path arc;
        auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
        arc.addCentredArc(center.x, center.y, reducedRadius, reducedRadius, 0.0f, rotaryCenterAngle, angle, true);
        g.setColour(juce::Colour(0xFF0EA7B5));
        g.strokePath(arc, juce::PathStrokeType(4.0f));

        juce::Path innerCircle;
        innerCircle.addEllipse(bounds.reduced(6));
        g.setColour(juce::Colour(0xFF3B3B3B).withAlpha(0.7f));
        g.strokePath(innerCircle, juce::PathStrokeType(4.0f));

        juce::Path p;
        auto pointerLength = reducedRadius - 65;
        auto pointerThickness = 4.0f;
        p.addRectangle(-pointerThickness * 0.5f, -pointerLength - 55, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(bounds.getCentreX(), bounds.getCentreY()));
        g.setColour(juce::Colour(0xFF0EA7B5));
        g.fillPath(p);
    }

    void drawPluginHeader(juce::Graphics& g, int editorWidth)
    {
        juce::Path headerRect;
        int headerHeight = 40;
        headerRect.addRectangle(0, 0, editorWidth, headerHeight);
        g.setColour(juce::Colour(0xFF181818));
        g.fillPath(headerRect);
    }

    CustomLnF()
    {
        juce::FontOptions titlesFont = juce::Typeface::createSystemTypefaceFor(
            BinaryData::MoonGlossDisplayMedium_otf,
            BinaryData::MoonGlossDisplayMedium_otfSize
        );

        juce::FontOptions audioParamsFont = juce::Typeface::createSystemTypefaceFor(
            BinaryData::ZF2334SquarishRegular_otf,
            BinaryData::ZF2334SquarishRegular_otfSize
        );

        moonGloss = juce::Font(titlesFont).withHeight(14.0f);
        squarish = juce::Font(audioParamsFont).withHeight(20.0f);
    }

    juce::Font getLabelFont(juce::Label& label) override
    {
        return squarish;
    }

    juce::Font getTitlesFont()
    {
        return moonGloss;
    }

    juce::Font getAudioParamsFont()
    {
        return squarish;
    }

private:
    juce::Font moonGloss;
    juce::Font squarish;
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

    int textColour = 0xFFADB5BD;

    float peakDisplay = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainAudioProcessorEditor)
};
