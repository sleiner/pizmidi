#include <string>
//for sort()
#include <algorithm>
//for seeding rand()
#include <time.h>

#include "midiChordSplit.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    dbg ("createEffectInstance");
    return new MidiChordSplit (audioMaster);
}

MidiChordSplitProgram::MidiChordSplitProgram()
{
    // default Program Values
    memset (param, 0, sizeof (param));
    for (int i = 0; i < kNumParams; i++)
    {
        switch (i)
        {
            case kPower:
                param[i] = 1.0f;
                break;
            case kCh1Power:
                param[i] = 1.f;
                break;
            case kCh2Power:
                param[i] = 1.f;
                break;
            case kCh3Power:
                param[i] = 1.f;
                break;
            case kCh4Power:
                param[i] = 1.f;
                break;
            case kCh4Low:
                param[i] = MIDI_TO_FLOAT (64);
                break;
            case kCh4High:
                param[i] = MIDI_TO_FLOAT (79);
                break;
            case kCh3Low:
                param[i] = MIDI_TO_FLOAT (59);
                break;
            case kCh3High:
                param[i] = MIDI_TO_FLOAT (71);
                break;
            case kCh2Low:
                param[i] = MIDI_TO_FLOAT (54);
                break;
            case kCh2High:
                param[i] = MIDI_TO_FLOAT (64);
                break;
            case kCh1Low:
                param[i] = MIDI_TO_FLOAT (43);
                break;
            case kCh1High:
                param[i] = MIDI_TO_FLOAT (59);
                break;

            case kCh5High:
            case kCh6High:
            case kCh7High:
            case kCh8High:
            case kCh9High:
            case kCh10High:
            case kCh11High:
            case kCh12High:
            case kCh13High:
            case kCh14High:
            case kCh15High:
            case kCh16High:
                param[i] = 1.0f;
                break;
            //case kCh1Octave:
            //case kCh2Octave:
            //case kCh3Octave:
            //case kCh4Octave:
            //case kCh5Octave:
            //case kCh6Octave:
            //case kCh7Octave:
            //case kCh8Octave:
            //case kCh9Octave:
            //case kCh10Octave:
            //case kCh11Octave:
            //case kCh12Octave:
            //case kCh13Octave:
            //case kCh14Octave:
            //case kCh15Octave:
            //case kCh16Octave:
            //	param[i]  = 0.5f; break;
            default:
                break;
        }
    }
    // default program name
    strcpy (name, "Default");
}

bool MidiChordSplit::midiSort::operator() (const VstMidiEvent& first, const VstMidiEvent& second)
{
    unsigned int r = rand();
    if (this->mode == random)
        return (r < (int) (RAND_MAX / 2)) ? true : false;
    bool result = false;
    if (first.deltaFrames == second.deltaFrames
        && isNoteOnOrOff (first) && isNoteOnOrOff (second))
    {
        if (first.midiData[1] == second.midiData[1])
        {
            if (isNoteOn (first) && isNoteOff (second))
                return false;
            if (isNoteOff (first) && isNoteOn (second))
                return true;
        }
        bool bothOff = isNoteOff (first) && isNoteOff (second);
        switch (mode)
        {
            case quietest:
                if (bothOff)
                    result = onvelocity[first.midiData[0] & 0x0f][first.midiData[1]]
                           < onvelocity[second.midiData[0] & 0x0f][second.midiData[1]];
                else
                    result = first.midiData[2] > second.midiData[2];
                break;
            case lowest:
                if (bothOff)
                    result = first.midiData[1] < second.midiData[1];
                else
                    result = first.midiData[1] > second.midiData[1];
                break;
            case highest:
                if (bothOff)
                    result = first.midiData[1] > second.midiData[1];
                else
                    result = first.midiData[1] < second.midiData[1];
                break;
            case centered:
                if (abs (first.midiData[1] - centerNote) == abs (second.midiData[1] - centerNote))
                {
                    if (bothOff)
                        result = first.midiData[1] > second.midiData[1];
                    else
                        result = first.midiData[1] < second.midiData[1];
                }
                else
                {
                    if (bothOff)
                        result = (abs (first.midiData[1] - centerNote) > abs (second.midiData[1] - centerNote));
                    else
                        result = (abs (first.midiData[1] - centerNote) < abs (second.midiData[1] - centerNote));
                }
                break;
            case random:
                result = (r < (int) (RAND_MAX / 2)) ? true : false;
                break;
            default:
                if (bothOff)
                    result = first.midiData[1] > second.midiData[1];
                else
                    result = first.midiData[1] < second.midiData[1];
                break;
        }
    }
    else if (first.deltaFrames < second.deltaFrames)
        result = true;
    if (isNoteOn (first))
        priorityNote = first.midiData[1];
    return result;
}

//-----------------------------------------------------------------------------
MidiChordSplit::MidiChordSplit (audioMasterCallback audioMaster)
    : PizMidi (audioMaster, kNumPrograms, kNumParams),
      programs (0)
{
    programs = new MidiChordSplitProgram[numPrograms];
    memset (param, 0, sizeof (param));
    if (programs)
    {
        CFxBank* defaultBank = new CFxBank (kNumPrograms, kNumParams);
        if (readDefaultBank (PLUG_NAME, defaultBank))
        {
            if ((VstInt32) defaultBank->GetFxID() == PLUG_IDENT)
            {
                for (int i = 0; i < kNumPrograms; i++)
                {
                    for (int p = 0; p < kNumParams; p++)
                    {
                        programs[i].param[p] = defaultBank->GetProgParm (i, p);
                    }
                    strcpy (programs[i].name, defaultBank->GetProgramName (i));
                }
            }
            dbg ("bank file loaded");
        }
        else
        {
            // built-in programs
            for (int i = 0; i < kNumPrograms; i++)
            {
                sprintf (programs[i].name, "Program %d", i + 1);
            }
            dbg ("default programs loaded");
        }
        memcpy (param, programs[0].param, sizeof (param));
        setProgram (0);
    }

    init();
    dbg ("constructor end");
}

//-----------------------------------------------------------------------------------------
MidiChordSplit::~MidiChordSplit()
{
    dbg ("destructor start");
    if (programs)
        delete[] programs;
    dbg ("destructor end");
}

//------------------------------------------------------------------------
void MidiChordSplit::setProgram (VstInt32 program)
{
    //dbg("setProgram("<<program<<")");
    MidiChordSplitProgram* ap = &programs[program];

    curProgram = program;
    for (int i = 0; i < kNumParams; i++)
        setParameter (i, ap->param[i]);
}

//------------------------------------------------------------------------
void MidiChordSplit::setProgramName (char* name)
{
    //dbg("setProgramName("<<name<<")");
    vst_strncpy (programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiChordSplit::getProgramName (char* name)
{
    strcpy (name, programs[curProgram].name);
    //dbg("getProgramName("<<name<<")");
}

//-----------------------------------------------------------------------------------------
bool MidiChordSplit::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
    //dbg("getProgramNameIndexed("<<index<<")");
    if (index < kNumPrograms)
    {
        strcpy (text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiChordSplit::setParameter (VstInt32 index, float value)
{
    dbg ("setParameter(" << index << "," << value << ")");
    MidiChordSplitProgram* ap = &programs[curProgram];
    param[index] = ap->param[index] = value;
    switch (index)
    {
        case kSteal:
            if (param[index] < 1.f / (float) (numStealModes))
                stealmode = oldest;
            else if (param[index] < 2.f / (float) (numStealModes))
                stealmode = newest;
            else if (param[index] < 3.f / (float) (numStealModes))
                stealmode = quietest;
            else if (param[index] < 4.f / (float) (numStealModes))
                stealmode = lowest;
            else if (param[index] < 5.f / (float) (numStealModes))
                stealmode = highest;
            else if (param[index] < 6.f / (float) (numStealModes))
                stealmode = centered;
            else
                stealmode = random;

            sorter.mode = stealmode;
            break;
        case kCh1Low:
        case kCh2Low:
        case kCh3Low:
        case kCh4Low:
        case kCh5Low:
        case kCh6Low:
        case kCh7Low:
        case kCh8Low:
        case kCh9Low:
        case kCh10Low:
        case kCh11Low:
        case kCh12Low:
        case kCh13Low:
        case kCh14Low:
        case kCh15Low:
        case kCh16Low:
            if (value > param[index + 1])
                setParameterAutomated (index + 1, value);
            break;
        case kCh1High:
        case kCh2High:
        case kCh3High:
        case kCh4High:
        case kCh5High:
        case kCh6High:
        case kCh7High:
        case kCh8High:
        case kCh9High:
        case kCh10High:
        case kCh11High:
        case kCh12High:
        case kCh13High:
        case kCh14High:
        case kCh15High:
        case kCh16High:
            if (value < param[index - 1])
                setParameterAutomated (index - 1, value);
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
float MidiChordSplit::getParameter (VstInt32 index)
{
    //dbg("getParameter("<<index<<")");
    return param[index];
}

//-----------------------------------------------------------------------------------------
void MidiChordSplit::getParameterName (VstInt32 index, char* label)
{
    switch (index)
    {
        case kPower:
            strcpy (label, "Power");
            break;
        case kSteal:
            strcpy (label, "NotePriority");
            break;
        case kInCh:
            strcpy (label, "InChannel");
            break;
        case kCh1Power:
        case kCh2Power:
        case kCh3Power:
        case kCh4Power:
        case kCh5Power:
        case kCh6Power:
        case kCh7Power:
        case kCh8Power:
        case kCh9Power:
        case kCh10Power:
        case kCh11Power:
        case kCh12Power:
        case kCh13Power:
        case kCh14Power:
        case kCh15Power:
        case kCh16Power:
            sprintf (label, "Ch%dPower", (index - kCh1Power) / 4 + 1);
            break;
        case kCh1Poly:
        case kCh2Poly:
        case kCh3Poly:
        case kCh4Poly:
        case kCh5Poly:
        case kCh6Poly:
        case kCh7Poly:
        case kCh8Poly:
        case kCh9Poly:
        case kCh10Poly:
        case kCh11Poly:
        case kCh12Poly:
        case kCh13Poly:
        case kCh14Poly:
        case kCh15Poly:
        case kCh16Poly:
            sprintf (label, "Ch%dPoly", (index - kCh1Poly) / 4 + 1);
            break;
        case kCh1Low:
        case kCh2Low:
        case kCh3Low:
        case kCh4Low:
        case kCh5Low:
        case kCh6Low:
        case kCh7Low:
        case kCh8Low:
        case kCh9Low:
        case kCh10Low:
        case kCh11Low:
        case kCh12Low:
        case kCh13Low:
        case kCh14Low:
        case kCh15Low:
        case kCh16Low:
            sprintf (label, "Ch%dLow", (index - kCh1Low) / 4 + 1);
            break;
        case kCh1High:
        case kCh2High:
        case kCh3High:
        case kCh4High:
        case kCh5High:
        case kCh6High:
        case kCh7High:
        case kCh8High:
        case kCh9High:
        case kCh10High:
        case kCh11High:
        case kCh12High:
        case kCh13High:
        case kCh14High:
        case kCh15High:
        case kCh16High:
            sprintf (label, "Ch%dHigh", (index - kCh1High) / 4 + 1);
            break;
        default:
            break;
    }
    dbg ("getParameterName(" << index << "," << label << ")");
}

//-----------------------------------------------------------------------------------------
void MidiChordSplit::getParameterDisplay (VstInt32 index, char* text)
{
    switch (index)
    {
        case kPower:
            if (param[index] < 0.5)
                strcpy (text, "Off (Bypass)");
            else
                strcpy (text, "On");
            break;
        case kInCh:
            if (FLOAT_TO_CHANNEL (param[index]) == -1)
                strcpy (text, "Any");
            else
                sprintf (text, "%d", FLOAT_TO_CHANNEL (param[index]) + 1);
            break;
        case kSteal:
            if (stealmode == oldest)
                strcpy (text, "Steal Oldest");
            else if (stealmode == newest)
                strcpy (text, "Steal Last");
            else if (stealmode == quietest)
                strcpy (text, "Steal Quietest");
            else if (stealmode == lowest)
                strcpy (text, "High");
            else if (stealmode == highest)
                strcpy (text, "Low");
            else if (stealmode == centered)
                strcpy (text, "Centered");
            else if (stealmode == random)
                strcpy (text, "Random");
            break;
        case kCh1Power:
        case kCh2Power:
        case kCh3Power:
        case kCh4Power:
        case kCh5Power:
        case kCh6Power:
        case kCh7Power:
        case kCh8Power:
        case kCh9Power:
        case kCh10Power:
        case kCh11Power:
        case kCh12Power:
        case kCh13Power:
        case kCh14Power:
        case kCh15Power:
        case kCh16Power:
            if (param[index] < 0.5)
                strcpy (text, "Off");
            else
                strcpy (text, "On");
            break;
        case kCh1Poly:
        case kCh2Poly:
        case kCh3Poly:
        case kCh4Poly:
        case kCh5Poly:
        case kCh6Poly:
        case kCh7Poly:
        case kCh8Poly:
        case kCh9Poly:
        case kCh10Poly:
        case kCh11Poly:
        case kCh12Poly:
        case kCh13Poly:
        case kCh14Poly:
        case kCh15Poly:
        case kCh16Poly:
            int p;
            p = roundToInt (((float) maxPoly - 1) * param[index]) + 1;
            if (p == 1)
                sprintf (text, "%d Note", p);
            else
                sprintf (text, "%d Notes", p);
            break;
        case kCh1Low:
        case kCh2Low:
        case kCh3Low:
        case kCh4Low:
        case kCh5Low:
        case kCh6Low:
        case kCh7Low:
        case kCh8Low:
        case kCh9Low:
        case kCh10Low:
        case kCh11Low:
        case kCh12Low:
        case kCh13Low:
        case kCh14Low:
        case kCh15Low:
        case kCh16Low:
        case kCh1High:
        case kCh2High:
        case kCh3High:
        case kCh4High:
        case kCh5High:
        case kCh6High:
        case kCh7High:
        case kCh8High:
        case kCh9High:
        case kCh10High:
        case kCh11High:
        case kCh12High:
        case kCh13High:
        case kCh14High:
        case kCh15High:
        case kCh16High:
            sprintf (text, "%s (%d)", getNoteName (FLOAT_TO_MIDI (param[index]), bottomOctave), FLOAT_TO_MIDI (param[index]));
            break;
        default:
            break;
    }
    dbg ("getParameterDisplay(" << index << "," << text << ")");
}

//-----------------------------------------------------------------------------------------
bool MidiChordSplit::init (void)
{
    dbg ("init()");
    srand ((unsigned int) time (NULL));

    lastoutch = 0;
    oldness   = 0;
    //queued=0;
    sustain    = false;
    playing    = false;
    wasplaying = false;
    memset (ntime, -1, sizeof (ntime));
    memset (held, 0, sizeof (held));
    memset (sounding, 0, sizeof (sounding));
    centerNote = sorter.centerNote = 60;
    for (int ch = 0; ch < MIDI_MAX_CHANNELS; ch++)
    {
        poly[ch]     = roundToInt (((float) maxPoly - 1) * param[kCh1Poly + ch * 4]) + 1;
        lastNote[ch] = FLOAT_TO_MIDI (param[kCh1Low + ch * 4]) + FLOAT_TO_MIDI ((param[kCh1High + ch * 4] - param[kCh1Low + ch * 4]) / 2.f);
        voices[ch]   = 0;
        for (int i = 0; i < maxPoly; i++)
        {
            Voice[ch][i].reset();
        }
    }
    for (int q = 0; q < queueSize; q++)
    {
        Queue[q].reset();
    }

    return PizMidi::init();
}

void MidiChordSplit::preProcess (void)
{
    // preparing Proccess
    VstTimeInfo* timeInfo = NULL;
    timeInfo              = getTimeInfo (0xffff); //ALL

    playing = false;
    if (timeInfo)
    {
        if (kVstTransportPlaying & timeInfo->flags)
        {
            playing = true;
        }
        else
        {
            playing = false;
        }
    }
    else
        playing = true;

    _cleanMidiOutBuffers();
}

VstInt32 MidiChordSplit::processEvents (VstEvents* ev)
{
    VstEvents* evts = (VstEvents*) ev;

    for (int i = 0; i < evts->numEvents; i++)
    {
        if ((evts->events[i])->type != kVstMidiType)
            continue;
        VstMidiEvent* event = (VstMidiEvent*) evts->events[i];
        _midiEventsIn[0].push_back (*event);
    }

    //sort according to note priority
    std::sort (_midiEventsIn[0].begin(), _midiEventsIn[0].end(), sorter);

    return true;
}

void MidiChordSplit::processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    const char listenchannel = FLOAT_TO_CHANNEL (param[kInCh]);

    int newpoly[16] = { roundToInt (((float) maxPoly - 1) * param[kCh1Poly]) + 1,
                        roundToInt (((float) maxPoly - 1) * param[kCh2Poly]) + 1,
                        roundToInt (((float) maxPoly - 1) * param[kCh3Poly]) + 1,
                        roundToInt (((float) maxPoly - 1) * param[kCh4Poly]) + 1,
                        roundToInt (((float) maxPoly - 1) * param[kCh5Poly]) + 1,
                        roundToInt (((float) maxPoly - 1) * param[kCh6Poly]) + 1,
                        roundToInt (((float) maxPoly - 1) * param[kCh7Poly]) + 1,
                        roundToInt (((float) maxPoly - 1) * param[kCh8Poly]) + 1,
                        roundToInt (((float) maxPoly - 1) * param[kCh9Poly]) + 1,
                        roundToInt (((float) maxPoly - 1) * param[kCh10Poly]) + 1,
                        roundToInt (((float) maxPoly - 1) * param[kCh11Poly]) + 1,
                        roundToInt (((float) maxPoly - 1) * param[kCh12Poly]) + 1,
                        roundToInt (((float) maxPoly - 1) * param[kCh13Poly]) + 1,
                        roundToInt (((float) maxPoly - 1) * param[kCh14Poly]) + 1,
                        roundToInt (((float) maxPoly - 1) * param[kCh15Poly]) + 1,
                        roundToInt (((float) maxPoly - 1) * param[kCh16Poly]) + 1 };

    int lownote[16] = { FLOAT_TO_MIDI (param[kCh1Low]),
                        FLOAT_TO_MIDI (param[kCh2Low]),
                        FLOAT_TO_MIDI (param[kCh3Low]),
                        FLOAT_TO_MIDI (param[kCh4Low]),
                        FLOAT_TO_MIDI (param[kCh5Low]),
                        FLOAT_TO_MIDI (param[kCh6Low]),
                        FLOAT_TO_MIDI (param[kCh7Low]),
                        FLOAT_TO_MIDI (param[kCh8Low]),
                        FLOAT_TO_MIDI (param[kCh9Low]),
                        FLOAT_TO_MIDI (param[kCh10Low]),
                        FLOAT_TO_MIDI (param[kCh11Low]),
                        FLOAT_TO_MIDI (param[kCh12Low]),
                        FLOAT_TO_MIDI (param[kCh13Low]),
                        FLOAT_TO_MIDI (param[kCh14Low]),
                        FLOAT_TO_MIDI (param[kCh15Low]),
                        FLOAT_TO_MIDI (param[kCh16Low]) };

    int highnote[16] = { FLOAT_TO_MIDI (param[kCh1High]),
                         FLOAT_TO_MIDI (param[kCh2High]),
                         FLOAT_TO_MIDI (param[kCh3High]),
                         FLOAT_TO_MIDI (param[kCh4High]),
                         FLOAT_TO_MIDI (param[kCh5High]),
                         FLOAT_TO_MIDI (param[kCh6High]),
                         FLOAT_TO_MIDI (param[kCh7High]),
                         FLOAT_TO_MIDI (param[kCh8High]),
                         FLOAT_TO_MIDI (param[kCh9High]),
                         FLOAT_TO_MIDI (param[kCh10High]),
                         FLOAT_TO_MIDI (param[kCh11High]),
                         FLOAT_TO_MIDI (param[kCh12High]),
                         FLOAT_TO_MIDI (param[kCh13High]),
                         FLOAT_TO_MIDI (param[kCh14High]),
                         FLOAT_TO_MIDI (param[kCh15High]),
                         FLOAT_TO_MIDI (param[kCh16High]) };

    int centerOfRange[16] = { lownote[0] + (highnote[0] - lownote[0]) / 2,
                              lownote[1] + (highnote[1] - lownote[1]) / 2,
                              lownote[2] + (highnote[2] - lownote[2]) / 2,
                              lownote[3] + (highnote[3] - lownote[3]) / 2,
                              lownote[4] + (highnote[4] - lownote[4]) / 2,
                              lownote[5] + (highnote[5] - lownote[5]) / 2,
                              lownote[6] + (highnote[6] - lownote[6]) / 2,
                              lownote[7] + (highnote[7] - lownote[7]) / 2,
                              lownote[8] + (highnote[8] - lownote[8]) / 2,
                              lownote[9] + (highnote[9] - lownote[9]) / 2,
                              lownote[10] + (highnote[10] - lownote[10]) / 2,
                              lownote[11] + (highnote[11] - lownote[11]) / 2,
                              lownote[12] + (highnote[12] - lownote[12]) / 2,
                              lownote[13] + (highnote[13] - lownote[13]) / 2,
                              lownote[14] + (highnote[14] - lownote[14]) / 2,
                              lownote[15] + (highnote[15] - lownote[15]) / 2 };

    bool useChan[16] = { param[kCh1Power] >= 0.5f,
                         param[kCh2Power] >= 0.5f,
                         param[kCh3Power] >= 0.5f,
                         param[kCh4Power] >= 0.5f,
                         param[kCh5Power] >= 0.5f,
                         param[kCh6Power] >= 0.5f,
                         param[kCh7Power] >= 0.5f,
                         param[kCh8Power] >= 0.5f,
                         param[kCh9Power] >= 0.5f,
                         param[kCh10Power] >= 0.5f,
                         param[kCh11Power] >= 0.5f,
                         param[kCh12Power] >= 0.5f,
                         param[kCh13Power] >= 0.5f,
                         param[kCh14Power] >= 0.5f,
                         param[kCh15Power] >= 0.5f,
                         param[kCh16Power] >= 0.5f };

    //int octave[16]=
    //{ 12*roundToInt(param[kCh1Octave]*8.f)-4,
    //  12*roundToInt(param[kCh2Octave]*8.f)-4,
    //  12*roundToInt(param[kCh3Octave]*8.f)-4,
    //  12*roundToInt(param[kCh4Octave]*8.f)-4,
    //  12*roundToInt(param[kCh5Octave]*8.f)-4,
    //  12*roundToInt(param[kCh6Octave]*8.f)-4,
    //  12*roundToInt(param[kCh7Octave]*8.f)-4,
    //  12*roundToInt(param[kCh8Octave]*8.f)-4,
    //  12*roundToInt(param[kCh9Octave]*8.f)-4,
    //  12*roundToInt(param[kCh10Octave]*8.f)-4,
    //  12*roundToInt(param[kCh11Octave]*8.f)-4,
    //  12*roundToInt(param[kCh12Octave]*8.f)-4,
    //  12*roundToInt(param[kCh13Octave]*8.f)-4,
    //  12*roundToInt(param[kCh14Octave]*8.f)-4,
    //  12*roundToInt(param[kCh15Octave]*8.f)-4,
    //  12*roundToInt(param[kCh16Octave]*8.f)-4, };

    unsigned int totalPoly = 0;
    for (int i = 0; i < 16; i++)
        if (useChan[i])
            totalPoly += newpoly[i];
    memcpy (poly, newpoly, sizeof (poly));
    bool retrig = false; //param[kRetrig]>=0.5f;
    //bool mono=lowch==highch && poly==1;
    bool usesustain = true; //param[kSustain]>=0.5f;

    if (inputs[0].size())
    {
        std::vector<int> heldNotesInBlock;
        bool held2[16][128];
        memcpy (held2, held, sizeof (held2));
        // re-pre-process incoming events
        for (unsigned int i = 0; i < inputs[0].size(); i++)
        {
            VstMidiEvent m = inputs[0][i];
            int channel    = m.midiData[0] & 0x0f;
            int note       = m.midiData[1] & 0x7f;
            if (isNoteOn (m))
                held2[channel][note] = true;
            else if (isNoteOff (m))
                held2[channel][note] = false;
        }
        //find center note
        int top    = -1;
        int bottom = 128;
        for (int n = 0; n < 128; n++)
        {
            for (int ch = 0; ch < 16; ch++)
            {
                if (held2[ch][n])
                {
                    heldNotesInBlock.push_back (n);
                    if (n < bottom)
                        bottom = n;
                    if (n > top)
                        top = n;
                }
            }
        }
        if (heldNotesInBlock.size() > 1)
        {
            //int c = (int)((float)heldNotes.size()/2.f);
            int total = 0;
            for (unsigned int i = 0; i < heldNotesInBlock.size(); i++)
            {
                total += heldNotesInBlock[i];
            }
            centerNote = roundToInt ((float) total / (float) heldNotesInBlock.size());
            //if (heldNotes.size()%2==0)
            //	centerNote = roundToInt(0.5f * (float)(heldNotes[c]+heldNotes[c+1]));
            //else
            //	centerNote = heldNotes[c];
            sorter.centerNote = centerNote;
        }
        std::sort (inputs[0].begin(), inputs[0].end(), sorter);

        //const int s = heldNotes.size();
        //int* bestChannel = new int[s];
        //if (heldNotes.size()>=totalPoly) {
        //	unsigned int h=0;
        //	while (h<heldNotes.size())
        //	{
        //		int tempd1 = 999;
        //		int tempd2 = 999;
        //		int tempi1 = 999;
        //		int tempchi = -1;
        //		int tempchd = -1;
        //		for (int ch=0;ch<16;ch++)
        //		{
        //			if (lownote[ch]<=heldNotes[h] && highnote[ch]>=heldNotes[h] && useChan[ch]) {
        //				bestChannel[h] = ch;
        //				h++;
        //			}
        //		}
        //		if (h<heldNotes.size()-1)
        //		{
        //			//not done
        //		}
        //	}
        //}
        //delete[] bestChannel;

        memset (ntime, -1, sizeof (ntime));
        for (unsigned int i = 0; i < inputs[0].size(); i++)
        {
            //copying event "i" from input (with all its fields)
            VstMidiEvent tomod = inputs[0][i];

            int status  = tomod.midiData[0] & 0xf0; // scraping  channel
            int channel = tomod.midiData[0] & 0x0f; // isolating channel (0-15)
            int data1   = tomod.midiData[1] & 0x7f;
            int data2   = tomod.midiData[2] & 0x7f;

            bool discard = false;

            if (isNoteOn (tomod))
            {
                held[channel][data1]              = true;
                sorter.onvelocity[channel][data1] = data2;
                findCenterNote();
                if ((channel == listenchannel || listenchannel == -1) && param[kPower] >= 0.5f)
                {
                    ntime[data1][channel] = tomod.deltaFrames;
                    int outch             = -1;
                    //int melodych = -1;
                    int stealch = -1;
                    int outn    = voices[outch] - 1;
                    v stealme;
                    //oldness: older==smaller number
                    if (stealmode == oldest)
                        stealme.oldness = 0xFFFFFFFF;
                    else
                        stealme.oldness = 0;
                    stealme.inch = outch;
                    if (stealmode == highest)
                        stealme.note = -1;
                    else if (stealmode == centered)
                        stealme.note = centerNote;
                    else
                        stealme.note = 128;
                    stealme.vel = 128;
                    stealme.on  = true;

                    bool stop    = false;
                    bool nosteal = false;

                    int d[16];        //interval between incoming note and center of each channel range
                    int interval[16]; //interval between incoming note and last note played on each channel
                    int tempd1 = 999;
                    int tempi1 = 999;
                    for (int ch = 0; ch < 16; ch++)
                    {
                        d[ch]        = abs (centerOfRange[ch] - data1);
                        interval[ch] = abs (lastNote[ch] - data1);
                        if (lownote[ch] <= data1 && highnote[ch] >= data1 && useChan[ch])
                        {
                            if (d[ch] < tempd1)
                            {
                                tempd1  = d[ch];
                                stealch = ch;
                            }
                            if (interval[ch] < tempi1 && voices[ch] < poly[ch])
                            {
                                tempi1 = interval[ch];
                                outch  = ch;
                            }
                        }
                    }
                    if (stealch == -1)
                    {
                        //no channels have this note in range
                        discard = true;
                    }
                    else
                    {
                        if (heldNotesInBlock.size() >= totalPoly)
                        {
                            for (int ch = 0; ch < 16; ch++)
                            {
                                if (lownote[ch] <= data1 && highnote[ch] >= data1
                                    && useChan[ch] && voices[ch] < poly[ch])
                                {
                                    outch = ch;
                                    break;
                                }
                            }
                        }
                        if (outch != -1)
                        {
                            for (int n = 0; n < poly[outch]; n++)
                            {
                                if (stop)
                                    break;
                                if (Voice[outch][n].on == false)
                                {
                                    outn                   = n;
                                    stop                   = true;
                                    sounding[outch][data1] = true;
                                    lastNote[outch]        = data1;
                                }
                            }
                        }
                        else
                        {
                            //all channels full, need voice stealing
                            int ch = stealch;
                            for (int n = 0; n < poly[ch]; n++)
                            {
                                if (Voice[ch][n].on)
                                {
                                    //determine which voice to steal
                                    switch (stealmode)
                                    {
                                        case oldest:
                                            if (Voice[ch][n].oldness < stealme.oldness)
                                            {
                                                //find oldest note for stealing
                                                stealme.oldness = Voice[ch][n].oldness;
                                                stealme.note    = Voice[ch][n].note;
                                                outch           = ch;
                                                outn            = n;
                                            }
                                            break;
                                        case newest:
                                            if (Voice[ch][n].oldness >= stealme.oldness)
                                            {
                                                //find oldest note for stealing
                                                stealme.oldness = Voice[ch][n].oldness;
                                                stealme.note    = Voice[ch][n].note;
                                                outch           = ch;
                                                outn            = n;
                                            }
                                            break;
                                        case quietest:
                                            if (Voice[ch][n].vel < stealme.vel)
                                            {
                                                //find quietest note for stealing
                                                stealme.vel  = Voice[ch][n].vel;
                                                stealme.note = Voice[ch][n].note;
                                                outch        = ch;
                                                outn         = n;
                                            }
                                            break;
                                        case lowest:
                                            if (Voice[ch][n].note < stealme.note)
                                            {
                                                //find lowest note for stealing
                                                stealme.note = Voice[ch][n].note;
                                                outch        = ch;
                                                outn         = n;
                                            }
                                            break;
                                        case highest:
                                            if (Voice[ch][n].note > stealme.note)
                                            {
                                                //find highest note for stealing
                                                stealme.note = Voice[ch][n].note;
                                                outch        = ch;
                                                outn         = n;
                                            }
                                            break;
                                        case centered:
                                            //steal note farthest from center
                                            if (abs (Voice[ch][n].note - centerNote) > abs (stealme.note - centerNote))
                                            {
                                                stealme.note = Voice[ch][n].note;
                                                outch        = ch;
                                                outn         = n;
                                            }
                                            break;
                                        case random:
                                            //steal random voice
                                            if (rand() < RAND_MAX / 2)
                                            {
                                                stealme.note = Voice[ch][n].note;
                                                outch        = ch;
                                                outn         = n;
                                                nosteal      = false;
                                            }
                                            else
                                                nosteal = true;

                                    } //switch(stealmode)
                                }     //if voice on
                            }         //for n
                            //if (retrig) {
                            if (stealmode == lowest && stealme.note > data1)
                                nosteal = true;
                            else if (stealmode == highest && stealme.note < data1)
                                nosteal = true;
                            else if (stealmode == centered && abs (stealme.note - centerNote) < abs (data1 - centerNote))
                                nosteal = true;
                            //else if (ntime[stealme.note][stealme.chan]==tomod.deltaFrames)
                            //	nosteal=true;
                            if (outn == -1)
                                break;
                            stealme = copyVoice (Voice[outch][outn]);
                            dbg ("stealme n=" << stealme.note << " v=" << stealme.vel);
                            //}
                            if (! nosteal)
                            {
                                dbg ("!nosteal");
                                //kill the stolen voice and put it in the queue for restarting
                                if (1 /*queued>=0 && queued<queueSize*/)
                                {
                                    //todo: apply voice stealing to queue
                                    //now it just ignores the (queueSize+1)th note
                                    bool gotq = false;
                                    for (int q = 0; q < queueSize; q++)
                                    {
                                        if (gotq)
                                            break;
                                        if (Queue[q].on && Queue[q].note == stealme.note && Queue[q].chan == outch)
                                        {
                                            dbg ("same note already queued q=" << q);
                                            Queue[q]           = copyVoice (stealme);
                                            Queue[q].on        = true;
                                            Queue[q].sustained = false;
                                            gotq               = true;
                                        }
#ifdef _DEBUG
                                        else
                                        {
                                            if (Queue[q].on || Queue[q].note)
                                                dbg ("no match q[" << q << "] on=" << Queue[q].on << " n=" << Queue[q].note << " ch=" << (int) Queue[q].chan);
                                        }
#endif
                                    }
                                    for (int q = 0; q < queueSize; q++)
                                    {
                                        if (gotq)
                                            break;
                                        if (! Queue[q].on)
                                        {
                                            dbg ("found empty queue slot q=" << q);
                                            Queue[q]           = copyVoice (stealme);
                                            Queue[q].on        = true;
                                            Queue[q].sustained = false;
                                            gotq               = true;
                                            //++queued;
                                        }
                                    }
                                    dbg ("voice [" << (int) outch << "][" << (int) outn << "]." << Voice[outch][outn].note << " stolen, queued");
                                }
                                else
                                    discard = true;
                                VstMidiEvent kill;
                                kill.midiData[0] = MIDI_NOTEOFF + outch;
                                kill.midiData[1] = (char) stealme.note;
                                kill.midiData[2] = 0;
                                kill.deltaFrames = tomod.deltaFrames;
                                kill.detune      = 0;
                                outputs[0].push_back (kill);
                                sounding[outch][stealme.note] = false;
                                --voices[outch];
                                Voice[outch][outn].on = false;
                                if (! sustain)
                                    Voice[outch][outn].sustained = false;
                                dbg ("voice [" << (int) outch << "][" << (int) outn << "]." << Voice[outch][outn].note << " killed, " << voices[outch] << " voices");
                            }
                        } //voices[channel]>=poly
                        if (nosteal)
                        {
                            //queue the note instead
                            if (1 /*queued>=0 && queued<queueSize*/)
                            {
                                //todo: apply voice stealing to queue
                                //now it just ignores the (queueSize+1)th note
                                bool gotq = false;
                                for (int q = 0; q < queueSize; q++)
                                {
                                    if (gotq)
                                        break;
                                    if (Queue[q].on && Queue[q].note == data1 && Queue[q].chan == outch)
                                    {
                                        Queue[q].oldness   = ++oldness;
                                        Queue[q].inch      = channel;
                                        Queue[q].chan      = outch;
                                        Queue[q].note      = data1;
                                        Queue[q].vel       = data2;
                                        Queue[q].on        = true;
                                        Queue[q].sustained = false;
                                        gotq               = true;
                                    }
                                }
                                for (int q = 0; q < queueSize; q++)
                                {
                                    if (gotq)
                                        break;
                                    if (! Queue[q].on)
                                    {
                                        Queue[q].oldness   = ++oldness;
                                        Queue[q].inch      = channel;
                                        Queue[q].chan      = outch;
                                        Queue[q].note      = data1;
                                        Queue[q].vel       = data2;
                                        Queue[q].on        = true;
                                        Queue[q].sustained = false;
                                        gotq               = true;
                                        //++queued;
                                        dbg ("voice [" << (int) outch << "][" << (int) outn << "]." << Voice[outch][outn].note << " not used, queued");
                                    }
                                }
                            }
                            discard = true;
                            dbg (voices[outch] << " voices");
                        }
                        else
                        {
                            Voice[outch][outn].oldness   = ++oldness;
                            Voice[outch][outn].inch      = channel;
                            Voice[outch][outn].chan      = outch;
                            Voice[outch][outn].note      = data1;
                            Voice[outch][outn].vel       = data2;
                            Voice[outch][outn].on        = true;
                            Voice[outch][outn].sustained = false;
                            tomod.midiData[0]            = outch | MIDI_NOTEON;
                            sorter.priorityNote          = data1;
                            sounding[outch][data1]       = true;
                            lastNote[outch]              = data1;
                            ++voices[outch];
                            lastoutch = outch;

                            dbg ("voice [" << (int) outch << "][" << (int) outn << "]." << Voice[outch][outn].note << " starting, " << voices[outch] << " voices");

                            bool gotq = false;
                            for (int q = 0; q < queueSize; q++)
                            {
                                if (gotq)
                                    break;
                                if (Queue[q].on && Queue[q].note == data1 && Queue[q].chan == outch)
                                {
                                    dbg ("playing note already queued q=" << q);
                                    Queue[q].on        = false;
                                    Queue[q].sustained = false;
                                    gotq               = true;
                                    //dbg(queued<<" queued");
                                }
#ifdef _DEBUG
                                else
                                {
                                    if (Queue[q].on || Queue[q].note)
                                        dbg ("no match q[" << q << "] on=" << Queue[q].on << " n=" << Queue[q].note << " ch=" << (int) Queue[q].chan);
                                }
#endif
                            }
                        }
                    }
                }
            } //if note-on
            else if (! isNoteOff (tomod))
            {
                if (status == MIDI_CONTROLCHANGE && data1 == MIDI_SUSTAIN && usesustain)
                {
                    discard = true;
                    if (data2 > 63)
                    {
#ifdef _DEBUG
                        if (! sustain)
                            dbg ("sustain on");
#endif
                        sustain = true;
                    }
                    else
                    {
                        sustain = false;
                        for (int q = 0; q < queueSize; q++)
                        {
                            if (Queue[q].on && Queue[q].sustained)
                            {
                                //--queued;
                                Queue[q].sustained = false;
                                Queue[q].on        = false;
                                dbg ("sustain off, q[" << q << "]." << Queue[q].note << " off");
                            }
                        }
                        for (int ch = 0; ch < MIDI_MAX_CHANNELS; ch++)
                        {
                            for (int n = 0; n < maxPoly; n++)
                            {
                                if (Voice[ch][n].on && Voice[ch][n].sustained)
                                {
                                    //end all sustained notes
                                    VstMidiEvent off;
                                    off.midiData[0] = MIDI_NOTEOFF + ch;
                                    off.midiData[1] = (char) Voice[ch][n].note;
                                    off.midiData[2] = 0;
                                    off.deltaFrames = tomod.deltaFrames;
                                    off.detune      = 0;
                                    outputs[0].push_back (off);
                                    sounding[ch][Voice[ch][n].note] = false;
                                    --voices[ch];
                                    Voice[ch][n].on        = false;
                                    Voice[ch][n].sustained = false;
                                    dbg ("sustain off, v[" << ch << "][" << n << "]." << Voice[ch][n].note << " off, " << voices[ch] << " voices");
                                }
                            }
                        }
                    }
                }
                else if (status == MIDI_CONTROLCHANGE && data1 == MIDI_ALL_NOTES_OFF)
                {
                    //ignore, see below
                }
                else
                {
                    //send other midi to all used channels
                    //this part crashes reaper on startup (when it sends pitchbend 0 on all channels)
                    if (! (status == MIDI_PITCHBEND && data1 == 0 && data2 == 64 && ! playing && ! wasplaying))
                    {
                        for (int ch = 0; ch < 16; ch++)
                        {
                            VstMidiEvent m;
                            m.midiData[0] = status | ch;
                            m.midiData[1] = data1;
                            m.midiData[2] = data2;
                            m.deltaFrames = tomod.deltaFrames;
                            m.detune      = 0;
                            outputs[0].push_back (m);
                            discard = true; //to avoid sending twice on input channel
                        }
                    }
                }
            }
            else if (isNoteOff (tomod))
            { //process every note-off to avoid hanging notes
                held[channel][data1]              = false;
                sorter.onvelocity[channel][data1] = 0;
                findCenterNote();
                dbg ("noteoff input n" << (int) data1 << " ch" << (int) channel);
                discard            = true;
                bool done          = false;
                bool queuednoteoff = false;
                //see if the note is in the queue
                for (int q = 0; q < queueSize; q++)
                {
                    if (Queue[q].inch == channel && Queue[q].note == data1 && Queue[q].on)
                    {
                        //ending a silent queued note
                        if (sustain)
                        {
                            Queue[q].sustained = true;
                            dbg ("silent queued note q[" << q << "]." << Queue[q].note << " sustained");
                        }
                        else
                        {
                            Queue[q].on        = false;
                            Queue[q].sustained = false;
                            //--queued;
                            dbg ("queued note q[" << q << "]." << Queue[q].note << " note-off");
                        }
                        queuednoteoff = true;
                    }
                    if (queuednoteoff)
                        break;
                }
                //see if the note is playing
                if (! queuednoteoff)
                {
                    for (int ch = 0; ch < 16; ch++)
                    {
                        if (done)
                            break;
                        for (int n = 0; n < maxPoly; n++)
                        {
                            if (done)
                                break;
                            if (Voice[ch][n].inch == channel && Voice[ch][n].note == data1 && Voice[ch][n].on)
                            {
                                //got match
                                done = true;
                                if (sustain)
                                {
                                    Voice[ch][n].sustained = true;
                                    dbg ("voice [" << ch << "][" << n << "]." << Voice[ch][n].note << " sustained, " << voices[ch] << " voices");
                                }
                                else
                                {
                                    VstMidiEvent off = tomod;
                                    off.midiData[0]  = MIDI_NOTEOFF | ch;
                                    off.midiData[1]  = (char) Voice[ch][n].note;
                                    off.midiData[2]  = 0;
                                    off.deltaFrames  = tomod.deltaFrames;
                                    off.detune       = 0;
                                    outputs[0].push_back (off);
                                    sounding[ch][Voice[ch][n].note] = false;
                                    --voices[ch];
                                    //if (voices[outch]<0) voices[outch]=0;
                                    Voice[ch][n].on        = false;
                                    Voice[ch][n].sustained = false;
                                    dbg ("voice [" << ch << "][" << n << "]." << Voice[ch][n].note << " stopped, " << voices[ch] << " voices");
                                    //this voice is now free, so look for a queued note
                                    if (1 /*queued>0*/)
                                    {
                                        v useme;
                                        //oldness: older==smaller
                                        if (stealmode == oldest)
                                            useme.oldness = 0;
                                        else
                                            useme.oldness = 0xFFFFFFFF;
                                        useme.inch = 0;
                                        if (stealmode == highest)
                                            useme.note = 128;
                                        else
                                            useme.note = -1;
                                        useme.vel = 1;
                                        useme.on  = true;
                                        int useq  = 0;
                                        bool stop = false;
                                        for (int q = 0; q < queueSize; q++)
                                        {
                                            if (Queue[q].on && ! stop)
                                            {
                                                switch (stealmode)
                                                {
                                                    case oldest: //steal oldest==replace with newest???
                                                        if (Queue[q].oldness >= useme.oldness)
                                                        {
                                                            //find newest queued note
                                                            useme = copyVoice (Queue[q]);
                                                            useq  = q;
                                                        }
                                                        break;
                                                    case newest:
                                                        if (Queue[q].oldness < useme.oldness)
                                                        {
                                                            //find oldest queued note
                                                            useme = copyVoice (Queue[q]);
                                                            useq  = q;
                                                        }
                                                        break;
                                                    case quietest:
                                                        if (Queue[q].vel >= useme.vel)
                                                        {
                                                            //find loudest queued note
                                                            useme = copyVoice (Queue[q]);
                                                            useq  = q;
                                                        }
                                                        break;
                                                    case lowest:
                                                        if (Queue[q].note > useme.note)
                                                        {
                                                            //find highest queued note
                                                            useme = copyVoice (Queue[q]);
                                                            useq  = q;
                                                        }
                                                        break;
                                                    case highest:
                                                        if (Queue[q].note < useme.note)
                                                        {
                                                            //find lowest queued note
                                                            useme = copyVoice (Queue[q]);
                                                            useq  = q;
                                                        }
                                                        break;
                                                    case centered:
                                                        if (abs (Queue[q].note - centerNote) < abs (useme.note - centerNote))
                                                        {
                                                            //find lowest queued note
                                                            useme = copyVoice (Queue[q]);
                                                            useq  = q;
                                                        }
                                                        break;
                                                    case random:
                                                        if (rand() < RAND_MAX / 2)
                                                        {
                                                            //get random note
                                                            useme = copyVoice (Queue[q]);
                                                            useq  = q;
                                                        }
                                                        else
                                                        {
                                                            //in case no voice is found
                                                            if (useme.note == -1)
                                                            {
                                                                useme = copyVoice (Queue[0]);
                                                                useq  = q;
                                                            }
                                                        }
                                                        break;
                                                } // /switch
                                            }     //if Queue[q].on
                                        }         // /for 0<=q<16
                                        //send the queued note
                                        if (useme.note != -1)
                                        {
                                            //if (voices[useme.chan]>=maxPoly) {
                                            useme.chan = ch;
                                            //}
                                            if (retrig)
                                            {
                                                VstMidiEvent queue = tomod;
                                                queue.midiData[0]  = MIDI_NOTEON + useme.chan;
                                                queue.midiData[1]  = (unsigned char) useme.note;
                                                queue.midiData[2]  = (unsigned char) useme.vel;
                                                queue.deltaFrames  = tomod.deltaFrames;
                                                queue.detune       = 0;
                                                //--queued;
                                                Queue[useq].on        = false;
                                                Queue[useq].sustained = false;
                                                outputs[0].push_back (queue);
                                                dbg ("queued note q[" << useq << "]." << Queue[useq].note << " unqueued");
                                                //update the voice
                                                Voice[ch][n]                     = copyVoice (useme);
                                                Voice[ch][n].sustained           = false;
                                                sounding[useme.chan][useme.note] = true;
                                                lastNote[useme.chan]             = useme.note;
                                                ++voices[ch];
                                                dbg ("voice [" << ch << "][" << n << "]." << Voice[ch][n].note << " retriggered, " << voices[ch] << " voices");
                                            }
                                        }
                                        else
                                        {
                                            dbg ("queue empty");
                                        }
                                    } // /if queued>0
                                }
                            } //got match
                        }     // for n
                    }         //for ch
                }             //if(!done)
                if (done)
                    discard = true;
                else if (! queuednoteoff)
                {
                    discard = false;
                    dbg ("no match????, v=" << voices[channel] << " s=" << (int) sustain << " n=" << (int) data1);
                }
            } //if note-off
            if (status == MIDI_CONTROLCHANGE && data1 == MIDI_ALL_NOTES_OFF)
            {
                dbg ("all notes off ch" << (int) channel);
                memset (held, 0, sizeof (held));
                memset (sounding, 0, sizeof (sounding));
                //queued=0;
                voices[channel] = 0;
                sustain         = false;
                for (int i = 0; i < maxPoly; i++)
                {
                    Voice[channel][i].on        = false;
                    Voice[channel][i].sustained = false;
                }
                for (int q = 0; q < queueSize; q++)
                {
                    if (Queue[q].chan == channel)
                    {
                        Queue[q].on        = false;
                        Queue[q].sustained = false;
                    }
                }
            }
            if (! discard)
            {
                outputs[0].push_back (tomod);
            }
        }
    }

    if (wasplaying && ! playing)
    { //just stopped
        dbg ("stopped");
        //queued=0;
        sustain = false;
        //end artifically sustained notes
        for (int channel = 0; channel < MIDI_MAX_CHANNELS; channel++)
        {
            voices[channel] = 0;
            for (int i = 0; i < maxPoly; i++)
            {
                if (Voice[channel][i].sustained)
                {
                    VstMidiEvent kill;
                    kill.midiData[0] = MIDI_NOTEOFF + channel;
                    kill.midiData[1] = (char) Voice[channel][i].note;
                    kill.midiData[2] = 0;
                    kill.deltaFrames = 0;
                    kill.detune      = 0;
                    outputs[0].push_back (kill);
                    Voice[channel][i].on                      = false;
                    Voice[channel][i].sustained               = false;
                    sounding[channel][Voice[channel][i].note] = false;
                }
            }
        }
        for (int q = 0; q < queueSize; q++)
        {
            if (Queue[q].sustained)
            {
                //VstMidiEvent kill;
                //kill.midiData[0] = MIDI_NOTEOFF + Queue[q].chan;
                //kill.midiData[1] = (char)Queue[q].note;
                //kill.midiData[2] = 0;
                //kill.deltaFrames = 0;
                //kill.detune = 0;
                //outputs[0].push_back(kill);
                //sounding[Queue[q].chan][Queue[q].note]=false;
                Queue[q].on        = false;
                Queue[q].sustained = false;
            }
        }
    }
    wasplaying = playing;

    //clean 0-length notes from stream here?????????
}

v MidiChordSplit::copyVoice (v in)
{
    v out;
    out.oldness   = in.oldness;
    out.inch      = in.inch;
    out.chan      = in.chan;
    out.note      = in.note;
    out.vel       = in.vel;
    out.on        = in.on;
    out.sustained = in.sustained;
    return out;
}

void MidiChordSplit::findCenterNote()
{
    //find center note
    int top    = -1;
    int bottom = 128;
    std::vector<int> heldNoteNumbers;
    for (int n = 0; n < 128; n++)
    {
        for (int ch = 0; ch < 16; ch++)
        {
            if (held[ch][n])
            {
                heldNoteNumbers.push_back (n);
                if (n < bottom)
                    bottom = n;
                if (n > top)
                    top = n;
            }
        }
    }
    if (heldNoteNumbers.size() > 1)
    {
        //int c = (int)((float)heldNotes.size()/2.f);
        int total = 0;
        for (unsigned int i = 0; i < heldNoteNumbers.size(); i++)
        {
            total += heldNoteNumbers[i];
        }
        centerNote = roundToInt ((float) total / (float) heldNoteNumbers.size());
        //if (heldNotes.size()%2==0)
        //	centerNote = roundToInt(0.5f * (float)(heldNotes[c]+heldNotes[c+1]));
        //else
        //	centerNote = heldNotes[c];
        sorter.centerNote = centerNote;
    }
}