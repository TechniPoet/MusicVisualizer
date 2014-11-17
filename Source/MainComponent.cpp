/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

OwnedArray<TextButton> windowButtons;
TextEditor* fileBox;
String filePath;
File audioFile;
//==============================================================================
MainContentComponent::MainContentComponent()
{
    setSize (500, 400);
	TextButton* newButton = new TextButton();
	windowButtons.add(newButton);
	addAndMakeVisible (newButton);
	newButton->setButtonText ("Browse Files");
	newButton->setBounds (10, 10, 100, 20);	newButton->addListener (this);	newButton = new TextButton();
	windowButtons.add(newButton);
	addAndMakeVisible (newButton);
	newButton->setButtonText ("Start");
	newButton->setBounds (10, 120, 50, 20);	newButton->addListener (this);	fileBox = new TextEditor();	addAndMakeVisible(fileBox);	fileBox->setBounds (10, 80, 400, 20);	fileBox->setReadOnly(true);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colours::black);

    g.setFont (Font (16.0f));
    g.setColour (Colours::black);
	fileBox-> setText(filePath);
	//fileBox->setBoundsToFit(10, 80, 1000, 20, Justification::left, false);
	
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

void MainContentComponent :: buttonClicked (Button* button) 
{
	if(button->getButtonText() == "Start") {
		AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
                                            "Heya there",
                                            "You hit start! Good for you!");
	}
	else {
		FileChooser fc ("Choose a file to open...",
                            File::getCurrentWorkingDirectory(),
                            "*",
                            false);

		if (fc.browseForFileToOpen())
		{
			audioFile = fc.getResult();
			filePath << fc.getResult().getFullPathName();
			AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
												"File Chooser...",
												"You picked: " + filePath);
		}
	}
    
}



void MainContentComponent::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}


