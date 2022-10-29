#ifndef MIDI_H
#define MIDI_H
#include <string>

//
// MIDI Status Bytes --------------------------------------------------------------
#define MIDI_NONE 0x0

#define MIDI_NOTEOFF         0x80
#define MIDI_NOTEON          0x90
#define MIDI_POLYKEYPRESSURE 0xA0 // AKA Polyphonic Aftertouch
#define MIDI_CONTROLCHANGE   0xB0
#define MIDI_PROGRAMCHANGE   0xC0
#define MIDI_CHANNELPRESSURE 0xD0 // AKA Aftertouch
#define MIDI_PITCHBEND       0xE0

// Sysex and more
#define MIDI_SYSEX         0xF0
#define MIDI_SYSTEMCOMMON  0xF1 // Undefined System Common
#define MIDI_SONGPOSITION  0xF2
#define MIDI_SONGSELECT    0xF3
#define MIDI_F4            0xF4 // UNDEFINED IN MIDI SPEC
#define MIDI_F5            0xF5 // UNDEFINED IN MIDI SPEC
#define MIDI_TUNEREQUEST   0xF6
#define MIDI_EOX           0xF7 // End OF SysEx
#define MIDI_TIMINGCLOCK   0xF8
#define MIDI_F9            0xF9 // UNDEFINED IN MIDI SPEC
#define MIDI_START         0xFA
#define MIDI_CONTINUE      0xFB // NOTE SPELLING
#define MIDI_STOP          0xFC
#define MIDI_FD            0xFD // UNDEFINED IN MIDI SPEC
#define MIDI_ACTIVESENSING 0xFE // This signal may be sent by a MIDI transmitter when there is no other activity.
#define MIDI_SYSTEMRESET   0xFF

// control numbers for MIDI_CONTROL_CHANGE (MIDI status byte 0xB0)
// note: not a complete list

//MSBs
#define MIDI_BANK_CHANGE       0x00
#define MIDI_MODULATION_WHEEL  0x01
#define MIDI_BREATH_CONTROL    0x02
#define MIDI_UNDEFINED_CC03    0x03
#define MIDI_FOOT_CONTROLLER   0x04
#define MIDI_PORTAMENTO_TIME   0x05
#define MIDI_DATA_ENTRY        0x06
#define MIDI_CHANNEL_VOLUME    0x07
#define MIDI_BALANCE           0x08
#define MIDI_UNDEFINED_CC09    0x09
#define MIDI_PAN               0x0A
#define MIDI_EXPRESSION        0x0B
#define MIDI_EFFECT_CONTROL1   0x0C
#define MIDI_EFFECT_CONTROL2   0x0D
#define MIDI_UNDEFINED_CC0E    0x0E
#define MIDI_UNDEFINED_CC0F    0x0F
#define MIDI_GENERAL_PURPOSE_1 0x10
#define MIDI_GENERAL_PURPOSE_2 0x11
#define MIDI_GENERAL_PURPOSE_3 0x12
#define MIDI_GENERAL_PURPOSE_4 0x13
#define MIDI_UNDEFINED_CC14    0x14
#define MIDI_UNDEFINED_CC15    0x15
#define MIDI_UNDEFINED_CC16    0x16
#define MIDI_UNDEFINED_CC17    0x17
#define MIDI_UNDEFINED_CC18    0x18
#define MIDI_UNDEFINED_CC19    0x19
#define MIDI_UNDEFINED_CC1A    0x1A
#define MIDI_UNDEFINED_CC1B    0x1B
#define MIDI_UNDEFINED_CC1C    0x1C
#define MIDI_UNDEFINED_CC1D    0x1D
#define MIDI_UNDEFINED_CC1E    0x1E
#define MIDI_UNDEFINED_CC1F    0x1F
//[0x20:0x3F] are LSBs

#define MIDI_SUSTAIN           0x40
#define MIDI_PORTAMENTO        0x41
#define MIDI_SOSTENUTO         0x42
#define MIDI_SOFT              0x43
#define MIDI_HOLD_2            0x45
#define MIDI_GENERAL_PURPOSE_5 0x50
#define MIDI_GENERAL_PURPOSE_6 0x51
#define MIDI_GENERAL_PURPOSE_7 0x52
#define MIDI_GENERAL_PURPOSE_8 0x53

#define MIDI_EXTERNAL_EFFECTS_DEPTH 0x5B
#define MIDI_TREMELO_DEPTH          0x5C
#define MIDI_CHORUS_DEPTH           0x5D
#define MIDI_CELESTE_DEPTH          0x5E
#define MIDI_PHASER_DEPTH           0x5F
#define MIDI_DATA_INCREMENT         0x60
#define MIDI_DATA_DECREMENT         0x61
#define MIDI_NONREG_PARAM_NUM_MSB   0x62
#define MIDI_NONREG_PARAM_NUM_LSB   0x63
#define MIDI_REG_PARAM_NUM_MSB      0x64
#define MIDI_REG_PARAM_NUM_LSB      0x65

#define MIDI_ALL_SOUND_OFF         0x78
#define MIDI_RESET_ALL_CONTROLLERS 0x79
#define MIDI_LOCAL_CONTROL         0x7A
#define MIDI_ALL_NOTES_OFF         0x7B
#define MIDI_OMNI_MODE_OFF         0x7C
#define MIDI_OMNI_MODE_ON          0x7D
#define MIDI_MONO_MODE_ON          0x7E
#define MIDI_POLY_MODE_ON          0x7F

inline int roundToInt(double x)
{
    return x >= 0 ? static_cast<int>(x + 0.5) : static_cast<int>(x - 0.5);
}

#define MIDISCALER             0.00787401574803149606299212598425197f
#define MIDI_TO_FLOAT(i)       ((float) (i) *MIDISCALER)
#define MIDI_TO_FLOAT2(i)      ((float) (i) *0.0078125f)
#define MIDI_TO_FLOAT_X(i)     ((float) (i + 1) * 0.0078125f)
#define VELOCITY_TO_FLOAT(i)   ((float) (i - 1) * 0.0079365079365079365079365079365079f)
#define FLOAT_TO_MIDI(i)       (roundToInt((i) *127.0f))
#define FLOAT_TO_MIDI2(i)      (roundToInt((i) *128.0f)) // use '0' for "none"
#define FLOAT_TO_MIDI_X(i)     (roundToInt((i) *128.0f) - 1)
#define FLOAT_TO_VELOCITY(i)   (roundToInt((i) *126.0f) + 1) //1-127
#define CHANNEL_TO_FLOAT(i)    ((float) (i + 1) * 0.0625f)
#define CHANNEL_TO_FLOAT015(i) ((float) (i) *0.066666666666666666666666666666667f)
#define CHANNEL_TO_FLOAT016(i) ((float) (i) *0.0625f)
#define FLOAT_TO_CHANNEL(i)    (roundToInt((i) *16.0f) - 1) // use '-1' for all/any channel, then 0-15
#define FLOAT_TO_CHANNEL015(i) (roundToInt((i) *15.0f))
#define FLOAT_TO_CHANNEL016(i) (roundToInt((i) *16.0f)) // use '0' for all/any channel, then 1-16
#define ANY_CHANNEL            (-1)

#define MIDI_MAX_NOTES    128
#define MIDI_MAX_CC       128
#define MIDI_MAX_CHANNELS 16

#define PI 3.1415926535897932384626433832795

#define MAP_TO_MIDI1(i, low, high) ((float) i) * (((float) (high - low)) / 126) + ((float) low - ((float) (high - low)) / 126)

int MAP_TO_MIDI(float x, float y1, float y2, float x1, float x2)
{
    float slope = ((float) (y2 - y1)) / ((float) (x2 - x1));
    float y0    = y1 - slope * x1;
    float y     = slope * x + y0;
    return roundToInt(y);
}

int MAP_TO_MIDI(int x, int y1, int y2, int x1, int x2)
{
    float slope = ((float) (y2 - y1)) / ((float) (x2 - x1));
    float y0    = (float) y1 - slope * (float) x1;
    float y     = slope * (float) x + (float) y0;
    return roundToInt(y);
}

char* getNoteName(int noteNumber)
{
    const char* Note;
    Note = new char[4];
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
            Note = "?";
            break;
    }
    int Octave     = (noteNumber / 12) - 2;
    char* noteName = new char[5];
    sprintf(noteName, "%s%d", Note, Octave);
    return noteName;
}

char* getNoteNameWithoutOctave(int noteNumber, bool flats = false)
{
    const char* Note;
    Note = new char[4];
    switch (noteNumber % 12)
    {
        case 0:
            Note = "C";
            break;
        case 1:
            Note = flats ? "Db" : "C#";
            break;
        case 2:
            Note = "D";
            break;
        case 3:
            Note = flats ? "Eb" : "D#";
            break;
        case 4:
            Note = "E";
            break;
        case 5:
            Note = "F";
            break;
        case 6:
            Note = flats ? "Gb" : "F#";
            break;
        case 7:
            Note = "G";
            break;
        case 8:
            Note = flats ? "Ab" : "G#";
            break;
        case 9:
            Note = "A";
            break;
        case 10:
            Note = flats ? "Bb" : "A#";
            break;
        case 11:
            Note = "B";
            break;
        default:
            Note = "?";
            break;
    }
    char* noteName = new char[5];
    sprintf(noteName, "%s", Note);
    return noteName;
}

bool isBlackKey(int noteNumber)
{
    switch (noteNumber % 12)
    {
        case 0:
            return false; //C
        case 1:
            return true; //C#
        case 2:
            return false; //D
        case 3:
            return true; //D#
        case 4:
            return false; //E
        case 5:
            return false; //F
        case 6:
            return true; //F#
        case 7:
            return false; //G
        case 8:
            return true; //G#
        case 9:
            return false; //A
        case 10:
            return true; //A#
        case 11:
            return false; //B

        default:
            return false;
    }
}

inline int midiLimit(int data)
{
    if (data < 0)
    {
        return 0;
    }
    if (data > 127)
    {
        return 127;
    }
    return data;
}

inline int midiNoteWrap(int n)
{
    while (n < 0)
    {
        n += 12;
    }
    while (n > 127)
    {
        n -= 12;
    }
    return n;
}

inline int velocityLimit(int data)
{
    if (data < 1)
    {
        return 1;
    }
    if (data > 127)
    {
        return 127;
    }
    return data;
}

inline int CombineBytes(unsigned char lsb, unsigned char msb)
{
    return (((unsigned int) msb) << 7) | (unsigned int) lsb;
}

inline int CombineBytes(int lsb, int msb)
{
    return ((msb & 0x7f) << 7) | (lsb & 0x7f);
}

//////////////////////////////////////////////////////////////////////////////
/*  (http://www.musique.umontreal.ca/electro/MAX/MidiTable4.html)
         Table 4: Summary of MIDI Note Numbers for Different Octaves
 (adapted from "MIDI by the Numbers" by D. Valenti - Electronic Musician 2/88)
               Updated 1995 By the MIDI Manufacturers Association

Octave||                     Note Numbers
   #  ||
      || C   | C#  | D   | D#  | E   | F   | F#  | G   | G#  | A   | A#  | B
------------------------------------------------------------------------------
  -1  ||   0 |   1 |   2 |   3 |   4 |   5 |   6 |   7 |   8 |   9 |  10 |  11
   0  ||  12 |  13 |  14 |  15 |  16 |  17 |  18 |  19 |  20 |  21 |  22 |  23
   1  ||  24 |  25 |  26 |  27 |  28 |  29 |  30 |  31 |  32 |  33 |  34 |  35
   2  ||  36 |  37 |  38 |  39 |  40 |  41 |  42 |  43 |  44 |  45 |  46 |  47
   3  ||  48 |  49 |  50 |  51 |  52 |  53 |  54 |  55 |  56 |  57 |  58 |  59
   4  ||  60 |  61 |  62 |  63 |  64 |  65 |  66 |  67 |  68 |  69 |  70 |  71
   5  ||  72 |  73 |  74 |  75 |  76 |  77 |  78 |  79 |  80 |  81 |  82 |  83
   6  ||  84 |  85 |  86 |  87 |  88 |  89 |  90 |  91 |  92 |  93 |  94 |  95
   7  ||  96 |  97 |  98 |  99 | 100 | 101 | 102 | 103 | 104 | 105 | 106 | 107
   8  || 108 | 109 | 110 | 111 | 112 | 113 | 114 | 115 | 116 | 117 | 118 | 119
   9  || 120 | 121 | 122 | 123 | 124 | 125 | 126 | 127 |
*/

#endif
