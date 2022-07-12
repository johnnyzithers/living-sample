
/*
  ==============================================================================

    SampleFile.h
    Created: 3 Jul 2022 12:49:50pm
    Author:  Duncan MacConnell

  ==============================================================================
*/


#pragma once
#include "ProcessorBase.h"
#include "SampleFileProcessor.h"

// example implementation of the Exercise 3
//==============================================================================
class SampleFileControl :  public juce::Component,
                    private juce::ChangeListener
{
public:
    SampleFileControl (SampleFileProcessor& sfProcToUse)
    :
        sampleProcessor(sfProcToUse),
        state (Stopped),
        thumbComponent (512, formatManager, thumbnailCache),
        thumbnailCache (5),
        positionOverlay (sampleProcessor, transportSource)
            
        
    {

//        sampleProcessor1 = sfProcToUse;
        addAndMakeVisible(&openButton);
        addAndMakeVisible(&triggerButton);
        
        openButton.setButtonText("open");
        triggerButton.setButtonText("play");
        
        setSize (400, 500);
        
        openButton.onClick = [this] { openButtonClicked(); };

        triggerButton.onClick = [this] { playButtonClicked(); };
        triggerButton.setColour (juce::TextButton::buttonColourId, juce::Colours::green);
        triggerButton.setEnabled (false);


        addAndMakeVisible (&levelSlider);
        levelSlider.setRange (0.0, 1.0);
        levelSlider.onValueChange = [this] { currentLevel = (float) levelSlider.getValue(); };

        addAndMakeVisible (&thumbComponent);
        addAndMakeVisible (&positionOverlay);
        
        formatManager.registerBasicFormats();

    }


    void paint (juce::Graphics& g) override
    {
        
        auto padding = 25;

        juce::Rectangle<int> thumbnailBounds (padding, padding, getWidth() - (2*padding), (getHeight() - 120)/5);
        thumbComponent.setBounds (thumbnailBounds);
        positionOverlay.setBounds (thumbnailBounds);
        
        openButton.setTopLeftPosition(padding, 0);
        openButton.setSize(100, padding);
        
        triggerButton.setTopLeftPosition(100+padding, 0);
        triggerButton.setSize(100, padding);
        
        levelSlider.setTopLeftPosition(200+padding,0);
        levelSlider.setSize(getWidth() - 200 - 1.5*padding, padding);
        

    }

    void changeListenerCallback (juce::ChangeBroadcaster* source) override
    {

    }

private:

    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping
    };
    
    void changeState (TransportState newState)
    {
        if (state != newState)
        {
            state = newState;

            switch (state)
            {
                case Stopped:
//                    stopButton.setEnabled (false);
                    triggerButton.setEnabled (true);
                    break;

                case Starting:
                    triggerButton.setEnabled (false);
//                    transportSource.start();
                    break;

                case Playing:
//                    stopButton.setEnabled (true);
                    break;

                case Stopping:
//                    transportSource.stop();
                    break;

                default:
                    jassertfalse;
                    break;
            }
        }
    }


    
    void openButtonClicked()
    {
        chooser = std::make_unique<juce::FileChooser> ("Select a Wave file to play...",
                                                       juce::File{},
                                                       "*.wav");
        auto chooserFlags = juce::FileBrowserComponent::openMode
                          | juce::FileBrowserComponent::canSelectFiles;

        chooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)
        {
            auto file = fc.getResult();

            if (file == juce::File{})
                return;

            std::unique_ptr<juce::AudioFormatReader> reader (formatManager.createReaderFor (file));

            if (reader.get() != nullptr)
            {
                auto duration = (float) reader->lengthInSamples / reader->sampleRate;

                if (duration < 10)
                {

                    // for thumbnail
                    // FIXME reader2..
                    auto* reader2 = formatManager.createReaderFor (file);
                    
                    // load this audio file into the processor
                    sampleProcessor.loadNewSample(*reader2);
                    
                    auto newSource = std::make_unique<juce::AudioFormatReaderSource> (reader2, true);
                    transportSource.setSource (newSource.get(), 0, nullptr, reader2->sampleRate);
                    triggerButton.setEnabled (true);
                    thumbComponent.setFile (file);
                    readerSource.reset (newSource.release());
                }
                else
                {
                    // handle the error that the file is 2 seconds or longer..
                }
            }
        });
    }

    void playButtonClicked()
    {
        changeState (Starting);
    }

    void stopButtonClicked()
    {
        changeState (Stopping);
    }

    float currentLevel = 0.0f, previousLevel = 0.0f;

    
    SampleFileProcessor& sampleProcessor;
    
    TransportState state;
    juce::AudioTransportSource transportSource;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

    juce::AudioFormatManager formatManager;
    juce::AudioSampleBuffer fileBuffer;

    ThumbnailComponent thumbComponent;
    juce::AudioThumbnailCache thumbnailCache;
    PositionOverlay positionOverlay;

    std::unique_ptr<juce::FileChooser> chooser;

    juce::TextButton openButton;
    juce::TextButton triggerButton;
    juce::Slider levelSlider;



    int position = 0;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleFileControl)
};
