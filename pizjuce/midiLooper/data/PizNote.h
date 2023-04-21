#ifndef PIZMIDI_PIZNOTE_H
#define PIZMIDI_PIZNOTE_H

#include "piz_MidiMessageSequence.h"

struct PizNote
{
    PizNote()
    {
        object = nullptr;
        length = -1;
    }

    explicit PizNote(const PizMidiMessageSequence::mehPtr& _object)
    {
        object = _object;
        jassert(_object->noteOffObject != nullptr);
        length = _object->noteOffObject != nullptr ? _object->noteOffObject->message.getTimeStamp() - _object->message.getTimeStamp() : 0;
    }

    void updateLength()
    {
        if (object->message.isNoteOn())
        {
            jassert(object->noteOffObject != nullptr);
            length = object->noteOffObject != nullptr ? object->noteOffObject->message.getTimeStamp() - object->message.getTimeStamp() : 0;
        }
    }

    double length;
    PizMidiMessageSequence::mehPtr object;
};

#endif //PIZMIDI_PIZNOTE_H
