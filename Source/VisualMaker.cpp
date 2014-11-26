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
	  bufferTransformAudioSource(&audioFilePlayer),
	  meterThread("Meter Thread")//,thread("temp Thread")
{

	//audio
    setSize (500, 400);
    formatManager.registerBasicFormats();
	addAndMakeVisible (&meterL);
    addAndMakeVisible (&meterR);
	meterThread.addTimeSliceClient (&meterL);
    meterThread.addTimeSliceClient (&meterR);
	
	meterThread.startThread (1);
	//thread.startThread(3);
	audioSourcePlayer.setSource (&bufferTransformAudioSource);
    deviceManager.initialise (1, 2, nullptr, true);
	deviceManager.addAudioCallback (this);

	audioFilePlayer.fileChanged(file);
	audioFilePlayer.pause();
    
   //AudioFormatReader* reader = formatManager.createReaderFor(file);
    //currentAudioFileSource = new AudioFormatReaderSource (reader, true);
    
    // ..and plug it into our transport source
	/*
    transportSource.setSource (currentAudioFileSource,
                               32768,                   // tells it to buffer this many samples ahead
                               //&thread,                 // this is the background thread to use for reading-ahead
							   &meterThread,
                               reader->sampleRate);     // allows for sample rate correction
	transportSource.setPosition (0);
    transportSource.start();
							   */
    //deviceManager.enableInputLevelMeasurement(true);
	//audioFilePlayer.startFromZero();
	//audioFilePlayer.start();
	
    
    
    startTimer (100);

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
	if(audioFilePlayer.isPlaying()) {
		g.drawEllipse(getWidth()/4, getHeight()/4, pitch /20, pitch /20, 5);
		g.drawText ( drow::Pitch::fromFrequency (pitch).getMidiNoteName(), getLocalBounds(),
                Justification::centred, true);  
	}
    
	
}

void VisualMaker::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void VisualMaker::audioDeviceIOCallback(const float** inputChannelData,
                                int numInputChannels,
                                float** outputChannelData,
                                int numOutputChannels,
                                int numSamples) {
	audioSourcePlayer.audioDeviceIOCallback (inputChannelData,
                                             numInputChannels,
                                             outputChannelData,
                                             numOutputChannels,
                                             numSamples);
	if(outputChannelData[0] != NULL) {
		const ScopedLock sl (detectorLock);
		pitchFinder.processSamples (inputChannelData[0], numSamples);
		pitch = pitchFinder.getPitch();
		meterL.copySamples (outputChannelData[0], numSamples);
		meterR.copySamples (outputChannelData[0], numSamples);
	}
}

void VisualMaker::audioDeviceAboutToStart (AudioIODevice* device)
{
    audioSourcePlayer.audioDeviceAboutToStart (device);
	sampleRate = device->getCurrentSampleRate();
	pitchFinder.setSampleRate (sampleRate);
}

void VisualMaker::audioDeviceStopped()
{
	audioSourcePlayer.audioDeviceStopped();
}



//Buffer 

BufferTransformAudioSource::BufferTransformAudioSource (AudioSource* source_,
                                                        bool deleteSourceWhenDeleted)
    : source (source_, deleteSourceWhenDeleted),
      buffer (512)
{
    jassert (source_ != nullptr);
    
    const float xScale = 1.0f / (buffer.getSize() - 1);
    
    for (int i = 0; i < buffer.getSize(); ++i)
    {
        buffer.getReference (i) = i * xScale;
    }
}

BufferTransformAudioSource::~BufferTransformAudioSource()
{
}

void BufferTransformAudioSource::setBypass (bool shouldBypass)
{
    isBypassed = shouldBypass;
}

void BufferTransformAudioSource::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    source->prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void BufferTransformAudioSource::releaseResources()
{
    source->releaseResources();
}

void BufferTransformAudioSource::getNextAudioBlock (const AudioSourceChannelInfo& info)
{
    source->getNextAudioBlock (info);
    
    if (! isBypassed)
    {
        const int bufferSize = buffer.getSize() - 1;
        float** channelData = info.buffer->getArrayOfWritePointers();
        float sample;

        for (int c = 0; c < info.buffer->getNumChannels(); ++c)
        {
            for (int s = 0; s < info.numSamples; ++s)
            {
                sample = channelData[c][s];
                
                if (sample < 0.0f && sample >= -1.0f)
                {
                    sample *= -1.0f;
                    sample = drow::linearInterpolate (buffer.getData(), bufferSize, sample * bufferSize);
                    sample *= -1.0f;
                }
                else if (sample >= 0.0f && sample <= 1.0f)
                {
                    sample = drow::linearInterpolate (buffer.getData(), bufferSize, sample * bufferSize);
                }
                
                channelData[c][s] = sample;
            }
        }
    }
}