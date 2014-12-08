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
	newButton->setBounds (10, 10, 100, 20);
	newButton->addListener (this);

	newButton = new TextButton();
	windowButtons.add(newButton);
	addAndMakeVisible (newButton);
	newButton->setButtonText ("Start");
	newButton->setBounds (10, 120, 50, 20);
	newButton->addListener (this);

	fileBox = new TextEditor();
	addAndMakeVisible(fileBox);
	fileBox->setBounds (10, 80, 400, 20);
	fileBox->setReadOnly(true);

}

//Destructor
MainContentComponent::~MainContentComponent()
{
    for (int i = windowButtons.size(); --i >= 0;) {
        windowButtons.getUnchecked(i) -> removeListener(this);
    }
    windowButtons.clear(true);
    fileBox -> clear();
    delete fileBox;
    //vWindow->
    if (vWindow != nullptr && vWindow != 0) {
        //if (NULL != *vWindow) {
            delete vWindow;
        //}
    }
}

//This is where everything is drawn
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

//Listener
void MainContentComponent :: buttonClicked (Button* button) 
{
	if(button->getButtonText() == "Start") {
        if (validFile()) {
            vWindow = new BasicWindow("visualizer", audioFile);
        }
        else {
            AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
                                              "File Error",
            "Your file hasn't been chosen or isn't a usable format");
        }
	}
	else {
		FileChooser fc ("Choose a file to open...",
                            File::getCurrentWorkingDirectory(),
                            "*",
                            false);
		if (fc.browseForFileToOpen())
		{
			audioFile = fc.getResult();
            filePath.clear();
			filePath << fc.getResult().getFullPathName();
			AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
												"File Chooser...",
												"You picked: " + filePath);
		}
	}
    
}

//Checks if audio file is valid
bool MainContentComponent::validFile()
{
    if (audioFile.existsAsFile()) {
        return audioFile.getFileName().toLowerCase().contains("mp3") ||
        audioFile.getFileName().toLowerCase().contains(".wav") ||
        audioFile.getFileName().toLowerCase().contains(".aiff") ||
        audioFile.getFileName().toLowerCase().contains(".flac") ||
        audioFile.getFileName().toLowerCase().contains(".ogg-vorbis");
    }
    return false;
}

void MainContentComponent::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}


