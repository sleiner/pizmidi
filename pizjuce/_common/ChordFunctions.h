#ifndef PIZ_CHORD_FUNCTIONS_H
#define PIZ_CHORD_FUNCTIONS_H

// Chord regognition functions are adapted from PyChoReLib: http://chordrecognizer.sourceforge.net/
// (GPL chord recognizer written in Python)

#include "../_common/midistuff.h"
#include "juce_core/juce_core.h"
#include <algorithm>

Array<int> getAsStackedChord (Array<int>& chord, bool reduce = true);

class PizChord
{
public:
    PizChord() {}
    PizChord (Array<int> newChord);
    ~PizChord() {}

    void setChord (Array<int> newChord);
    int getSum() const;
    Array<int> getChord() const { return chord; }
    Array<int> getPattern() const { return pattern; }
    String getStringPattern() const;

    bool operator< (const PizChord& other)
    {
        return this->getSum() < other.getSum();
    }

    bool operator> (const PizChord& other)
    {
        return this->getSum() > other.getSum();
    }

    bool operator== (const PizChord& other)
    {
        return this->getSum() == other.getSum();
    }

    bool operator!= (const PizChord& other)
    {
        return this->getSum() != other.getSum();
    }

private:
    Array<int> chord;
    Array<int> pattern;
    void makeIntervalPattern();
};

bool operator< (const PizChord& first, const PizChord& second);

class ChordName
{
public:
    ChordName (String chordName, String noteString);
    ~ChordName() {}
    String getName (int rootNote, int bassNote, bool flats);
    int getRootIndex() { return rootIndex; }

    bool equals (String& noteString);
    bool equals (Array<int>& chord);
    bool equals2 (String& intervalString);
    String getIntervalString (String noteString);
    static String getIntervalString (Array<int> chord);

private:
    String name;
    String pattern;
    int root;
    int rootIndex;
};

static Array<ChordName> ChordNames;
void fillChordDatabase();

String listNoteNames (Array<int> chord);
String getFirstRecognizedChord (Array<int> chord, bool flats);
String getIntervalStringFromNoteNames (int root, String noteString, int bottomOctave);

#endif
