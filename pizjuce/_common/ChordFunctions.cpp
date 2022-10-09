#include "ChordFunctions.h"

PizChord::PizChord (Array<int> newChord)
{
    chord.addArray (newChord);
    makeIntervalPattern();
}
void PizChord::setChord (Array<int> newChord)
{
    chord.clear();
    chord.addArray (newChord);
    makeIntervalPattern();
}
int PizChord::getSum() const
{
    int sum = 0;
    for (int i = 0; i < pattern.size(); i++)
        sum += pattern[i];
    return sum;
}
String PizChord::getStringPattern() const
{
    String p;
    for (int i = 0; i < pattern.size(); i++)
    {
        p += String (pattern[i]) + ",";
    }
    return p;
}
void PizChord::makeIntervalPattern()
{
    if (chord.size() < 2)
    {
        pattern.add (0);
        return;
    }
    int interval = 0;
    for (int i = 0; i < chord.size() - 1; i++)
    {
        interval = chord[i + 1] - chord[i];
        while (interval < 0)
            interval += 12;
        pattern.add (interval);
    }
}

bool operator<(const PizChord& first, const PizChord& second)
{
    int f = first.getSum();
    int s = second.getSum();
    if (f != s)
        return f < s;

    return first.getStringPattern() < second.getStringPattern();
}

ChordName::ChordName (String chordName, String noteString)
{
    name    = chordName;
    pattern = getIntervalString (noteString);
}

String ChordName::getName (int rootNote, int bassNote, bool flats)
{
    if (name == "dim7" || name == "+")
        rootNote = bassNote;
    String chordName = getNoteNameWithoutOctave (rootNote, ! flats) + name;
    if (bassNote % 12 != rootNote % 12)
        chordName += "/" + getNoteNameWithoutOctave (bassNote, ! flats);
    return chordName;
}

bool ChordName::equals (String& noteString)
{
    return getIntervalString (noteString) == pattern;
}
bool ChordName::equals (Array<int>& chord)
{
    return getIntervalString (chord).equalsIgnoreCase (pattern);
}
bool ChordName::equals2 (String& intervalString)
{
    return intervalString.equalsIgnoreCase (pattern);
}
String ChordName::getIntervalString (String noteString)
{
    String p;
    StringArray a;
    a.addTokens (noteString, ",", String());

    Array<int> temp;
    for (int i = 0; i < a.size(); i++)
    {
        temp.add (getNoteValue (a[i]));
    }
    root               = temp[0];
    Array<int> stacked = getAsStackedChord (temp);
    for (int i = 0; i < stacked.size(); i++)
    {
        if (stacked[i] == root)
            rootIndex = i;
        if (i < stacked.size() - 1)
        {
            int interval = stacked[i + 1] - stacked[i];
            while (interval < 0)
                interval += 12;
            p += String (interval) + ",";
        }
    }
    return p;
}

String ChordName::getIntervalString (Array<int> chord)
{
    Array<int> newChord = getAsStackedChord (chord);
    String p;
    for (int i = 0; i < chord.size() - 1; i++)
    {
        int interval = chord[i + 1] - chord[i];
        while (interval < 0)
            interval += 12;
        p += String (interval) + ",";
    }
    return p;
}

Array<int> getAsStackedChord (Array<int>& chord, bool reduce)
{
    Array<int> temp;
    if (reduce)
    {
        //remove duplicate notes
        for (int i = 0; i < chord.size(); i++)
        {
            temp.addIfNotAlreadyThere (chord[i] % 12);
        }
        if (temp.size() == 1)
            return temp;
    }
    else
        temp.addArray (chord);

    //sort
    DefaultElementComparator<int> intsorter;
    temp.sort (intsorter);

    int Minimum = -1;
    //the base chord is the one with the minimal energy function
    //the energy function is the sum of the number of half steps between successive notes in the chord
    //hence, the canonic chord is the chord in which the notes lie as closely spaced as possible
    Array<PizChord> MinimumEnergyChordList;

    PizChord tempChord0 (temp);
    //DBG("permutation: " + tempChord0.getStringPattern());
    Minimum = tempChord0.getSum();
    MinimumEnergyChordList.add (tempChord0);

    while (std::next_permutation (temp.begin(), temp.end()))
    {
        PizChord tempChord (temp);
        //DBG("permutation: " + tempChord.getStringPattern());
        int S = tempChord.getSum();
        if (S < Minimum || Minimum == -1)
        {
            //new minimum found => discard what we found up to now
            Minimum = S;
            MinimumEnergyChordList.clear();
            MinimumEnergyChordList.add (tempChord);
        }
        else if (S == Minimum)
        {
            //another chord with minimum energy => add it to the list
            MinimumEnergyChordList.add (tempChord);
        }
    }
    //now we face a problem: multiple minimum energy chords can exist if more than 3 notes are present
    //we need a way of picking one chord that will always lead to the same interval pattern, no matter
    //what note names are used. Normal "sort" will sort alphabetically. This is unusable.
    //We need to sort on the interval patterns instead, but return the chord that corresponds to the pattern.
    DefaultElementComparator<PizChord> sorter;
    MinimumEnergyChordList.sort (sorter);
    //DBG("picked" + MinimumEnergyChordList.getFirst().getStringPattern());
    return MinimumEnergyChordList.getFirst().getChord();
}

void fillChordDatabase()
{
    if (ChordNames.size() > 0)
        return;
    ChordNames.add (ChordName ("", "c,e,g"));
    ChordNames.add (ChordName ("5", "c,g"));
    ChordNames.add (ChordName ("6(no3)", "c,g,a"));
    ChordNames.add (ChordName ("Maj7", "c,e,g,b"));
    ChordNames.add (ChordName ("Maj7(#11)", "c,e,f#,b"));
    ChordNames.add (ChordName ("add9", "c,e,g,d"));
    ChordNames.add (ChordName ("Maj7(9)", "c,d,e,b"));
    ChordNames.add (ChordName ("6(9)", "c,d,e,a"));
    ChordNames.add (ChordName ("+", "c,e,g#"));
    ChordNames.add (ChordName ("m", "c,eb,g"));
    ChordNames.add (ChordName ("madd9", "c,eb,g,d"));
    ChordNames.add (ChordName ("m7", "c,eb,g,bb"));
    ChordNames.add (ChordName ("m7(9)", "c,d,eb,bb"));
    ChordNames.add (ChordName ("mMaj7", "c,eb,g,b"));
    ChordNames.add (ChordName ("mMaj7(9)", "c,d,eb,b"));
    ChordNames.add (ChordName ("dim", "c,eb,f#"));
    ChordNames.add (ChordName ("dim7", "c,eb,f#,a"));
    ChordNames.add (ChordName ("7", "c,e,g,bb"));
    ChordNames.add (ChordName ("7(no5)", "c,e,bb"));
    ChordNames.add (ChordName ("7sus4", "c,f,g,bb"));
    ChordNames.add (ChordName ("7(b5)", "c,e,f#,bb"));
    ChordNames.add (ChordName ("7(9)", "c,d,e,bb"));
    ChordNames.add (ChordName ("7(13)", "c,e,a,bb"));
    ChordNames.add (ChordName ("7(b9)", "c,c#,e,bb"));
    ChordNames.add (ChordName ("+7", "c,e,g#,bb")); //7(b13)
    ChordNames.add (ChordName ("7(#9)", "c,eb,e,bb"));
    ChordNames.add (ChordName ("sus4", "c,f,g"));
    ChordNames.add (ChordName ("6add9", "c,e,g,a,d"));
    ChordNames.add (ChordName ("Maj9", "c,e,g,b,d"));
    ChordNames.add (ChordName ("9", "c,e,g,bb,d"));
    ChordNames.add (ChordName ("13", "c,e,g,bb,d,a"));
    ChordNames.add (ChordName ("13", "c,e,g,bb,d,f,a"));
    ChordNames.add (ChordName ("13", "c,e,bb,d,a"));
    ChordNames.add (ChordName ("m6", "c,eb,g,a"));
    ChordNames.add (ChordName ("m6add9", "c,eb,g,a,d"));
    ChordNames.add (ChordName ("m6/9", "c,eb,a,d"));
    ChordNames.add (ChordName ("m7add13", "c,eb,g,a,bb"));
    ChordNames.add (ChordName ("m9", "c,eb,g,bb,d"));
    ChordNames.add (ChordName ("m11", "c,eb,g,bb,d,f"));
    ChordNames.add (ChordName ("m11", "c,eb,bb,d,f"));
    ChordNames.add (ChordName ("m13", "c,eb,g,bb,d,f,a"));
    ChordNames.add (ChordName ("m9/Maj7", "c,eb,g,b,d"));
    ChordNames.add (ChordName ("m9(b5)", "c,eb,gb,bb,d"));
    ChordNames.add (ChordName ("m11(b5)", "c,eb,gb,bb,d,f"));
    ChordNames.add (ChordName ("Maj7(#5)", "c,e,g#,b"));
    ChordNames.add (ChordName ("Maj7(#11)", "c,e,g,b,f#"));
    ChordNames.add (ChordName ("Maj9(#11)", "c,e,g,b,d,f#"));
    ChordNames.add (ChordName ("7(b9)", "c,e,g,bb,db"));
    ChordNames.add (ChordName ("7(#9)", "c,e,g,bb,d#"));
    ChordNames.add (ChordName ("7(#5)(#9)", "c,e,g#,bb,d#"));
    ChordNames.add (ChordName ("7(#11)", "c,e,g,bb,f#"));
    ChordNames.add (ChordName ("9(#11)", "c,e,g,bb,d,f#"));
    ChordNames.add (ChordName ("7(b9)(#11)", "c,e,g,bb,db,f#"));
    ChordNames.add (ChordName ("13b5", "c,e,gb,bb,d,a"));
    ChordNames.add (ChordName ("13b5", "c,e,gb,bb,d,f,a"));
    ChordNames.add (ChordName ("13b9", "c,e,g,bb,db,a"));
    ChordNames.add (ChordName ("13b9", "c,e,g,bb,db,f,a"));
    ChordNames.add (ChordName ("13#11", "c,e,g,bb,d,f#,a"));
    ChordNames.add (ChordName ("7(no3)", "c,g,bb"));
    ChordNames.add (ChordName ("Maj7(no5)", "c,e,b"));

    ChordNames.add (ChordName ("13b5b9", "c,e,gb,bb,db,a"));
    ChordNames.add (ChordName ("13b5b9", "c,e,gb,bb,db,f,a"));

    ChordNames.add (ChordName ("7#9#11", "c,e,g,bb,d#,f#"));
    ChordNames.add (ChordName ("add#9", "c,e,g,d#"));
    ChordNames.add (ChordName ("addb9", "c,e,g,db"));
    ChordNames.add (ChordName ("11", "c,g,bb,d,f"));
    ChordNames.add (ChordName ("11", "c,e,g,bb,d,f"));
    ChordNames.add (ChordName ("add11", "c,e,g,f"));
    ChordNames.add (ChordName ("Maj11", "c,e,g,b,d,f"));
    ChordNames.add (ChordName ("Maj7add11", "c,e,g,b,f"));
    ChordNames.add (ChordName ("7add11", "c,e,g,bb,f"));
    ChordNames.add (ChordName ("m7b9", "c,eb,g,bb,db"));
    ChordNames.add (ChordName ("+Maj9", "c,e,g#,b,d"));
    ChordNames.add (ChordName ("+9", "c,e,g#,bb,d"));
    ChordNames.add (ChordName ("mMaj11", "c,eb,g,b,d,f"));
    ChordNames.add (ChordName ("+Maj11", "c,e,g#,b,d,f"));
    ChordNames.add (ChordName ("+13", "c,e,g#,bb,d,f,a"));
    ChordNames.add (ChordName ("+Maj13", "c,e,g#,b,d,f,a"));
    ChordNames.add (ChordName ("Maj7sus4", "c,f,g,b"));
    ChordNames.add (ChordName ("Maj9sus4", "c,f,g,b,d"));
    ChordNames.add (ChordName ("7add13", "c,e,g,bb,a"));
    ChordNames.add (ChordName ("mMaj7add11", "c,eb,g,b,f"));
    ChordNames.add (ChordName ("7add11", "c,e,g,bb,f"));

    //ChordNames.add(ChordName("(It+6)"		, "ab,c,f#"));
    //ChordNames.add(ChordName("(Fr+6)"		, "ab,c,d,f#"));
    //ChordNames.add(ChordName("(Gr+6)"		, "ab,c,eb,f#"));

    //ChordNames.add(ChordName("Maj7aug"      , "c,e,g#,b"));
    //ChordNames.add(ChordName("1+2+5"        , "c,d,g"));
    //ChordNames.add(ChordName("Maj7add(13) " , "c,e,g,a,b"));
    //ChordNames.add(ChordName("Maj13"        , "c,e,g,b,d,a"));
    //ChordNames.add(ChordName("m7add(11)"    , "c,eb,g,bb,f"));
    //ChordNames.add(ChordName("m7(b5)"       , "c,eb,gb,bb"));
    //ChordNames.add(ChordName("o7"           , "c,eb,gb,a"));
    //ChordNames.add(ChordName("o7add(Maj7) " , "c,eb,gb,a,b"));
    //ChordNames.add(ChordName("sus7"         , "c,f,g,bb"));
    //ChordNames.add(ChordName("sus9"         , "c,f,g,bb,d"));
    //ChordNames.add(ChordName("sus13"        , "c,f,g,bb,d,a"));
    //ChordNames.add(ChordName("sus7"         , "c,g,bb"));
    //ChordNames.add(ChordName("Maj7(b5)"     , "c,e,gb,b"));
    //ChordNames.add(ChordName("Maj13(#11)"   , "c,e,g,b,d,f#,a"));
    //ChordNames.add(ChordName("7(#5)"        , "c,e,g#,bb"));
    //ChordNames.add(ChordName("9(#5)"        , "c,e,g#,bb,d"));
    //ChordNames.add(ChordName("7(b5)(b9)"    , "c,e,gb,bb,db"));
    //ChordNames.add(ChordName("7(#5)(b9)"    , "c,e,g#,bb,db"));
    //ChordNames.add(ChordName("sus7(b9)"     , "c,f,g,bb,db"));
    //ChordNames.add(ChordName("sus13(b9)"    , "c,f,g,bb,db,a"));
    //ChordNames.add(ChordName("m7(no5)"      , "c,eb,bb"));
}

String getIntervalName (int semitones)
{
    while (semitones > 21)
        semitones -= 12;

    switch (semitones)
    {
        case 0:
            return "Unison";
        case 1:
            return "Minor 2nd";
        case 2:
            return "Major 2nd";
        case 3:
            return "Minor 3rd";
        case 4:
            return "Major 3rd";
        case 5:
            return "Perfect 4th";
        case 6:
            return "Tritone";
        case 7:
            return "Perfect 5th";
        case 8:
            return "Minor 6th (or #5)";
        case 9:
            return "Major 6th";
        case 10:
            return "Minor 7th";
        case 11:
            return "Major 7th";
        case 12:
            return "Octave";
        case 13:
            return "Flat 9th";
        case 14:
            return "9th";
        case 15:
            return "Minor 10th (or #9)";
        case 16:
            return "Major 10th";
        case 17:
            return "11th";
        case 18:
            return "Augmented 11th";
        case 19:
            return "Perfect 12th";
        case 20:
            return "Flat 13th";
        case 21:
            return "13th";
        default:
            return String();
    }
}

String listNoteNames (Array<int> chord, bool flats)
{
    String s;
    for (int i = 0; i < chord.size() - 1; i++)
        s += getNoteNameWithoutOctave (chord[i], ! flats) + ", ";
    s += getNoteNameWithoutOctave (chord[chord.size() - 1], ! flats);
    return s;
}

String getFirstRecognizedChord (Array<int> chord, bool flats)
{
    if (chord.size() == 0)
        return " ";
    if (chord.size() == 2)
        return getIntervalName (chord[1] - chord[0]) + " (" + listNoteNames (chord, flats) + ")";

    Array<int> temp;
    for (int i = 0; i < chord.size(); i++)
    {
        temp.addIfNotAlreadyThere (chord[i] % 12);
    }

    if (temp.size() >= 9)
        return getNoteNameWithoutOctave (chord[0], ! flats) + " Note Soup";

    Array<int> stackedChord = getAsStackedChord (temp, false);
    if (stackedChord.size() == 1)
        return "(" + getNoteNameWithoutOctave (stackedChord[0], ! flats) + ")";

    String s = ChordName::getIntervalString (stackedChord);
    for (int i = 0; i < ChordNames.size(); i++)
    {
        if (ChordNames.getReference (i).equals2 (s))
        {
            return ChordNames.getReference (i).getName (stackedChord[ChordNames.getReference (i).getRootIndex()], chord[0], flats);
        }
    }
    return "(" + listNoteNames (temp, flats) + ")"; // "Unknown chord";
}

String getIntervalStringFromNoteNames (int root, String noteString, int bottomOctave)
{
    bool multichannel = noteString.contains (".");
    StringArray sa;
    sa.addTokens (noteString, " ,", String());
    bool absolute;
    int bass = getNoteValue (sa[0].upToFirstOccurrenceOf (".", false, false), bottomOctave, absolute);
    int last = 0;
    String string;
    if (bass != NOT_A_NOTE)
    {
        int channel = multichannel ? sa[0].fromFirstOccurrenceOf (".", false, false).getIntValue() : 0;

        if (! absolute)
        {
            root %= 12;
            if (bass > root)
                bass -= 12;
            last = bass - root;
            if (channel > 0)
                string += String (last) + "." + String (channel);
            else
                string += String (last);
            for (int i = 1; i < sa.size(); i++)
            {
                const int note = getNoteValue (sa[i].upToFirstOccurrenceOf (".", false, false));
                if (note != NOT_A_NOTE)
                {
                    int step = note - root;
                    while (step - last < 0)
                        step += 12;
                    channel = sa[i].fromFirstOccurrenceOf (".", false, false).getIntValue();
                    if (channel > 0)
                        string += " " + String (step) + "." + String (channel);
                    else
                        string += " " + String (step);
                    last = step;
                }
            }
        }
        else
        {
            last = bass - root;
            if (channel > 0)
                string += String (last) + "." + String (channel);
            else
                string += String (last);
            for (int i = 1; i < sa.size(); i++)
            {
                const int note = getNoteValue (sa[i].upToFirstOccurrenceOf (".", false, false), bottomOctave, absolute);
                if (note != NOT_A_NOTE)
                {
                    int step = note - root;
                    while (step - last < 0)
                        step += 12;
                    channel = sa[i].fromFirstOccurrenceOf (".", false, false).getIntValue();
                    if (channel > 0)
                        string += " " + String (step) + "." + String (channel);
                    else
                        string += " " + String (step);
                    last = step;
                }
            }
        }
    }
    else
    {
        root %= 12;
        bass = getIntervalValue (sa[0].upToFirstOccurrenceOf (".", false, false));
        if (bass > root)
            bass -= 12;
        last        = bass - root;
        int channel = multichannel ? sa[0].fromFirstOccurrenceOf (".", false, false).getIntValue() : 0;
        if (channel > 0)
            string += String (last) + "." + String (channel);
        else
            string += String (last);
        for (int i = 1; i < sa.size(); i++)
        {
            const int note = getIntervalValue (sa[i].upToFirstOccurrenceOf (".", false, false));
            if (note != NOT_A_NOTE)
            {
                int step = note - root;
                while (step - last < 0)
                    step += 12;
                if (channel > 0)
                    string += " " + String (step) + "." + String (channel);
                else
                    string += " " + String (step);
                last = step;
            }
        }
    }
    return string;
}
