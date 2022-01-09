#ifndef GUITAR_NECK_COMPONENT_H
#define GUITAR_NECK_COMPONENT_H

#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "../_common/midistuff.h"

#define maxFrets (32)
#define maxStrings (16)
#define nutWidth (7)

class FrettedNote {
public:
	FrettedNote() {fret=-1; string=-1;}
	FrettedNote(int f, int s) {fret=f; string=s;}
	~FrettedNote() {}

	bool isValid() const {return fret>=0 && string>=0;}
	void invalidate() {fret = -1; string = -1;}

	bool operator != (FrettedNote const &other) {return other.fret!=this->fret || other.string!=this->string;}
	bool operator == (FrettedNote const &other) {return other.fret==this->fret && other.string==this->string;}

	int fret;
	int string;
};

class GuitarNeckComponent : public Component,
							public MidiKeyboardStateListener,
							public ChangeBroadcaster,
                            private Timer,
                            private AsyncUpdater
{
public:
    GuitarNeckComponent (MidiKeyboardState& state);
    ~GuitarNeckComponent() override;

	void setNumStrings(int n);
	void setNumFrets(int n);
	void setDotSize(float diameter) {
		dotSize = diameter;
		repaint();
	}
	void setStringNote(int string, int note) {
		stringNote[string]=note;
		repaint();
	}

	void setFlats(bool flats) {showFlats=flats;}
    void setVelocity (float velocity);
    void setMidiChannel (int midiChannelNumber);
    int getMidiChannel() const noexcept                             { return midiChannel; }
    void setMidiChannelsToDisplay (int midiChannelMask);
    int getMidiChannelsToDisplay() const noexcept                   { return midiInChannelMask; }

    void setAvailableRange (int lowestNote,
                            int highestNote);
    int getRangeStart() const noexcept                              { return rangeStart; }
    int getRangeEnd() const noexcept                                { return rangeEnd; }

    void setLowestVisibleFret (int fretNumber);
    int getLowestVisibleFret() const noexcept                        { return firstFret; }

	void drawNote (int fret, int string,
                                           Graphics& g, int x, int y, int w, int h,
                                           bool isDown, bool isOver,
                                           const Colour& lineColour,
                                           const Colour& textColour);

	const String getNoteText(const int fret, const int string);
    //==============================================================================
    /** A set of colour IDs to use to change the colour of various aspects of the keyboard.

        These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
        methods.

        @see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
    */
    enum ColourIds
    {
        whiteNoteColourId               = 0x1005000,
        blackNoteColourId               = 0x1005001,
        keySeparatorLineColourId        = 0x1005002,
        mouseOverKeyOverlayColourId     = 0x1005003,  /**< This colour will be overlaid on the normal note colour. */
        keyDownOverlayColourId          = 0x1005004,  /**< This colour will be overlaid on the normal note colour. */
        textLabelColourId               = 0x1005005,
        upDownButtonBackgroundColourId  = 0x1005006,
        upDownButtonArrowColourId       = 0x1005007
    };

    /** Returns the position within the component of the left-hand edge of a key.

        Depending on the keyboard's orientation, this may be a horizontal or vertical
        distance, in either direction.
    */
    int getKeyStartPosition (const int midiNoteNumber) const;

    /** This sets the octave number which is shown as the octave number for middle C.

        This affects only the default implementation of getWhiteNoteText(), which
        passes this octave number to MidiMessage::getMidiNoteName() in order to
        get the note text. See MidiMessage::getMidiNoteName() for more info about
        the parameter.

        By default this value is set to 3.

        @see getOctaveForMiddleC
    */
    void setOctaveForMiddleC (int octaveNumForMiddleC);

    /** This returns the value set by setOctaveForMiddleC().
        @see setOctaveForMiddleC
    */
    int getOctaveForMiddleC() const noexcept            { return octaveNumForMiddleC; }

	int getNote(FrettedNote n) { return stringNote[n.string] + n.fret; }
	int getStringFret(int string);

    //==============================================================================
    /** @internal */
    void paint (Graphics& g) override;
    /** @internal */
    void resized() override;
    /** @internal */
    void mouseMove (const MouseEvent& e) override;
    /** @internal */
    void mouseDrag (const MouseEvent& e) override;
    /** @internal */
    void mouseDown (const MouseEvent& e) override;
    /** @internal */
    void mouseUp (const MouseEvent& e) override;
    /** @internal */
    void mouseEnter (const MouseEvent& e) override;
    /** @internal */
    void mouseExit (const MouseEvent& e) override;
    /** @internal */
    void timerCallback() override;
    /** @internal */
    void focusLost (FocusChangeType cause) override;
    /** @internal */
    void handleNoteOn (MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    /** @internal */
    void handleNoteOff (MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    /** @internal */
    void handleAsyncUpdate() override;
    /** @internal */
    void colourChanged() override;

protected:
    //==============================================================================
    virtual void drawFretString (int fret, int string,
                                Graphics& g,
                                int x, int y, int w, int h,
                                bool isDown, bool isOver,
                                const Colour& lineColour,
                                const Colour& textColour);

    virtual bool mouseDownOnKey (int fret, int string, const MouseEvent& e);
    virtual void mouseDraggedToKey (int fret, int string, const MouseEvent& e);

private:
    //==============================================================================
    MidiKeyboardState& state;
    int xOffset;

    int midiChannel, midiInChannelMask;
    float velocity;
    BigInteger keysPressed, keysCurrentlyDrawnDown;

	int numStrings, numFrets;
	int stringNote[maxStrings];
	int currentlyFrettedFret[maxStrings];
	void getFretPos (int fret, int& x, int& w) const;
	int getFretPos (int fret) const;
	int getFretWidth (int fret) const;
	float dotSize;

    int rangeStart, rangeEnd, firstFret;
    bool mouseDragging;

    int octaveNumForMiddleC;

    FrettedNote noteUnderMouse, mouseDownNote;
    FrettedNote xyToNote (const Point<int>& pos, float& mousePositionVelocity);
    void resetAnyKeysInUse();
    void updateNoteUnderMouse (const Point<int>& pos);
    void repaintNote (const int fret);
	bool showFlats;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GuitarNeckComponent);
};


#endif
