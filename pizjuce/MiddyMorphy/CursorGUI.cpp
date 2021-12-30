
#include "CursorGUI.h"
#include "Cursor.h"

//==============================================================================

void CursorGUI::paint(Graphics & g) {
  // Bouml preserved body begin 0003550D
	g.setColour(Colours::blue);
	g.drawLine((float)getWidth()*0.5f,0,(float)getWidth()*0.5f,(float)getHeight(),2);
	g.drawLine(0,(float)getHeight()*0.5f,(float)getWidth(),(float)getHeight()*0.5f,2);
	g.drawEllipse(6,6,(float)(getWidth()-12),(float)(getHeight()-12),2);
	g.setColour(Colours::orange);
	g.drawEllipse(5,5,(float)(getWidth()-10),(float)(getHeight()-10),4);
  // Bouml preserved body end 0003550D
}

CursorGUI::CursorGUI(Cursor * cursor)
:ModuleGUI(cursor)
{
  // Bouml preserved body begin 0003E20D
	this->setAlwaysOnTop(true);
	this->cursor = cursor;
	cursor->addChangeListener(this);
    this->setMouseClickGrabsKeyboardFocus(false);
  // Bouml preserved body end 0003E20D
}

void CursorGUI::mouseDown(const MouseEvent & e) {
  // Bouml preserved body begin 0003F10D
    this->setInterceptsMouseClicks(true,false);
  if (e.mods.isLeftButtonDown() && !e.mods.isAltDown() && !e.mods.isCtrlDown()) {
	startDrag(e);
    }
  // Bouml preserved body end 0003F10D
}

void CursorGUI::mouseDrag(const MouseEvent & e) {
  // Bouml preserved body begin 0003F18D
    this->setInterceptsMouseClicks(true,false);
  if (e.mods.isLeftButtonDown() && !e.mods.isAltDown() && !e.mods.isCtrlDown()) {
	this->drag(e);
}
	//this->refreshOriginalBounds();
  // Bouml preserved body end 0003F18D
}

void CursorGUI::mouseUp(const MouseEvent & e) {
  // Bouml preserved body begin 0003F20D
  if (e.mods.isRightButtonDown()) {
    this->setInterceptsMouseClicks(false,true);
    }
    else this->setInterceptsMouseClicks(true,false);
  // Bouml preserved body end 0003F20D
}

void CursorGUI::changeListenerCallback(ChangeBroadcaster* source) {
  // Bouml preserved body begin 00047B0D
	if(source == cursor)
	{
		rePosition();
	}
  // Bouml preserved body end 00047B0D
}

CursorGUI::~CursorGUI() {
  // Bouml preserved body begin 00047B8D
	cursor->removeChangeListener(this);
  // Bouml preserved body end 00047B8D
}
