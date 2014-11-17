/*
  ==============================================================================

    VisualMaker.cpp
    Created: 16 Nov 2014 8:21:51pm
    Author:  Jeffrey Robbins

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "VisualMaker.h"

//==============================================================================
VisualMaker::VisualMaker(File file) : deviceManager(*new AudioDeviceManager()),
                                      thread("visualizer") 
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSize (500, 400);
    transportSource.setSource (nullptr);
    formatManager.registerBasicFormats();
    thread.startThread (3);
    deviceManager.addAudioCallback (&audioSourcePlayer);
    audioSourcePlayer.setSource (&transportSource);
    deviceManager.initialise (2, 2, 0, true, String::empty, 0);
    
    AudioFormatReader* reader = formatManager.createReaderFor(file);
    currentAudioFileSource = new AudioFormatReaderSource (reader, true);
    
    // ..and plug it into our transport source
    transportSource.setSource (currentAudioFileSource,
                               32768,                   // tells it to buffer this many samples ahead
                               &thread,                 // this is the background thread to use for reading-ahead
                               reader->sampleRate);     // allows for sample rate correction
    deviceManager.enableInputLevelMeasurement(true);
    transportSource.setPosition (0);
    transportSource.start();
    
    startTimer (1000 / 40);

}

VisualMaker::~VisualMaker()
{
}

void VisualMaker::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (Colours::white);   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::lightblue);
    g.setFont (14.0f);
    g.drawText ((String)pitchFinder.getPitch(), getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void VisualMaker::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
