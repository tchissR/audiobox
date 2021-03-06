/*
  ==============================================================================

    AutoPan.cpp
    Created: 22 Nov 2020 3:07:59pm
    Author:  Mathis Raibaud

  ==============================================================================
*/

#include "AutoPan.h"

AutoPan::AutoPan() :
sampleRate(0)
{
    offset.setCurrentAndTargetValue(180.f);
    lfos[0].initialise([] (float x) { return -0.5f + 0.5f * std::sin(x) ; } );
    lfos[1].initialise([&] (float x) { return -0.5f + 0.5f * std::sin(x + (offset.getNextValue() * juce::MathConstants<float>::pi / 180.f)) ; } );
    lfos[0].setFrequency(2.f);
    lfos[1].setFrequency(2.f);
}

void AutoPan::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    
    for (size_t i = 0; i < 2; ++i)
    {
        lfos[i].prepare(spec);
        gains[i].prepare(spec);
    }
    
    amount.setCurrentAndTargetValue(1.f);
    amount.reset(sampleRate, 0.05);
    offset.reset(sampleRate, 0.05);
}

void AutoPan::process(const juce::dsp::ProcessContextReplacing<float> &context)
{
    const auto& inputBlock = context.getInputBlock();
    auto& outputBlock      = context.getOutputBlock();
    const auto numChannels = outputBlock.getNumChannels();
    const auto numSamples  = outputBlock.getNumSamples();
    
    jassert (inputBlock.getNumChannels() == numChannels);
    jassert (inputBlock.getNumChannels() == 2);
    jassert (inputBlock.getNumSamples()  == numSamples);
    
    for (size_t channel = 0; channel < numChannels; ++channel)
    {
        auto* inputSamples = inputBlock.getChannelPointer (channel);
        auto* outputSamples = outputBlock.getChannelPointer (channel);
        
        auto& lfo = lfos[channel];
        auto& gain = gains[channel];

        for (size_t i = 0; i < numSamples; ++i)
        {
            auto lfoValue = amount.getNextValue() * lfo.processSample(0);
            gain.setGainLinear(1.f + lfoValue);
//            auto gainDecibels = juce::jmap(1.f + lfoValue, 0.f, 1.f, -30.f, 0.f);
//            gain.setGainDecibels(gainDecibels);
            
            outputSamples[i] = gain.processSample(inputSamples[i]);
        }
    }
}

void AutoPan::setFrequency(float frequency)
{
    jassert(juce::isPositiveAndBelow(frequency, sampleRate));
    
    for (auto& lfo : lfos)
        lfo.setFrequency(frequency);
}

void AutoPan::setAmount(float amount)
{
    this->amount.setTargetValue(amount);
}

void AutoPan::setPhaseOffset(float offset)
{
    this->offset.setTargetValue(offset);
}

void AutoPan::reset()
{
    for (auto& lfo : lfos)
        lfo.reset();
    
    for (auto& gain : gains)
        gain.reset();
}
