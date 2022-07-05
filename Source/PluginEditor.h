/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SampleFileControl.h"

//==============================================================================
/**
*/
class LivingSampleAudioProcessorEditor  :   public juce::AudioProcessorEditor,
                                            private juce::ChangeListener
{
public:
    LivingSampleAudioProcessorEditor (LivingSampleAudioProcessor&, juce::AudioProcessorValueTreeState&);

    ~LivingSampleAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    void openButtonClicked ();

private:


    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    LivingSampleAudioProcessor& audioProcessor;
    
    SampleFileControl sample1Control;
    SampleFileControl sample2Control;
    SampleFileControl sample3Control;
    SampleFileControl sample4Control;

    juce::AudioProcessorValueTreeState& valueTreeState;


    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LivingSampleAudioProcessorEditor)
};
