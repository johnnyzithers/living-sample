/*
  ==============================================================================

    ThumbailComponent.h
    Created: 3 Jul 2022 12:49:50pm
    Author:  Duncan MacConnell

  ==============================================================================
*/


#pragma once

// example implementation of the Exercise 3
//==============================================================================
class ThumbnailComponent : public juce::Component,
                                 private juce::ChangeListener
{
public:
    ThumbnailComponent (int sourceSamplesPerThumbnailSample,
                              juce::AudioFormatManager& formatManager,
                              juce::AudioThumbnailCache& cache)
       : thumbnail (sourceSamplesPerThumbnailSample, formatManager, cache)
    {
        thumbnail.addChangeListener (this);
    }

    void setFile (const juce::File& file)
    {
        thumbnail.setSource (new juce::FileInputSource (file));
    }

    void paint (juce::Graphics& g) override
    {
        if (thumbnail.getNumChannels() == 0)
            paintIfNoFileLoaded (g);
        else
            paintIfFileLoaded (g);
    }

    void paintIfNoFileLoaded (juce::Graphics& g)
    {
        g.fillAll (juce::Colours::white);
        g.setColour (juce::Colours::darkgrey);
        g.drawFittedText ("No File Loaded", getLocalBounds(), juce::Justification::centred, 1);
    }

    void paintIfFileLoaded (juce::Graphics& g)
    {
        g.fillAll (juce::Colours::white);

        g.setColour (juce::Colours::red);
        thumbnail.drawChannels (g, getLocalBounds(), 0.0, thumbnail.getTotalLength(), 1.0f);
    }

    void changeListenerCallback (juce::ChangeBroadcaster* source) override
    {
        if (source == &thumbnail)
            thumbnailChanged();
    }

private:
    void thumbnailChanged()
    {
        repaint();
    }

    juce::AudioThumbnail thumbnail;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThumbnailComponent)
};
