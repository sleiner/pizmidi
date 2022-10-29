
#include "Cursor.h"
#include "MidiMorph.h"

Cursor::Cursor (MidiMorph* core)
{
    this->core = core;
    this->size = 50;
}

void Cursor::moved()
{
    core->cursorChanged();
    sendChangeMessage();
}

juce::Point<float> Cursor::getCursorPosition()
{
    return { this->getX(), this->getY() };
}
