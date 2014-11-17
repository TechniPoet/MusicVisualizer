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
VisualMaker::VisualMaker(File file) 
	: deviceManager(*new AudioDeviceManager()),
	  meterThread("Meter Thread"),
	  thread("temp Thread")
{
    setSize (500, 400);
    transportSource.setSource (nullptr);
    formatManager.registerBasicFormats();
	addAndMakeVisible (&meterL);
    addAndMakeVisible (&meterR);
	meterThread.addTimeSliceClient (&meterL);
    meterThread.addTimeSliceClient (&meterR);
	
	meterThread.startThread (1);
	thread.startThread(3);
    deviceManager.addAudioCallback (&audioSourcePlayer);
    audioSourcePlayer.setSource (&transportSource);
    deviceManager.initialise (1, 2, 0, true, String::empty, 0);
    
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

    g.fillAll (Colours::white);   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::lightblue);
    g.setFont (14.0f);
    g.drawText ((String)pitchFinder.getPitch(), getLocalBounds(),
                Justification::centred, true);  
}

void VisualMaker::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
