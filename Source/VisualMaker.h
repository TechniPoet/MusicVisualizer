/*
  ==============================================================================

    VisualMaker.h
    Created: 16 Nov 2014 8:21:51pm
    Author:  Jeffrey Robbins

  ==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"
#include "Shapes.cpp"


//==============================================================================
class BufferTransformAudioSource :  public AudioSource
{
public:
    //==============================================================================
    BufferTransformAudioSource (AudioSource* source,
                                bool deleteSourceWhenDeleted = false);
    
    /** Destructor. */
    ~BufferTransformAudioSource();
    
    /** Setting this to true does not apply the buffer. */
    void setBypass (bool shouldBypass);
    
    /** Returns all of the settings.
     */
    drow::Buffer& getBuffer()     {   return buffer;    }
        
    //==============================================================================
    /** @internal. */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
    
    /** @internal. */
    void releaseResources();
    
    /** @internal. */
    void getNextAudioBlock (const AudioSourceChannelInfo& info);
        
private:
    //==============================================================================
    OptionalScopedPointer<AudioSource> source;
    drow::Buffer buffer;
    bool isBypassed;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BufferTransformAudioSource);
};

#ifndef VISUALMAKER_H_INCLUDED
#define VISUALMAKER_H_INCLUDED

//==============================================================================
/*
*/
class VisualMaker    : public Component,  
					   private Timer, 
					   public AudioIODeviceCallback
{
public:
    VisualMaker(File);
    ~VisualMaker();

    void paint (Graphics&);
    void resized();
	
	void audioDeviceIOCallback (const float**,int,
                                float**,int,int);

	void audioDeviceAboutToStart (AudioIODevice* device);
    void audioDeviceStopped();
    void drawCircle(Graphics&);
	//void processBlock(const float *channelData, int numSamples);
								

private:
    AudioDeviceManager& deviceManager;
    AudioFormatManager formatManager;
	drow::AudioFilePlayer audioFilePlayer;
	//TimeSliceThread thread;
    TimeSliceThread meterThread;
	drow::SegmentedMeter meterL, meterR;
    ScopedPointer<AudioFormatReaderSource> currentAudioFileSource;
    
	BufferTransformAudioSource bufferTransformAudioSource;
	//OptionalScopedPointer<AudioSource> source;
    //drow::Buffer buffer;
    
    AudioSourcePlayer audioSourcePlayer;
    //AudioTransportSource transportSource;
	//pitch
    double lastPitch;
	double pitch;
	double sampleRate;
    double height;
    double width;
    float red{50}, blue{50}, green{50};
    drow::PitchDetector pitchFinder;
	CriticalSection detectorLock;
    Shapes shape;
    
    
    void timerCallback() override
    {
        repaint();
    }



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VisualMaker)
};
#endif  

