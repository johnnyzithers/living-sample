/*
  ==============================================================================

    PositionOverlay.h
    Created: 3 Jul 2022 12:55:04pm
    Author:  Duncan MacConnell

  ==============================================================================
*/

#pragma once
#include "SampleFileProcessor.h"

class PositionOverlay : public juce::Component,
                              private juce::Timer
{
public:
    PositionOverlay (SampleFileProcessor& sampleProcessorToUse, juce::AudioTransportSource& transportSourceToUse)
    :    transportSource (transportSourceToUse),
        sampleProcessor (sampleProcessorToUse)

    {
        startTimer (40);
    }

    void paint (juce::Graphics& g) override
    {
        
        
        auto duration = (float) transportSource.getLengthInSeconds();

        if (duration > 0.0)
        {
            auto audioPosition = (float) transportSource.getCurrentPosition();
            auto drawPosition = (audioPosition / duration) * (float) getWidth();

            g.setColour (juce::Colours::green);
            g.drawLine (drawPosition, 0.0f, drawPosition, (float) getHeight(), 2.0f);
        }
        
        
    }

    void mouseDown (const juce::MouseEvent& event) override
    {
        auto duration = transportSource.getLengthInSeconds();

        if (duration > 0.0)
        {
            auto clickPosition = event.position.x;
            auto audioPosition = (clickPosition / (float) getWidth()) * duration;

            transportSource.setPosition (audioPosition);
        }
    }

private:
    void timerCallback() override
    {
        repaint();
    }

    juce::AudioTransportSource& transportSource;
    SampleFileProcessor& sampleProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PositionOverlay)
};
