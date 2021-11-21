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
class DistortionPedalAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DistortionPedalAudioProcessorEditor (DistortionPedalAudioProcessor&);
    ~DistortionPedalAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DistortionPedalAudioProcessor& audioProcessor;
    juce::Slider driveKnob;
    
public:
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> driveKnobValue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionPedalAudioProcessorEditor)
};


//#pragma once
//
//#include "../JuceLibraryCode/JuceHeader.h"
//#include "PluginProcessor.h"
//
////==============================================================================
///**
//*/
//class DistortionAudioProcessorEditor  : public AudioProcessorEditor
//{
//public:
//    DistortionAudioProcessorEditor (DistortionAudioProcessor&);
//    ~DistortionAudioProcessorEditor();
//
//    //==============================================================================
//    void paint (Graphics&) override;
//    void resized() override;
//
//private:
//    DistortionAudioProcessor& processor;
//    Slider driveKnob;
//    Slider rangeKnob;
//    Slider blendKnob;
//    Slider volumeKnob;
//    Label driveLabel;
//    Label rangeLabel;
//    Label blendLabel;
//    Label volumeLabel;
//
//public:
//    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> driveKnobValue;
//    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> rangeKnobValue;
//    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> blendKnobValue;
//    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> volumeKnobValue;
//
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionAudioProcessorEditor)
//};
