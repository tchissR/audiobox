/*
  ==============================================================================

    AutoPan.h
    Created: 22 Nov 2020 3:07:59pm
    Author:  Mathis Raibaud

  ==============================================================================
*/

#pragma once

#include <juce_dsp/juce_dsp.h>

class AutoPan : public juce::dsp::ProcessorBase
{
public:
    
    AutoPan();
    
    void prepare(const juce::dsp::ProcessSpec& spec) override;
    void process (const juce::dsp::ProcessContextReplacing<float>& context) override;
    void reset() override;
    
    void setAmount(float amount);
    void setFrequency(float frequency);
    void setPhaseOffset(float offset);
    
private:
    
    double sampleRate{};
    std::array<juce::dsp::Oscillator<float>, 2> lfos;
    std::array<juce::dsp::Gain<float>, 2> gains;
    juce::SmoothedValue<float> amount;
    juce::SmoothedValue<float> offset;
    
};
