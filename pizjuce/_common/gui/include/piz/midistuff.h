#ifndef PIZ_MIDI_STUFF_H
#define PIZ_MIDI_STUFF_H

#include "juce_core/juce_core.h"
using namespace juce;

#define NOT_PLAYING  (-999)
#define ANY_CHANNEL  (0)
#define MIDI_NOTEOFF (0x80)
#define MIDI_NOTEON  (0x90)
#define midiScaler   ((float) 0.007874015748031496062992125984252)
#define NOT_A_NOTE   (98765)

const String Flat ("b"); //(L"\x266d");
const String Natural (L"\x266e");
const String Sharp ("#");      //(L"\x266f");
const String Diminished ("o"); //(L"\x0080");

enum NoteNames
{
    nC,
    nDb,
    nD,
    nEb,
    nE,
    nF,
    nGb,
    nG,
    nAb,
    nA,
    nBb,
    nB
};

inline int floatToMidi (float f, bool learnspace = true)
{
    if (learnspace)
        return roundToInt (f * 128.f) - 1;
    return roundToInt (f * 127.f);
}
inline float midiToFloat (int n, bool learnspace = true)
{
    if (learnspace)
        return (float) (n + 1) * 0.0078125f;
    return (float) n * (float) 0.007874015748031496062992125984252;
}

inline int floatToChannel (float f) { return roundToInt (f * 16.0f); }
inline float channelToFloat (int c) { return (float) c * 0.0625f; }

inline int getNoteValue (String noteName)
{
    if (noteName.equalsIgnoreCase ("C") || noteName.equalsIgnoreCase ("B#") || noteName.equalsIgnoreCase ("Dbb"))
        return nC;
    if (noteName.equalsIgnoreCase ("C#") || noteName.equalsIgnoreCase ("Db") || noteName.equalsIgnoreCase ("Bx"))
        return nDb;
    if (noteName.equalsIgnoreCase ("D") || noteName.equalsIgnoreCase ("Ebb") || noteName.equalsIgnoreCase ("Cx"))
        return nD;
    if (noteName.equalsIgnoreCase ("Eb") || noteName.equalsIgnoreCase ("D#") || noteName.equalsIgnoreCase ("Fbb"))
        return nEb;
    if (noteName.equalsIgnoreCase ("E") || noteName.equalsIgnoreCase ("Fb") || noteName.equalsIgnoreCase ("Dx"))
        return nE;
    if (noteName.equalsIgnoreCase ("F") || noteName.equalsIgnoreCase ("E#") || noteName.equalsIgnoreCase ("Gbb"))
        return nF;
    if (noteName.equalsIgnoreCase ("Gb") || noteName.equalsIgnoreCase ("F#") || noteName.equalsIgnoreCase ("Ex"))
        return nGb;
    if (noteName.equalsIgnoreCase ("G") || noteName.equalsIgnoreCase ("Fx") || noteName.equalsIgnoreCase ("Abb"))
        return nG;
    if (noteName.equalsIgnoreCase ("Ab") || noteName.equalsIgnoreCase ("G#"))
        return nAb;
    if (noteName.equalsIgnoreCase ("A") || noteName.equalsIgnoreCase ("Gx") || noteName.equalsIgnoreCase ("Bbb"))
        return nA;
    if (noteName.equalsIgnoreCase ("Bb") || noteName.equalsIgnoreCase ("A#") || noteName.equalsIgnoreCase ("Cbb"))
        return nBb;
    if (noteName.equalsIgnoreCase ("B") || noteName.equalsIgnoreCase ("Cb") || noteName.equalsIgnoreCase ("Ax"))
        return nB;
    return NOT_A_NOTE;
}

inline int getNoteValue (String noteName, int bottomOctave, bool& hasOctaveNumber)
{
    int octave = 0;
    if (noteName.containsAnyOf ("-0123456789"))
    {
        octave          = (noteName.getTrailingIntValue() - bottomOctave) * 12;
        hasOctaveNumber = true;
        noteName        = noteName.removeCharacters ("-0123456789");
    }
    else
        hasOctaveNumber = false;
    if (noteName.equalsIgnoreCase ("C") || noteName.equalsIgnoreCase ("B#") || noteName.equalsIgnoreCase ("Dbb"))
        return nC + octave;
    if (noteName.equalsIgnoreCase ("C#") || noteName.equalsIgnoreCase ("Db") || noteName.equalsIgnoreCase ("Bx"))
        return nDb + octave;
    if (noteName.equalsIgnoreCase ("D") || noteName.equalsIgnoreCase ("Ebb") || noteName.equalsIgnoreCase ("Cx"))
        return nD + octave;
    if (noteName.equalsIgnoreCase ("Eb") || noteName.equalsIgnoreCase ("D#") || noteName.equalsIgnoreCase ("Fbb"))
        return nEb + octave;
    if (noteName.equalsIgnoreCase ("E") || noteName.equalsIgnoreCase ("Fb") || noteName.equalsIgnoreCase ("Dx"))
        return nE + octave;
    if (noteName.equalsIgnoreCase ("F") || noteName.equalsIgnoreCase ("E#") || noteName.equalsIgnoreCase ("Gbb"))
        return nF + octave;
    if (noteName.equalsIgnoreCase ("Gb") || noteName.equalsIgnoreCase ("F#") || noteName.equalsIgnoreCase ("Ex"))
        return nGb + octave;
    if (noteName.equalsIgnoreCase ("G") || noteName.equalsIgnoreCase ("Fx") || noteName.equalsIgnoreCase ("Abb"))
        return nG + octave;
    if (noteName.equalsIgnoreCase ("Ab") || noteName.equalsIgnoreCase ("G#"))
        return nAb + octave;
    if (noteName.equalsIgnoreCase ("A") || noteName.equalsIgnoreCase ("Gx") || noteName.equalsIgnoreCase ("Bbb"))
        return nA + octave;
    if (noteName.equalsIgnoreCase ("Bb") || noteName.equalsIgnoreCase ("A#") || noteName.equalsIgnoreCase ("Cbb"))
        return nBb + octave;
    if (noteName.equalsIgnoreCase ("B") || noteName.equalsIgnoreCase ("Cb") || noteName.equalsIgnoreCase ("Ax"))
        return nB + octave;
    return NOT_A_NOTE;
}

inline int getIntervalValue (String intervalName)
{
    int result    = NOT_A_NOTE;
    bool inverted = intervalName.startsWith ("-");
    if (inverted)
        intervalName = intervalName.fromFirstOccurrenceOf ("-", false, true);

    if (intervalName.equalsIgnoreCase ("r") || intervalName.equalsIgnoreCase ("root") || intervalName == "1")
        result = 0;
    else if (intervalName == "m2" || intervalName.equalsIgnoreCase ("min2") || intervalName == "b2")
        result = 1;
    else if (intervalName == "M2" || intervalName.equalsIgnoreCase ("maj2") || intervalName == "2")
        result = 2;
    else if (intervalName == "m3" || intervalName.equalsIgnoreCase ("min3") || intervalName == "b3")
        result = 3;
    else if (intervalName == "M3" || intervalName.equalsIgnoreCase ("maj3") || intervalName == "3"
             || intervalName == "b4" || intervalName == "o4" || intervalName.equalsIgnoreCase ("dim4"))
        result = 4;
    else if (intervalName.equalsIgnoreCase ("p4") || intervalName == "4")
        result = 5;
    else if (intervalName == "b5" || intervalName == "o5" || intervalName.equalsIgnoreCase ("dim5")
             || intervalName == "+4" || intervalName == "#4" || intervalName == "aug4" || intervalName.equalsIgnoreCase ("tritone"))
        result = 6;
    else if (intervalName.equalsIgnoreCase ("p5") || intervalName == "5")
        result = 7;
    else if (intervalName == "#5" || intervalName.equalsIgnoreCase ("aug5") || intervalName == "+5"
             || intervalName == "m6" || intervalName.equalsIgnoreCase ("min6") || intervalName == "b6")
        result = 8;
    else if (intervalName.equalsIgnoreCase ("maj6") || intervalName == "M6" || intervalName == "6"
             || intervalName == "o7" || intervalName == "bb7" || intervalName == "dim7")
        result = 9;
    else if (intervalName.equalsIgnoreCase ("min7") || intervalName == "b7" || intervalName == "m7"
             || intervalName == "+6" || intervalName.equalsIgnoreCase ("aug6"))
        result = 10;
    else if (intervalName.equalsIgnoreCase ("maj7") || intervalName == "M7" || intervalName == "7")
        result = 11;
    else if (intervalName.equalsIgnoreCase ("octave") || intervalName.equalsIgnoreCase ("o") || intervalName == "oct" || intervalName == "8")
        result = 12;
    else if (intervalName.equalsIgnoreCase ("min9") || intervalName == "b9" || intervalName == "m9")
        result = 13;
    else if (intervalName.equalsIgnoreCase ("maj9") || intervalName == "9" || intervalName == "M9")
        result = 14;
    else if (intervalName.equalsIgnoreCase ("min10") || intervalName == "#9" || intervalName == "b10" || intervalName == "m10")
        result = 15;
    else if (intervalName.equalsIgnoreCase ("maj10") || intervalName == "10" || intervalName == "M10" || intervalName == "b11")
        result = 16;
    else if (intervalName.equalsIgnoreCase ("p11") || intervalName == "11")
        result = 17;
    else if (intervalName.equalsIgnoreCase ("aug11") || intervalName == "#11" || intervalName == "#11")
        result = 18;
    else if (intervalName.equalsIgnoreCase ("p12") || intervalName == "12")
        result = 19;
    else if (intervalName.equalsIgnoreCase ("min13") || intervalName == "b13" || intervalName == "m13")
        result = 20;
    else if (intervalName.equalsIgnoreCase ("maj13") || intervalName == "13" || intervalName == "M13")
        result = 21;
    if (inverted)
        result = -result;
    return result;
}

inline String getNoteName (int noteNumber, int baseOctave /*=-2*/)
{
    String Note{};
    switch (noteNumber % 12)
    {
        case 0:
            Note = "C";
            break;
        case 1:
            Note = "C#";
            break;
        case 2:
            Note = "D";
            break;
        case 3:
            Note = "D#";
            break;
        case 4:
            Note = "E";
            break;
        case 5:
            Note = "F";
            break;
        case 6:
            Note = "F#";
            break;
        case 7:
            Note = "G";
            break;
        case 8:
            Note = "G#";
            break;
        case 9:
            Note = "A";
            break;
        case 10:
            Note = "A#";
            break;
        case 11:
            Note = "B";
            break;
        default:
            break;
    }
    return Note + String ((noteNumber / 12) + baseOctave);
}

inline String getNoteNameWithoutOctave (int noteNumber, bool sharps = true)
{
    String Note{};
    if (sharps)
    {
        switch (noteNumber % 12)
        {
            case 0:
                Note = "C";
                break;
            case 1:
                Note = "C" + Sharp;
                break;
            case 2:
                Note = "D";
                break;
            case 3:
                Note = "D" + Sharp;
                break;
            case 4:
                Note = "E";
                break;
            case 5:
                Note = "F";
                break;
            case 6:
                Note = "F" + Sharp;
                break;
            case 7:
                Note = "G";
                break;
            case 8:
                Note = "G" + Sharp;
                break;
            case 9:
                Note = "A";
                break;
            case 10:
                Note = "A" + Sharp;
                break;
            case 11:
                Note = "B";
                break;
            default:
                break;
        }
    }
    else
    {
        switch (noteNumber % 12)
        {
            case 0:
                Note = "C";
                break;
            case 1:
                Note = "D" + Flat;
                break;
            case 2:
                Note = "D";
                break;
            case 3:
                Note = "E" + Flat;
                break;
            case 4:
                Note = "E";
                break;
            case 5:
                Note = "F";
                break;
            case 6:
                Note = "G" + Flat;
                break;
            case 7:
                Note = "G";
                break;
            case 8:
                Note = "A" + Flat;
                break;
            case 9:
                Note = "A";
                break;
            case 10:
                Note = "B" + Flat;
                break;
            case 11:
                Note = "B";
                break;
            default:
                break;
        }
    }
    return Note;
}

inline int mapToRange (float x, float in1, float in2, float out1, float out2)
{
    float slope = ((float) (out2 - out1)) / ((float) (in2 - in1));
    float b     = out1 - slope * in1;
    return roundToInt (slope * x + b);
}

inline int CombineBytes (unsigned char lsb, unsigned char msb)
{
    return (((unsigned int) msb) << 7) | (unsigned int) lsb;
}

#endif
