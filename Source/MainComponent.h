/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "VisualMaker.h"

class BasicWindow   : public DocumentWindow
{
public:
    BasicWindow (const String& name, File& audioFile)
        : DocumentWindow (name, Colours::lightgrey, DocumentWindow::allButtons)
    {
		setContentOwned(new VisualMaker(audioFile), true);
        setTitleBarButtonsRequired(0, true);
		//centreWithSize (getWidth(), getHeight());
        centreWithSize(400, 400);
        setVisible (true);
    }

    void closeButtonPressed()
    {
        delete this;
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicWindow)
};
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component,
							   private Button::Listener 
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&);
    void resized();
	bool validFile();

private:
    //==============================================================================
	void buttonClicked (Button* button);
	// called by the OS when the window's close button is pressed.
	void closeButtonPressed();
	//ScopedPointer<BasicWindow> vWindow;
    BasicWindow* vWindow;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
