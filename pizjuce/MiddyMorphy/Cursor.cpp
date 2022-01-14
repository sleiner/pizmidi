
#include "Cursor.h"
#include "MidiMorph.h"

Cursor::Cursor (MidiMorph* core)
{
    // Bouml preserved body begin 0003110D
    this->core = core;
    this->size = 50;
    // Bouml preserved body end 0003110D
}

//gets called when the XYItem gets new coordinates
void Cursor::moved()
{
    // Bouml preserved body begin 0003588D
    core->cursorChanged();
    sendChangeMessage();
    // Bouml preserved body end 0003588D
}

juce::Point<float> Cursor::getCursorPosition()
{
    // Bouml preserved body begin 0003C40D
    return Point<float> (Point::getX(), Point::getY());
    // Bouml preserved body end 0003C40D
}
