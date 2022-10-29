#ifndef PIZ_CHORD_FUNCTIONS_H
#define PIZ_CHORD_FUNCTIONS_H

// Chord regognition functions are adapted from PyChoReLib: http://chordrecognizer.sourceforge.net/
// (GPL chord recognizer written in Python)

#include <algorithm>

#include <juce_core/juce_core.h>

#include "../_common/midistuff.h"

juce::Array<int> getAsStackedChord (juce::Array<int>& chord, bool reduce = true);

class PizChord
{
public:
    PizChord()
    {
    }
    PizChord (juce::Array<int> newChord);
    ~PizChord()
    {
    }

    void setChord (juce::Array<int> newChord);
    int getSum() const;
    juce::Array<int> getChord() const
    {
        return chord;
    }
    juce::Array<int> getPattern() const
    {
        return pattern;
    }
    juce::String getStringPattern() const;

    bool operator<(const PizChord& other)
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
    juce::Array<int> chord;
    juce::Array<int> pattern;
    void makeIntervalPattern();
};

bool operator<(const PizChord& first, const PizChord& second);

class ChordName
{
public:
    ChordName (juce::String chordName, juce::String noteString);
    ~ChordName()
    {
    }
    juce::String getName (int rootNote, int bassNote, bool flats);
    int getRootIndex()
    {
        return rootIndex;
    }

    bool equals (juce::String& noteString);
    bool equals (juce::Array<int>& chord);
    bool equals2 (juce::String& intervalString);
    juce::String getIntervalString (juce::String noteString);
    static juce::String getIntervalString (juce::Array<int> chord);

private:
    juce::String name;
    juce::String pattern;
    int root;
    int rootIndex;
};

static juce::Array<ChordName> ChordNames;
void fillChordDatabase();

juce::String listNoteNames (juce::Array<int> chord);
juce::String getFirstRecognizedChord (juce::Array<int> chord, bool flats);
juce::String getIntervalStringFromNoteNames (int root, juce::String noteString, int bottomOctave);

#endif
