#include "midiNotesToCC.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiNotesToCC(audioMaster);
}

MidiNotesToCCProgram::MidiNotesToCCProgram()
{
    // default Program Values
    fNoteHi = 1.f;
    fNoteLo = 0.f;
    fVeloHi = 1.f;
    fVeloLo = 0.f;
    fChin   = CHANNEL_TO_FLOAT016(0);
    fNCC    = MIDI_TO_FLOAT2(74.1 + 1);
    fNCCoff = MIDI_TO_FLOAT2(0);
    fNCCHi  = 1.0;
    fNCCLo  = 0.0;
    fVCC    = MIDI_TO_FLOAT2(75.1 + 1);
    fVCCoff = MIDI_TO_FLOAT2(0);
    fVCCHi  = 1.0;
    fVCCLo  = 0.0;
    fChout  = 0.0;
    fThru   = 0.0;
    // default program name
    strcpy(name, "Default");
}

//-----------------------------------------------------------------------------
MidiNotesToCC::MidiNotesToCC(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams), programs(0)
{
    programs = new MidiNotesToCCProgram[numPrograms];
    fNoteHi  = 1.f;
    fNoteLo  = 0.f;
    fVeloHi  = 1.f;
    fVeloLo  = 0.f;
    fChin    = CHANNEL_TO_FLOAT016(0);
    fNCC     = MIDI_TO_FLOAT2(74.1 + 1);
    fNCCoff  = MIDI_TO_FLOAT2(0);
    fNCCHi   = 1.0;
    fNCCLo   = 0.0;
    fVCC     = MIDI_TO_FLOAT2(75.1 + 1);
    fVCCoff  = MIDI_TO_FLOAT2(0);
    fVCCHi   = 1.0;
    fVCCLo   = 0.0;
    fChout   = 0.0;
    fThru    = 0.0;

    if (programs)
    {
        CFxBank* defaultBank = new CFxBank(kNumPrograms, kNumParams);
        if (readDefaultBank(PLUG_NAME, defaultBank))
        {
            if ((VstInt32) defaultBank->GetFxID() == PLUG_IDENT)
            {
                for (int i = 0; i < kNumPrograms; i++)
                {
                    programs[i].fNoteHi = defaultBank->GetProgParm(i, 0);
                    programs[i].fNoteLo = defaultBank->GetProgParm(i, 1);
                    programs[i].fVeloHi = defaultBank->GetProgParm(i, 2);
                    programs[i].fVeloLo = defaultBank->GetProgParm(i, 3);
                    programs[i].fChin   = defaultBank->GetProgParm(i, 4);
                    programs[i].fNCC    = defaultBank->GetProgParm(i, 5);
                    programs[i].fNCCoff = defaultBank->GetProgParm(i, 6);
                    programs[i].fNCCHi  = defaultBank->GetProgParm(i, 7);
                    programs[i].fNCCLo  = defaultBank->GetProgParm(i, 8);
                    programs[i].fVCC    = defaultBank->GetProgParm(i, 9);
                    programs[i].fVCCoff = defaultBank->GetProgParm(i, 10);
                    programs[i].fVCCHi  = defaultBank->GetProgParm(i, 11);
                    programs[i].fVCCLo  = defaultBank->GetProgParm(i, 12);
                    programs[i].fChout  = defaultBank->GetProgParm(i, 13);
                    programs[i].fThru   = defaultBank->GetProgParm(i, 14);
                    strcpy(programs[i].name, defaultBank->GetProgramName(i));
                }
            }
        }
        else
        {
            // built-in programs
            for (int i = 0; i < kNumPrograms; i++)
            {
                sprintf(programs[i].name, "Program %d", i + 1);
            }
        }
        setProgram(0);
    }

    for (int n = 0; n < 128; n++)
    {
        for (int ch = 0; ch < 16; ch++)
        {
            notePlaying[n][ch] = false;
        }
    }
    init();
}

//-----------------------------------------------------------------------------------------
MidiNotesToCC::~MidiNotesToCC()
{
    if (programs)
    {
        delete[] programs;
    }
}

//------------------------------------------------------------------------
void MidiNotesToCC::setProgram(VstInt32 program)
{
    MidiNotesToCCProgram* ap = &programs[program];

    curProgram = program;
    setParameter(kNoteHi, ap->fNoteHi);
    setParameter(kNoteLo, ap->fNoteLo);
    setParameter(kVeloHi, ap->fVeloHi);
    setParameter(kVeloLo, ap->fVeloLo);
    setParameter(kChin, ap->fChin);
    setParameter(kNCC, ap->fNCC);
    setParameter(kNCCoff, ap->fNCCoff);
    setParameter(kNCCHi, ap->fNCCHi);
    setParameter(kNCCLo, ap->fNCCLo);
    setParameter(kVCC, ap->fVCC);
    setParameter(kVCCoff, ap->fVCCoff);
    setParameter(kVCCHi, ap->fVCCHi);
    setParameter(kVCCLo, ap->fVCCLo);
    setParameter(kChout, ap->fChout);
    setParameter(kThru, ap->fThru);
}

//------------------------------------------------------------------------
void MidiNotesToCC::setProgramName(char* name)
{
    vst_strncpy(programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiNotesToCC::getProgramName(char* name)
{
    strcpy(name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool MidiNotesToCC::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy(text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiNotesToCC::setParameter(VstInt32 index, float value)
{
    MidiNotesToCCProgram* ap = &programs[curProgram];

    switch (index)
    {
        case kNoteHi:
            if (value < fNoteLo)
            {
                setParameterAutomated(kNoteLo, value);
            }
            fNoteHi = ap->fNoteHi = value;
            break;
        case kNoteLo:
            if (value > fNoteHi)
            {
                setParameterAutomated(kNoteHi, value);
            }
            fNoteLo = ap->fNoteLo = value;
            break;
        case kVeloHi:
            if (value < fVeloLo)
            {
                setParameterAutomated(kVeloLo, value);
            }
            fVeloHi = ap->fVeloHi = value;
            break;
        case kVeloLo:
            if (value > fVeloHi)
            {
                setParameterAutomated(kVeloHi, value);
            }
            fVeloLo = ap->fVeloLo = value;
            break;
        case kChin:
            fChin = ap->fChin = value;
            break;
        case kNCC:
            fNCC = ap->fNCC = value;
            break;
        case kNCCoff:
            fNCCoff = ap->fNCCoff = value;
            break;
        case kNCCHi:
            fNCCHi = ap->fNCCHi = value;
            break;
        case kNCCLo:
            fNCCLo = ap->fNCCLo = value;
            break;
        case kVCC:
            fVCC = ap->fVCC = value;
            break;
        case kVCCoff:
            fVCCoff = ap->fVCCoff = value;
            break;
        case kVCCHi:
            fVCCHi = ap->fVCCHi = value;
            break;
        case kVCCLo:
            fVCCLo = ap->fVCCLo = value;
            break;
        case kChout:
            fChout = ap->fChout = value;
            break;
        case kThru:
            fThru = ap->fThru = value;
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
float MidiNotesToCC::getParameter(VstInt32 index)
{
    float v = 0;

    switch (index)
    {
        case kNoteHi:
            v = fNoteHi;
            break;
        case kNoteLo:
            v = fNoteLo;
            break;
        case kVeloHi:
            v = fVeloHi;
            break;
        case kVeloLo:
            v = fVeloLo;
            break;
        case kChin:
            v = fChin;
            break;
        case kNCC:
            v = fNCC;
            break;
        case kNCCoff:
            v = fNCCoff;
            break;
        case kNCCHi:
            v = fNCCHi;
            break;
        case kNCCLo:
            v = fNCCLo;
            break;
        case kVCC:
            v = fVCC;
            break;
        case kVCCoff:
            v = fVCCoff;
            break;
        case kVCCHi:
            v = fVCCHi;
            break;
        case kVCCLo:
            v = fVCCLo;
            break;
        case kChout:
            v = fChout;
            break;
        case kThru:
            v = fThru;
            break;
        default:
            break;
    }
    return v;
}

//-----------------------------------------------------------------------------------------
void MidiNotesToCC::getParameterName(VstInt32 index, char* label)
{
    switch (index)
    {
        case kNCC:
            strcpy(label, "Note to CC");
            break;
        case kNoteHi:
            strcpy(label, "High Note");
            break;
        case kNoteLo:
            strcpy(label, "Low Note");
            break;
        case kNCCoff:
            strcpy(label, "NCC Rest Value");
            break;
        case kNCCHi:
            strcpy(label, "High NCC Value");
            break;
        case kNCCLo:
            strcpy(label, "Low NCC Value");
            break;
        case kVCC:
            strcpy(label, "Velocity to CC");
            break;
        case kVeloHi:
            strcpy(label, "High Velocity");
            break;
        case kVeloLo:
            strcpy(label, "Low Velocity");
            break;
        case kVCCHi:
            strcpy(label, "High VCC Value");
            break;
        case kVCCLo:
            strcpy(label, "Low VCC Value");
            break;
        case kVCCoff:
            strcpy(label, "VCC Rest Value");
            break;
        case kChin:
            strcpy(label, "In Channel");
            break;
        case kChout:
            strcpy(label, "Out Channel");
            break;
        case kThru:
            strcpy(label, "Thru");
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiNotesToCC::getParameterDisplay(VstInt32 index, char* text)
{
    switch (index)
    {
        case kNoteHi:
            sprintf(text, "%s (%d)", getNoteName(FLOAT_TO_MIDI(fNoteHi), bottomOctave), FLOAT_TO_MIDI(fNoteHi));
            break;
        case kNoteLo:
            sprintf(text, "%s (%d)", getNoteName(FLOAT_TO_MIDI(fNoteLo), bottomOctave), FLOAT_TO_MIDI(fNoteLo));
            break;
        case kVeloHi:
            if (FLOAT_TO_MIDI(fVeloHi) == 0)
            {
                strcpy(text, "1");
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_MIDI(fVeloHi));
            }
            break;
        case kVeloLo:
            if (FLOAT_TO_MIDI(fVeloLo) == 0)
            {
                strcpy(text, "1");
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_MIDI(fVeloLo));
            }
            break;
        case kChin:
            if (FLOAT_TO_CHANNEL016(fChin) == 0)
            {
                strcpy(text, "Any");
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_CHANNEL016(fChin));
            }
            break;
        case kNCC:
            if (FLOAT_TO_MIDI2(fNCC) == 0)
            {
                strcpy(text, "Off");
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_MIDI2(fNCC) - 1);
            }
            break;
        case kNCCoff:
            if (FLOAT_TO_MIDI2(fNCCoff) == 0)
            {
                strcpy(text, "Off");
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_MIDI2(fNCCoff) - 1);
            }
            break;
        case kNCCHi:
            sprintf(text, "%d", FLOAT_TO_MIDI(fNCCHi));
            break;
        case kNCCLo:
            sprintf(text, "%d", FLOAT_TO_MIDI(fNCCLo));
            break;
        case kVCC:
            if (FLOAT_TO_MIDI2(fVCC) == 0)
            {
                strcpy(text, "Off");
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_MIDI2(fVCC) - 1);
            }
            break;
        case kVCCoff:
            if (FLOAT_TO_MIDI2(fVCCoff) == 0)
            {
                strcpy(text, "Off");
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_MIDI2(fVCCoff) - 1);
            }
            break;
        case kVCCHi:
            if (FLOAT_TO_MIDI(fVCCHi) == 0)
            {
                strcpy(text, "1");
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_MIDI(fVCCHi));
            }
            break;
        case kVCCLo:
            if (FLOAT_TO_MIDI(fVCCLo) == 0)
            {
                strcpy(text, "1");
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_MIDI(fVCCLo));
            }
            break;
        case kChout:
            if (FLOAT_TO_CHANNEL016(fChout) == 0)
            {
                strcpy(text, "Same as input");
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_CHANNEL016(fChout));
            }
            break;
        case kThru:
            if (fThru < 0.3f)
            {
                strcpy(text, "Off");
            }
            else if (fThru <= 0.6f)
            {
                strcpy(text, "Unconverted");
            }
            else
            {
                strcpy(text, "All");
            }
        default:
            break;
    }
}

void MidiNotesToCC::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];
        short status       = tomod.midiData[0] & 0xf0;       // scraping  channel
        short channel      = (tomod.midiData[0] & 0x0f) + 1; // isolating channel (1-16)
        short data1        = tomod.midiData[1] & 0x7f;
        short data2        = tomod.midiData[2] & 0x7f;

        if (status == MIDI_NOTEON && data2 == 0)
        {
            status = MIDI_NOTEOFF;
        }

        int lolimit1 = FLOAT_TO_MIDI(fNoteLo);
        int hilimit1 = FLOAT_TO_MIDI(fNoteHi);
        int low1     = FLOAT_TO_MIDI(fNCCLo);
        int high1    = FLOAT_TO_MIDI(fNCCHi);
        int lolimit2 = FLOAT_TO_MIDI(fVeloLo);
        int hilimit2 = FLOAT_TO_MIDI(fVeloHi);
        int low2     = FLOAT_TO_MIDI(fVCCLo);
        int high2    = FLOAT_TO_MIDI(fVCCHi);

        int chin  = FLOAT_TO_CHANNEL016(fChin);  //1-16
        int chout = FLOAT_TO_CHANNEL016(fChout); //1-16
        if (chout == 0)
        {
            chout = channel;
        }

        bool discard = (fThru < 0.3f); //Thru==No --> discard=true

        //------------------------------------------------------------------------------
        if (channel == chin || chin == 0)
        { //channel matches
            if (status == MIDI_NOTEON)
            {
                notePlaying[data1][channel] = sentNote;
                if (data1 >= lolimit1 && data1 <= hilimit1)
                { //note is in range
                    if (data2 >= lolimit2 && data2 <= hilimit2)
                    { //velocity is in range
                        if (FLOAT_TO_MIDI2(fNCC) > 0)
                        {
                            if (fThru <= 0.6f)
                            {
                                discard = true; //Thru!=Yes --> discard=true
                            }
                            int newdata = data1;
                            if (lolimit1 == hilimit1)
                            {
                                newdata = (low1 + high1) / 2;
                            }
                            //else if (fRangeMode1<0.3f)
                            //else newdata = MAP_TO_MIDI(data1,low1,high1,0,127);
                            else
                            {
                                newdata = MAP_TO_MIDI(data1, low1, high1, lolimit1, hilimit1);
                            }
                            if (newdata > 127)
                            {
                                newdata = 127;
                            }
                            else if (newdata < 0)
                            {
                                newdata = 0;
                            }
                            //if (FLOAT_TO_MIDI2(fNCC)==0) discard=true;
                            // create new message
                            VstMidiEvent NCC = inputs[0][i];
                            NCC.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                            NCC.midiData[1]  = FLOAT_TO_MIDI2(fNCC) - 1;
                            NCC.midiData[2]  = newdata;
                            outputs[0].push_back(NCC);
                            if (! discard)
                            {
                                notePlaying[data1][channel] = sentBoth;
                            }
                            else
                            {
                                notePlaying[data1][channel] = sentCC;
                            }
                        }
                        if (FLOAT_TO_MIDI2(fVCC) > 0)
                        {
                            if (fThru <= 0.6f)
                            {
                                discard = true; //Thru!=Yes --> discard=true
                            }
                            int newdata = data2;
                            if (lolimit2 == hilimit2)
                            {
                                newdata = (low2 + high2) / 2;
                            }
                            //else if (fRangeMode1<0.3f)
                            //else newdata = MAP_TO_MIDI(data2,low1,high1,0,127);
                            else
                            {
                                newdata = MAP_TO_MIDI(data2, low2, high2, lolimit2, hilimit2);
                            }
                            if (newdata > 127)
                            {
                                newdata = 127;
                            }
                            else if (newdata < 0)
                            {
                                newdata = 0;
                            }
                            // create new message
                            VstMidiEvent VCC = inputs[0][i];
                            VCC.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                            VCC.midiData[1]  = FLOAT_TO_MIDI2(fVCC) - 1;
                            VCC.midiData[2]  = newdata;
                            outputs[0].push_back(VCC);
                            if (! discard)
                            {
                                notePlaying[data1][channel] = sentBoth;
                            }
                            else
                            {
                                notePlaying[data1][channel] = sentCC;
                            }
                        }
                    }
                    else if (discard)
                    {
                        notePlaying[data1][channel] = 0;
                    }
                }
                else if (discard)
                {
                    notePlaying[data1][channel] = 0;
                }
            }
            else if (status == MIDI_NOTEOFF)
            {
                if (data1 >= lolimit1 && data1 <= hilimit1)
                { //note is in range
                    //don't send the "rest" cc if we didn't send the "on" cc:
                    if (notePlaying[data1][channel] == sentCC || notePlaying[data1][channel] == sentBoth)
                    {
                        if (FLOAT_TO_MIDI2(fNCCoff) > 0 && FLOAT_TO_MIDI2(fVCC) > 0)
                        {
                            if (fThru <= 0.6f)
                            {
                                discard = true;
                            }
                            VstMidiEvent NCC = inputs[0][i];
                            NCC.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                            NCC.midiData[1]  = FLOAT_TO_MIDI2(fNCC) - 1;
                            NCC.midiData[2]  = FLOAT_TO_MIDI2(fNCCoff) - 1;
                            outputs[0].push_back(NCC);
                        }
                        if (FLOAT_TO_MIDI2(fVCCoff) > 0 && FLOAT_TO_MIDI2(fVCC) > 0)
                        {
                            if (fThru <= 0.6f)
                            {
                                discard = true;
                            }
                            VstMidiEvent VCC = inputs[0][i];
                            VCC.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                            VCC.midiData[1]  = FLOAT_TO_MIDI2(fVCC) - 1;
                            VCC.midiData[2]  = FLOAT_TO_MIDI2(fVCCoff) - 1;
                            outputs[0].push_back(VCC);
                        }
                    }
                }
                notePlaying[data1][channel] = 0;
            }
        }
        //------------------------------------------------------------------------------
        //send thru the original message
        if (! discard)
        {
            outputs[0].push_back(tomod);
        }
    }
}
