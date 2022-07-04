/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LivingSampleAudioProcessorEditor::LivingSampleAudioProcessorEditor (LivingSampleAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
      : AudioProcessorEditor (&p),
        audioProcessor (p),
        valueTreeState(vts),
        sample1Control(p.sampleProcessor1)
        
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    
    setSize (400, 500);

    
    addAndMakeVisible(&sample1Control);
    
}

LivingSampleAudioProcessorEditor::~LivingSampleAudioProcessorEditor()
{
}

//==============================================================================
void LivingSampleAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (22.0f);
    g.drawFittedText ("L I V I N G sample", 25, 400, 200, 25, juce::Justification::centred, 1);
    
    sample1Control.setTopLeftPosition(10, 10);
    sample1Control.setBounds(10, 10, getWidth() - 20, getWidth() - 20);
}

void LivingSampleAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    
}

void LivingSampleAudioProcessorEditor::changeListenerCallback (juce::ChangeBroadcaster* source)
{
//    if (source == &transportSource) transportSourceChanged();
//    if (source == &thumbnail)       thumbnailChanged();
}

void LivingSampleAudioProcessorEditor::openButtonClicked
()
{
    
}
