/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "PluginProcessor.h"

//==============================================================================
/**
*/
class AutoPanAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AutoPanAudioProcessorEditor (AutoPanAudioProcessor&);
    ~AutoPanAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AutoPanAudioProcessor& audioProcessor;
    
    juce::Slider amountSlider;
    juce::Slider frequencySlider;
    juce::Slider phaseSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AutoPanAudioProcessorEditor)
};
