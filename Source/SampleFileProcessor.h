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
    enum
    {
        maxMidiChannel = 16,
        maxNumberOfVoices = 5
    };

    SampleFileProcessor ()
    {
        
        formatManager.registerBasicFormats();
        
        // one midi channel, one output
        for (auto i = 0; i < 5; ++i)                // [1]
            synth.addVoice (new juce::SamplerVoice());
//        loadNewSample();
        
    }
    
    
    void prepareToPlay (double sampleRate, int samplesPerBlock) override
    {
        juce::ignoreUnused (samplesPerBlock);
        synth.setCurrentPlaybackSampleRate (sampleRate);
    }

    void processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiBuffer) override
    {
        if(fileLoaded)
        {
//            auto busCount = getBusCount (false);                // [11]
     
//            for (auto busNr = 0; busNr < busCount; ++busNr)     // [12]
//            {
//                auto midiChannelBuffer = filterMidiMessagesForChannel (midiBuffer, busNr + 1);
//                auto audioBusBuffer = getBusBuffer (buffer, false, 0);
     
                synth.renderNextBlock (buffer, midiBuffer, 0, buffer.getNumSamples()); // [13]
//            }
        }
    }

    void reset() override
    {

    }
    
    void loadNewSample(juce::AudioFormatReader& reader)
    {
        fileLoaded = true;
        
        juce::BigInteger midiNotes;
        midiNotes.setRange (0, 126, true);
        juce::SynthesiserSound::Ptr newSound = new juce::SamplerSound("Voice", reader, midiNotes, 0x40, 0.0, 0.0, 10.0);
        sound = newSound;
        synth.removeSound(0);
        synth.addSound(sound);
        
    }
    
    bool canAddBus    (bool isInput) const override   { return (! isInput && getBusCount (false) < maxMidiChannel); }
    bool canRemoveBus (bool isInput) const override   { return (! isInput && getBusCount (false) > 1); }


private:

    static juce::MidiBuffer filterMidiMessagesForChannel (const juce::MidiBuffer& input, int channel)
    {
        juce::MidiBuffer output;

        for (auto metadata : input)     // [14]
        {
            auto message = metadata.getMessage();

            if (message.getChannel() == channel)
                output.addEvent (message, metadata.samplePosition);
        }

        return output;                  // [15]
    }
    
    float currentLevel = 0.0f, previousLevel = 0.0f;
 
    
//    TransportState state;
    juce::AudioTransportSource transportSource;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

    juce::AudioFormatManager formatManager;
    juce::AudioSampleBuffer fileBuffer;

    juce::Synthesiser synth;
    juce::SynthesiserSound::Ptr sound;

    int position = 0;
    bool fileLoaded = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleFileProcessor)
};
