
#include "D3CKLook.h"

int D3CKLook::getDefaultScrollbarWidth() {
  // Bouml preserved body begin 00049D0D
	return 12;
  // Bouml preserved body end 00049D0D
}

//    int  thumbSize,      bool  isMouseOver,      bool  isMouseDown    )   
void D3CKLook::drawScrollbar(Graphics & g, ScrollBar & scrollbar, int x, int y, int width, int height, bool isScrollbarVertical, int thumbStartPosition, int thumbSize, bool isMouseOver, bool isMouseDown) {
  // Bouml preserved body begin 00049F0D
	if(thumbSize > 0)
	{
		if(isScrollbarVertical)
		{
				if(isMouseOver)
				{
					g.fillRoundedRectangle((float)(x+1),(float)(thumbStartPosition+1),
										   (float)(width-2),(float)(thumbSize-2),(float)jmin(width,height)*0.25f);

				}
				else
				{
					g.drawRoundedRectangle((float)(x+1),(float)(thumbStartPosition+1),
										   (float)(width-2),(float)(thumbSize-2),(float)jmin(width,height)*0.25f,1.f);
				}
		}
		else
		{
				if(isMouseOver)
				{
					g.fillRoundedRectangle((float)(thumbStartPosition+1),(float)(y+1),(float)(thumbSize-2),(float)(height-2),(float)jmin(width,height)*0.25f);
				}
				else
				{
					g.drawRoundedRectangle((float)(thumbStartPosition+1),(float)(y+1),(float)(thumbSize-2),(float)(height-2),(float)jmin(width,height)*0.25f,1.f);
				}
		}
	}

  // Bouml preserved body end 00049F0D
}

void D3CKLook::drawScrollbarButton(Graphics & g, ScrollBar & scrollbar, int width, int height, int buttonDirection, bool isScrollbarVertical, bool isMouseOver, bool isButtonDown) {
  // Bouml preserved body begin 0004A00D
	if(isMouseOver)
	{
		g.fillRoundedRectangle(1,1,(float)(width-2),(float)(height-2),(float)jmin(width,height)*0.25f);
	}

	g.drawRoundedRectangle(1,1,(float)(width-2),(float)(height-2),(float)jmin(width,height)*0.25f,1);
  // Bouml preserved body end 0004A00D
}

//  (   &     ) 
int D3CKLook::getScrollbarButtonSize(ScrollBar & scrollbar) {
  // Bouml preserved body begin 0004A18D
	return 8;
  // Bouml preserved body end 0004A18D
}

//getMinimumScrollbarThumbSize  (   &     ) 
int D3CKLook::getMinimumScrollbarThumbSize(ScrollBar & scrollbar) {
  // Bouml preserved body begin 0004A20D
	return 8;
  // Bouml preserved body end 0004A20D
}

//  (   &  g,      int  width,      int  height,      TextEditor &  textEditor    )   
void D3CKLook::drawTextEditorOutline(Graphics & g, int width, int height, juce::TextEditor & textEditor) {
  // Bouml preserved body begin 0004A30D
	g.drawRoundedRectangle(1,1,(float)(width-2),(float)(height-2),(float)jmin(width,height)*0.5f,1);
  // Bouml preserved body end 0004A30D
}

void D3CKLook::drawPopupMenuItem(Graphics & g, int width, int height, const bool isSeperator, const bool isActive, const bool isHightlited, const bool isTicked, const bool hasSubmenu, const String & text, const String & shortcutText, Image * image, const juce::Colour *const textColour) {
  // Bouml preserved body begin 0004A78D
	g.fillRect(0,0,width,height);
	if( ! isSeperator)
	{

	if(isHightlited)
	{
		g.setColour(Colours::lightgrey);

	}
	else
	{
		g.setColour(Colours::white);
	}
	g.fillRoundedRectangle(0,0,(float)width,(float)height,(float)height*0.5f);
	g.setColour(Colours::green);
	g.drawRoundedRectangle(2,2,(float)(width-4),(float)(height-4),(float)(height-2)*0.5f,1);
	g.drawEllipse(2,2,(float)(height-4),(float)(height-4),1);
	if(isTicked)
	{
		g.fillEllipse(4,4,(float)(height-8),(float)(height-8));
	}
	g.drawText(text,height+10,0,width,height,Justification::left,true);
	}
	if(hasSubmenu)
	{
		g.fillEllipse((float)(width+5),5,(float)(height-10),(float)(height-10));
	}
  // Bouml preserved body end 0004A78D
}

void D3CKLook::drawPopupMenuBackground(Graphics & g, int width, int height) {
  // Bouml preserved body begin 0004A80D
	//g.fillAll(Colours::white);
  // Bouml preserved body end 0004A80D
}

DropShadower* D3CKLook::createDropShadowerForComponent(juce::Component * comp) {
  // Bouml preserved body begin 0004A88D
	return 0;
  // Bouml preserved body end 0004A88D
}

