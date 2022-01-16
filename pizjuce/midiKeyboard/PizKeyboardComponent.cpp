#include "PizKeyboardComponent.h"
#include "PizKeyboardEditor.h"

PizKeyboardComponent::PizKeyboardComponent (MidiKeyboardState& state, const Orientation orientation)
    : MidiKeyboardComponent (state, orientation),
      drawQwerty (false),
      drawNoteNumber (false)
{
    s = &state;
    for (int i = String (keymap).length(); --i >= 0;)
        setKeyPressForNote (KeyPress (keymap[i], 0, 0), i);
    setKeyPressBaseOctave (4);
}

bool PizKeyboardComponent::keyStateChanged (bool isKeyDown)
{
    if (drawQwerty)
        return true;
    if (toggle)
    {
        bool keyPressUsed = false;
        for (int i = _keyPresses.size(); --i >= 0;)
        {
            const int note = 12 * baseOctave + _keyPressNotes.getUnchecked (i);

            if (_keyPresses.getReference (i).isCurrentlyDown())
            {
                if (! _keysPressed[note])
                {
                    midiKeyboardEditor* editor = ((midiKeyboardEditor*) (this->getParentComponent()));
                    if (s->isNoteOn (getMidiChannel(), note))
                    {
                        s->noteOff (getMidiChannel(), note, 1.f);
                        keyPressUsed = true;
                    }
                    else
                    {
                        _keysPressed.setBit (note);
                        s->noteOn (getMidiChannel(), note, editor->getFilter()->getParameter (kVelocity));
                        keyPressUsed = true;
                    }
                }
            }
            else
            {
                if (_keysPressed[note])
                {
                    _keysPressed.clearBit (note);
                    keyPressUsed = true;
                }
            }
        }
        return keyPressUsed;
    }

    return MidiKeyboardComponent::keyStateChanged (isKeyDown);
}

bool PizKeyboardComponent::mouseDownOnKey (int midiNoteNumber, const MouseEvent& e)
{
    midiKeyboardEditor* editor = ((midiKeyboardEditor*) (this->getParentComponent()));
    if (e.mods.isAltDown())
    {
        editor->getFilter()->setParameter (kHidePanel, 1.f - editor->getFilter()->getParameter (kHidePanel));
        return false;
    }
    int l          = isBlackKey (midiNoteNumber) ? this->getBlackNoteLength() : getHeight();
    float velocity = editor->getFilter()->getParameter (kUseY) >= 0.5f ? (float) e.getMouseDownY() / (float) l : editor->getFilter()->getParameter (kVelocity);
    if (e.mods.isCtrlDown() || e.mods.isMiddleButtonDown())
    {
        s->allNotesOff (this->getMidiChannel());
        _keysPressed.clear();
        return false;
    }
    else if (e.mods.isShiftDown())
    {
        if (! e.mods.isPopupMenu())
            setKeyPressBaseOctave (midiNoteNumber / 12);
        grabKeyboardFocus();
        repaint();
        return false;
    }
    else if (e.mods.isPopupMenu() != toggle)
    {
        if (s->isNoteOn (this->getMidiChannel(), midiNoteNumber))
        {
            s->noteOff (this->getMidiChannel(), midiNoteNumber, 1.f);
        }
        else
        {
            s->noteOn (this->getMidiChannel(), midiNoteNumber, velocity);
        }
        return false;
    }
    else
    {
        if (! s->isNoteOn (this->getMidiChannel(), midiNoteNumber))
        {
            //s->noteOn(this->getMidiChannel(),midiNoteNumber,velocity);
            this->setVelocity (velocity, false);
            return true;
        }
        else
        {
            s->noteOff (this->getMidiChannel(), midiNoteNumber, 1.f);
            return false;
        }
    }
    return true;
}
