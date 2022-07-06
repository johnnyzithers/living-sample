/*
  ==============================================================================

    SampleFile.h
    Created: 3 Jul 2022 12:49:50pm
    Author:  Duncan MacConnell

  ==============================================================================
*/


#pragma once
//#include "ProcessorBase.h"


// example implementation of the Exercise 3
//==============================================================================
class SampleFileProcessor : public juce::AudioSource
//public ProcessorBase

{
public:
    enum
    {
        maxMidiChannel = 16,
        maxNumberOfVoices = 5
    };

    SampleFileProcessor (juce::MidiKeyboardState& keyState, juce::MidiBuffer& buf)
        : keyboardState (keyState),
        incomingMidi(buf)
    {
        
        formatManager.registerBasicFormats();

        // one midi channel, one output
        for (auto i = 0; i < maxNumberOfVoices; ++i)                // [1]
            synth.addVoice (new juce::SamplerVoice());
        
    }
    
    
//    void prepareToPlay (double sampleRate, int samplesPerBlock) override
//    {
//        juce::ignoreUnused (samplesPerBlock);
//        synth.setCurrentPlaybackSampleRate (sampleRate);
//    }
    
    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        synth.setCurrentPlaybackSampleRate (sampleRate);
        midiCollector.reset(sampleRate);
    }

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
//        bufferToFill.clearActiveBufferRegion();


        keyboardState.processNextMidiBuffer (incomingMidi, bufferToFill.startSample,
                                             bufferToFill.numSamples, true);

        synth.renderNextBlock (*bufferToFill.buffer, incomingMidi,
                               bufferToFill.startSample, bufferToFill.numSamples);
    }
    
    void setIncomingMidi(juce::MidiBuffer& newMidi)
    {
        incomingMidi = newMidi;
    }

    juce::MidiMessageCollector* getMidiCollector()
    {
        return &midiCollector;
    }
    
    void releaseResources() override {}
    
    void loadNewSample(juce::AudioFormatReader& reader)
    {
        juce::BigInteger midiNotes;
        midiNotes.setRange (0, 126, true);
        juce::SynthesiserSound::Ptr newSound = new juce::SamplerSound("Voice", reader, midiNotes, 60, 0.0, 0.0, 10.0);
        sound = newSound;
        synth.removeSound(0);
        synth.addSound(sound);
        // set fileLoaded flag
        fileLoaded = true;
        
    }

private:
    
    float currentLevel = 0.0f, previousLevel = 0.0f;
    
    juce::MidiBuffer& incomingMidi;
    

    juce::AudioTransportSource transportSource;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

    juce::AudioFormatManager formatManager;


    juce::Synthesiser synth;
    juce::SynthesiserSound::Ptr sound;
    juce::MidiMessageCollector midiCollector;
    juce::MidiKeyboardState& keyboardState;

    int position = 0;
    bool fileLoaded = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleFileProcessor)
};
