#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <juce_gui_basics/juce_gui_basics.h>

class ClickableLabel;

class ClickableLabelListener
{
public:
    virtual ~ClickableLabelListener(){};
    virtual void clickableLabelMouseDown(ClickableLabel* label, const juce::MouseEvent& e)        = 0;
    virtual void clickableLabelMouseDoubleClick(ClickableLabel* label, const juce::MouseEvent& e) = 0;
};

class ClickableLabel : public juce::Label
{
public:
    ClickableLabel(const juce::String& _name, const juce::String& _text)
        : Label(_name, _text)
    {
        id       = 0;
        listener = nullptr;
        setRepaintsOnMouseActivity(true);
    }

    void setListener(ClickableLabelListener* listener_)
    {
        listener = listener_;
    }

    void setId(int id_)
    {
        id = id_;
    }

    int getId()
    {
        return id;
    }

    // pop ups a modal text editor
    void edit()
    {
        juce::TextEditor* ed = new juce::TextEditor();
        if (ed == nullptr)
        {
            return;
        }
        ed->setFont(getFont());
        const int cols[] = { juce::TextEditor::backgroundColourId,
                             juce::TextEditor::textColourId,
                             juce::TextEditor::highlightColourId,
                             juce::TextEditor::highlightedTextColourId,
                             juce::TextEditor::outlineColourId,
                             juce::TextEditor::focusedOutlineColourId,
                             juce::TextEditor::shadowColourId };

        for (int i = 0; i < juce::numElementsInArray(cols); ++i)
        {
            ed->setColour(cols[i], findColour(cols[i]));
        }
        ed->setBounds(0, 0, getWidth(), getHeight());
        ed->setText(getText(), false);
        ed->setHighlightedRegion(juce::Range<int>(0, getText().length()));
        addAndMakeVisible(ed);
        ed->addListener(this);
        ed->runModalLoop();
        setText(ed->getText(), juce::dontSendNotification);
        deleteAndZero(ed);
    }

private:
    int id;
    ClickableLabelListener* listener;

    void mouseDown(const juce::MouseEvent& e) override
    {
        if (listener != nullptr)
        {
            listener->clickableLabelMouseDown(this, e);
        }
    }

    void mouseDoubleClick(const juce::MouseEvent& e) override
    {
        if (listener != nullptr)
        {
            listener->clickableLabelMouseDoubleClick(this, e);
        }
    }

    void textEditorTextChanged(juce::TextEditor& editor) override
    {
    }

    void textEditorReturnKeyPressed(juce::TextEditor& editor) override
    {
        editor.exitModalState(0);
    }

    void textEditorEscapeKeyPressed(juce::TextEditor& editor) override
    {
        editor.setText(getText());
        editor.exitModalState(0);
    }

    void textEditorFocusLost(juce::TextEditor& editor) override
    {
        editor.exitModalState(0);
    }
};

#endif
