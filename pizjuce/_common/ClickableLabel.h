#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include "JuceHeader.h"

class ClickableLabel;
class ClickableLabelListener
{
public:
	virtual ~ClickableLabelListener(){};
	virtual void clickableLabelMouseDown(ClickableLabel *label, const MouseEvent &e)=0;
	virtual void clickableLabelMouseDoubleClick(ClickableLabel *label, const MouseEvent &e)=0;
};

class ClickableLabel : public Label
{
public:
	ClickableLabel(const String& _name, const String& _text)
		: Label(_name,_text)
	{
		id=0;
		listener=0;
		setRepaintsOnMouseActivity(true);
	}

	void setListener(ClickableLabelListener *listener_)
	{
		listener=listener_;
	}

	void setId(int id_)
	{
		id=id_;
	}

	int getId()
	{
		return id;
	}

	// pop ups a modal text editor
	void edit()
	{
		TextEditor *ed=new TextEditor();
		if (ed==0) return;
		ed->setFont (getFont());
		const int cols[] = { TextEditor::backgroundColourId,
							 TextEditor::textColourId,
							 TextEditor::highlightColourId,
							 TextEditor::highlightedTextColourId,
							 TextEditor::outlineColourId,
							 TextEditor::focusedOutlineColourId,
							 TextEditor::shadowColourId };

		for (int i = 0; i < numElementsInArray (cols); ++i)
			ed->setColour (cols[i], findColour (cols[i]));
		ed->setBounds(0, 0, getWidth(), getHeight());
		ed->setText(getText(),false);
        ed->setHighlightedRegion (Range<int> (0, getText().length()));
		addAndMakeVisible(ed);
		ed->addListener(this);
		ed->runModalLoop();
		setText(ed->getText(),true);
		deleteAndZero(ed);
	}

private:   
	int id;
	ClickableLabelListener *listener;

	void mouseDown(const MouseEvent &e)
	{
		if (listener!=0)
			listener->clickableLabelMouseDown(this, e);
	}

	void mouseDoubleClick(const MouseEvent &e)
	{
		if (listener!=0)
			listener->clickableLabelMouseDoubleClick(this, e);
	}

	void  textEditorTextChanged (TextEditor &editor)
	{

	}

	void  textEditorReturnKeyPressed (TextEditor &editor)
	{
		editor.exitModalState(0);
	}

	void  textEditorEscapeKeyPressed (TextEditor &editor)
	{
		editor.setText(getText());
		editor.exitModalState(0);
	}

	void  textEditorFocusLost (TextEditor &editor)
	{
		editor.exitModalState(0);
	}
};

#endif
