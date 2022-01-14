
#include "MidiMorph.h"
#include "ControllerGUI.h"
#include "Controller.h"
#include "ControllerValue.h"

ControllerGUI::ControllerGUI(Controller * controller, MidiMorph * core) {
  // Bouml preserved body begin 00033A8D
	this->controller = controller;
	this->core = core;


	addAndMakeVisible(value = new TextBoxSlider(0));
	value->setRange(0,127,1,2);
	value->addListener(this);

	addAndMakeVisible(channel = new TextBoxSlider(0));
	channel->setRange(1,16,1,2);
	channel->addListener(this);

	addAndMakeVisible(ccNo = new TextBoxSlider(0));
	ccNo->setRange(0,127,1,2);
	ccNo->addListener(this);

	addAndMakeVisible(this->name = new TextEditor());
	name->setSelectAllWhenFocused(true);
	name->setColour(TextEditor::outlineColourId,Colour(0.f,0,0,0.f));
	name->setColour(TextEditor::backgroundColourId,Colour(0.f,0,0,0.f));
	name->setColour(TextEditor::focusedOutlineColourId,Colour(0.f,0,0,0.f));
	name->setColour(TextEditor::highlightColourId,Colours::white);
	name->setFont(Font(10));
	name->setInputRestrictions(32);
	name->addListener(this);

	addAndMakeVisible(labCc = new Label("","cc:"));
	addAndMakeVisible(labCh = new Label("","ch:"));
	labCc->setFont(Font(10));
	labCh->setFont(Font(10));

	setOpaque(true);

	this->setSize(core->controllerListWidth,24);
	this->isSelected=false;

	refreshControllerData();

	core->addChangeListener(this);

	//controller->addChangeListener(this);
  // Bouml preserved body end 00033A8D
}

ControllerGUI::~ControllerGUI() {
  // Bouml preserved body begin 0003EF0D
	core->removeChangeListener(this);
	//controller->removeChangeListener(this);
	deleteAndZero(value);
	deleteAndZero(channel);
	deleteAndZero(ccNo);
	deleteAndZero(name);
	deleteAndZero(labCc);
	deleteAndZero(labCh);
	//deleteAndZero(test);
  // Bouml preserved body end 0003EF0D
}

void ControllerGUI::resized() {
  // Bouml preserved body begin 00033B0D
	int left = roundToInt((float)getWidth()*0.30f);
	int right = getWidth()-left;
	int h2 =  getHeight()/2;
	int rightdelta = roundToInt((float)right*0.25f);
	int right1 = rightdelta+left;
	int right2 = right1+rightdelta;
	int right3 = right2+rightdelta;

	value->setBounds(0 , 0 , left , getHeight());

	name->setBounds(left ,0 ,right , h2);

	labCh->setBounds(left , h2 , rightdelta , h2);
	channel->setBounds(right1 , h2 , rightdelta , h2 );

	labCc->setBounds(right2 , h2 , rightdelta , h2 );
	ccNo->setBounds(right3 , h2 , rightdelta , h2 );
  // Bouml preserved body end 00033B0D
}

//==============================================================================

void ControllerGUI::paint(Graphics & g) {
  // Bouml preserved body begin 00033B8D
	g.fillAll(Colours::lightgrey);
  // Bouml preserved body end 00033B8D
}

//==============================================================================

void ControllerGUI::paintOverChildren(Graphics & g) {
  // Bouml preserved body begin 00033F0D
	g.drawRoundedRectangle(0,0,(float)getWidth(),(float)getHeight(),2,2);
  // Bouml preserved body end 00033F0D
}

void ControllerGUI::mouseUp(const MouseEvent & e) {
  // Bouml preserved body begin 00033F8D
    if(e.mods.isRightButtonDown())
	{
		PopupMenu menu;
		menu.addItem(1,"remove controller");
		menu.addItem(2,"add controller");
		int result = menu.show();
		if(result == 1)
		{
			controller->remove();
		}
		else if(result == 2)
		{
			int ch = core->getController(core->getNumControllers()-1)->getChannel();
			int cc = core->getController(core->getNumControllers()-1)->getCcNo();
			core->addController(cc+1,ch);
		}
	}
  // Bouml preserved body end 00033F8D
}

//()=
void ControllerGUI::sliderValueChanged(Slider* slider) {
  // Bouml preserved body begin 00034C0D
  // Bouml preserved body end 00034C0D
}

void ControllerGUI::setSelected(bool shouldDrawSelected) {
  // Bouml preserved body begin 00033E8D
	this->isSelected = true;
  // Bouml preserved body end 00033E8D
}

void ControllerGUI::changeListenerCallback(ChangeBroadcaster* source) {
  // Bouml preserved body begin 0003EA8D
	//if(objectThatHasChanged == controller)
	//{
	//	refreshControllerData();
	//	this->repaint();
	//}
	//else if(objectThatHasChanged == core->getSelectedScenes())
	//{
	//	refreshControllerData();
	//	this->repaint();
	//}
	//else if(objectThatHasChanged == core->getCursor())
	//{
		refreshControllerData();

	//}
  // Bouml preserved body end 0003EA8D
}

void ControllerGUI::refreshControllerData() {
  // Bouml preserved body begin 0003F38D
	if(core->getNumSelectedScenes() > 0)
	{
		if(controller->getValue(core->getSelectedScene(0)) != 0)
		{
			this->value->setValue(controller->getValue(core->getSelectedScene(0))->getValue(),dontSendNotification);
		}
		else
		{
			this->value->setValue(0,dontSendNotification);
		}
	}
	else
	{
		this->value->setValue(controller->getInterpolatedValue(),dontSendNotification);
	}
	this->channel->setValue(controller->getChannel(),dontSendNotification);
	this->ccNo->setValue(controller->getCcNo(),dontSendNotification);
	this->name->setText(controller->getName(),false);
	int state = controller->getState();
	switch(state)
	{
	case Controller::undefined :
        value->setColour(Label::textColourId,Colours::blue);
		break;
	case Controller::mutival :
		value->setColour(Label::textColourId,Colours::red);
		break;
	case Controller::defined :
		value->setColour(Label::textColourId,Colours::black);
		break;
	}


	this->repaint();
  // Bouml preserved body end 0003F38D
}

void ControllerGUI::textEditorTextChanged(TextEditor& editor) {
  // Bouml preserved body begin 0004178D
	if(&editor == name)
	{
		controller->setName(editor.getText());
	}
  // Bouml preserved body end 0004178D
}

//  (       )
void ControllerGUI::textEditorReturnKeyPressed(TextEditor & editor) {
  // Bouml preserved body begin 0004180D
  // Bouml preserved body end 0004180D
}

void ControllerGUI::textEditorEscapeKeyPressed(TextEditor & editor) {
  // Bouml preserved body begin 0004188D
  // Bouml preserved body end 0004188D
}

void ControllerGUI::textEditorFocusLost(TextEditor & editor) {
  // Bouml preserved body begin 0004190D
  // Bouml preserved body end 0004190D
}

void ControllerGUI::labelTextChanged(Label * labelThatHasChanged) {
  // Bouml preserved body begin 00048A0D
	if(labelThatHasChanged == value)
	{
		controller->setValue(roundToInt (value->getValue()));
		repaint();
	}
	else if(labelThatHasChanged == ccNo)
	{
		controller->ccNo = roundToInt (ccNo->getValue());
	}
	else if(labelThatHasChanged == channel)
	{
		controller->channel = roundToInt (channel->getValue());
	}
	//else if(labelThatHasChanged = test)
	//{

	//}

  // Bouml preserved body end 00048A0D
}
