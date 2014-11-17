/*
  ==============================================================================

    VisualMaker.h
    Created: 16 Nov 2014 8:21:51pm
    Author:  Jeffrey Robbins

  ==============================================================================
*/

#ifndef VISUALMAKER_H_INCLUDED
#define VISUALMAKER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class VisualMaker    : public Component, public ChangeListener, public ChangeBroadcaster, private Timer
{
public:
    VisualMaker(File);
    ~VisualMaker();

    void paint (Graphics&);
    void resized();
    void changeListenerCallback (ChangeBroadcaster*) override
    {
        // this method is called by the thumbnail when it has changed, so we should repaint it..
        repaint();
    }

private:
    AudioDeviceManager& deviceManager;
    AudioFormatManager formatManager;
    TimeSliceThread thread;
    ScopedPointer<AudioFormatReaderSource> currentAudioFileSource;
    
    
    AudioSourcePlayer audioSourcePlayer;
    AudioTransportSource transportSource;
    drow::PitchDetector pitchFinder;
    void timerCallback() override
    {
        repaint();
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VisualMaker)
};


#endif  // VISUALMAKER_H_INCLUDED
