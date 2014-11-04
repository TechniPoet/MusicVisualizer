/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

OwnedArray<TextButton> windowButtons;
//==============================================================================
MainContentComponent::MainContentComponent()
{
    setSize (500, 400);
	TextButton* newButton = new TextButton();
	windowButtons.add(newButton);
	addAndMakeVisible (newButton);
	newButton->setButtonText ("test");
	newButton->setBounds (10, 10, 50, 20);	newButton->addListener (this);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colours::black);

    g.setFont (Font (16.0f));
    g.setColour (Colours::black);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

void MainContentComponent :: buttonClicked (Button* button) 
{
        FileChooser fc ("Choose a file to open...",
                                File::getCurrentWorkingDirectory(),
                                "*",
                                true);
		if (fc.browseForFileToOpen())
                {
                    String chosen;
                    for (int i = 0; i < fc.getResults().size(); ++i)
                        chosen << fc.getResults().getReference(i).getFullPathName() << "\n";

                    AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
                                                      "File Chooser...",
                                                      "You picked: " + chosen);
                }
}

