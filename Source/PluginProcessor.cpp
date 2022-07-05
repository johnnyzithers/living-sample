/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LivingSampleAudioProcessor::LivingSampleAudioProcessor()

            : AudioProcessor (BusesProperties()
                  .withOutput ("Output #1",  juce::AudioChannelSet::stereo(), true)
                  .withOutput ("Output #2",  juce::AudioChannelSet::stereo(), false)
                  .withOutput ("Output #3",  juce::AudioChannelSet::stereo(), false)
                  .withOutput ("Output #4",  juce::AudioChannelSet::stereo(), false)
                  .withOutput ("Output #5",  juce::AudioChannelSet::stereo(), false)),
            parameters (*this, nullptr, juce::Identifier ("LivingSampleParams"),
              {
                  std::make_unique<juce::AudioParameterBool> ("play1",
                                                               "Play Button 1",
                                                              false),
                  std::make_unique<juce::AudioParameterBool> ("stop1",
                                                              "Stop Button 1",
                                                              false)
              }),
            sampleProcessor1(keyboardState, temp),
            sampleProcessor2(keyboardState, temp),
            sampleProcessor3(keyboardState, temp),
            sampleProcessor4(keyboardState, temp)
{

//    addAndMakeVisible(&sampleFile1);
    stopParameter1  = parameters.getRawParameterValue ("stop1");
    playParameter1  = parameters.getRawParameterValue ("play1");

}

LivingSampleAudioProcessor::~LivingSampleAudioProcessor()
{
}

//==============================================================================
const juce::String LivingSampleAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LivingSampleAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LivingSampleAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LivingSampleAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LivingSampleAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LivingSampleAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LivingSampleAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LivingSampleAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String LivingSampleAudioProcessor::getProgramName (int index)
{
    return {};
}

void LivingSampleAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void LivingSampleAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    sampleProcessor1.prepareToPlay(sampleRate, samplesPerBlock);

}

void LivingSampleAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.


}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LivingSampleAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void LivingSampleAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
//    juce::ScopedNoDenormals noDenormals;
//    auto totalNumInputChannels  = getTotalNumInputChannels();
//    auto totalNumOutputChannels = getTotalNumOutputChannels();

//
    
    auto midiChannelBuffer = filterMidiMessagesForChannel (midiMessages, 1);
    sampleProcessor1.setIncomingMidi(midiChannelBuffer);
    
    juce::AudioSourceChannelInfo info (&buffer, 0, buffer.getNumSamples());
    sampleProcessor1.getNextAudioBlock(info);

    
    
}

    juce::MidiBuffer LivingSampleAudioProcessor::filterMidiMessagesForChannel (const juce::MidiBuffer& input, int channel)
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

//==============================================================================
bool LivingSampleAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* LivingSampleAudioProcessor::createEditor()
{
//    return new LivingSampleAudioProcessorEditor (*this);
    return new LivingSampleAudioProcessorEditor (*this, parameters);

}

//==============================================================================
void LivingSampleAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void LivingSampleAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LivingSampleAudioProcessor();
}

