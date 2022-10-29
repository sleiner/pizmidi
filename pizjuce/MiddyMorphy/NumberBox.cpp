
#include "NumberBox.h"

bool NumberBox::keyPressed (const juce::KeyPress& key)
{
    if (juce::String ("0123456789.,-+").indexOfChar (key.getTextCharacter()) > -1 || key == juce::KeyPress::returnKey || key.isKeyCode (juce::KeyPress::escapeKey))
    {
        TextEditor::keyPressed (key);
    }
    return false;
}
