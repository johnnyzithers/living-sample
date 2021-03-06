/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ThumbnailComponent.h"
#include "PositionOverlay.h"
#include "SampleFileProcessor.h"

//==============================================================================
/**
*/
class LivingSampleAudioProcessor  : public juce::AudioProcessor
//,
//                                    private juce::ChangeListener

{
public:
    //==============================================================================
    LivingSampleAudioProcessor();
    ~LivingSampleAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
//    FIXME
    SampleFileProcessor sampleProcessor1;
    SampleFileProcessor sampleProcessor2;
    SampleFileProcessor sampleProcessor3;
    SampleFileProcessor sampleProcessor4;
    
    juce::MidiKeyboardState keyboardState;
    
private:
    static juce::MidiBuffer filterMidiMessagesForChannel (const juce::MidiBuffer& input, int channel);
    
    juce::AudioProcessorValueTreeState parameters;

    // FIXME
    juce::MidiBuffer temp;


    
    std::atomic<float>* stopParameter1  = nullptr;
    std::atomic<float>* playParameter1  = nullptr;
    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LivingSampleAudioProcessor)
};
