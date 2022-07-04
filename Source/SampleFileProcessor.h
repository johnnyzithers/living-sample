/*
  ==============================================================================

    SampleFile.h
    Created: 3 Jul 2022 12:49:50pm
    Author:  Duncan MacConnell

  ==============================================================================
*/


#pragma once
#include "ProcessorBase.h"


// example implementation of the Exercise 3
//==============================================================================
class SampleFileProcessor : public ProcessorBase

{
public:
    SampleFileProcessor ()
    {
        
        formatManager.registerBasicFormats();

    }
    
    
    void prepareToPlay (double sampleRate, int samplesPerBlock) override
    {



    }

    void processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override
    {
        auto level = currentLevel;
        auto startLevel = level == previousLevel ? level : previousLevel;

        auto numInputChannels = fileBuffer.getNumChannels();
        auto numOutputChannels = buffer.getNumChannels();

        auto outputSamplesRemaining = buffer.getNumSamples();
        auto outputSamplesOffset = 0;

        while (outputSamplesRemaining > 0)
        {
            auto bufferSamplesRemaining = fileBuffer.getNumSamples() - position;
            auto samplesThisTime = juce::jmin (outputSamplesRemaining, bufferSamplesRemaining);

            for (auto channel = 0; channel < numOutputChannels; ++channel)
            {
                buffer.copyFrom (channel,
                                   outputSamplesOffset,
                                   fileBuffer,
                                   channel % numInputChannels,
                                   position,
                                   samplesThisTime);

                buffer.applyGainRamp (channel, outputSamplesOffset, samplesThisTime, startLevel, level);
            }

            outputSamplesRemaining -= samplesThisTime;
            outputSamplesOffset += samplesThisTime;
            position += samplesThisTime;

            if (position == fileBuffer.getNumSamples())
                position = 0;
        }
        
        previousLevel = level;
        
    }

    void reset() override
    {
//        filter.reset();
    }
    

private:


    float currentLevel = 0.0f, previousLevel = 0.0f;

    
//    TransportState state;
    juce::AudioTransportSource transportSource;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

    juce::AudioFormatManager formatManager;
    juce::AudioSampleBuffer fileBuffer;


    int position = 0;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleFileProcessor)
};
