#include "midiNotchFilter.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiNotchFilter(audioMaster);
}

MidiNotchFilterProgram::MidiNotchFilterProgram()
{
    // default Program Values
    for (int i = 0; i < 4; i++)
    {
        fmsg[i]   = 0;
        fch[i]    = CHANNEL_TO_FLOAT016(0);
        fvalue[i] = MIDI_TO_FLOAT2(0);
    }
    fmode  = 0;
    foutch = CHANNEL_TO_FLOAT016(0);
    // default program name
    strcpy(name, "Default");
}

//-----------------------------------------------------------------------------
MidiNotchFilter::MidiNotchFilter(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams), programs(0)
{
    programs = new MidiNotchFilterProgram[numPrograms];

    if (programs)
    {
        CFxBank* defaultBank = new CFxBank(kNumPrograms, kNumParams);
        if (readDefaultBank(PLUG_NAME, defaultBank))
        {
            if ((VstInt32) defaultBank->GetFxID() == PLUG_IDENT)
            {
                for (int i = 0; i < kNumPrograms; i++)
                {
                    programs[i].fmsg[0]   = defaultBank->GetProgParm(i, 0);
                    programs[i].fch[0]    = defaultBank->GetProgParm(i, 1);
                    programs[i].fvalue[0] = defaultBank->GetProgParm(i, 2);
                    programs[i].fmsg[1]   = defaultBank->GetProgParm(i, 3);
                    programs[i].fch[1]    = defaultBank->GetProgParm(i, 4);
                    programs[i].fvalue[1] = defaultBank->GetProgParm(i, 5);
                    programs[i].fmsg[2]   = defaultBank->GetProgParm(i, 6);
                    programs[i].fch[2]    = defaultBank->GetProgParm(i, 7);
                    programs[i].fvalue[2] = defaultBank->GetProgParm(i, 8);
                    programs[i].fmsg[3]   = defaultBank->GetProgParm(i, 9);
                    programs[i].fch[3]    = defaultBank->GetProgParm(i, 10);
                    programs[i].fvalue[3] = defaultBank->GetProgParm(i, 11);
                    programs[i].fmode     = defaultBank->GetProgParm(i, 12);
                    programs[i].foutch    = defaultBank->GetProgParm(i, 13);
                    strcpy(programs[i].name, defaultBank->GetProgramName(i));
                }
            }
        }
        else
        {
            // built-in programs
            for (int i = 0; i < kNumPrograms; i++)
            {
                switch (i)
                {
                    case 1:
                        programs[i].fmsg[0]   = 0.15f;
                        programs[i].fch[0]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[0] = MIDI_TO_FLOAT2(8.1);
                        programs[i].fmsg[1]   = 0.15f;
                        programs[i].fch[1]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[1] = MIDI_TO_FLOAT2(12.1);
                        programs[i].fmsg[2]   = 0.15f;
                        programs[i].fch[2]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[2] = MIDI_TO_FLOAT2(74.1);
                        programs[i].fmsg[3]   = 0.15f;
                        programs[i].fch[3]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[3] = MIDI_TO_FLOAT2(124.1);
                        programs[i].fmode     = 0.0f;
                        sprintf(programs[i].name, "Block Some CCs");
                        break;
                    case 2:
                        programs[i].fmsg[0]   = 0.3f;
                        programs[i].fch[0]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[0] = MIDI_TO_FLOAT2(0);
                        programs[i].fmsg[1]   = 0.0f;
                        programs[i].fch[1]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[1] = MIDI_TO_FLOAT2(0);
                        programs[i].fmsg[2]   = 0.0f;
                        programs[i].fch[2]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[2] = MIDI_TO_FLOAT2(0);
                        programs[i].fmsg[3]   = 0.0f;
                        programs[i].fch[3]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[3] = MIDI_TO_FLOAT2(0);
                        programs[i].fmode     = 0.0f;
                        sprintf(programs[i].name, "Block All Notes");
                        break;
                    case 3:
                        programs[i].fmsg[0]   = 0.5f;
                        programs[i].fch[0]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[0] = MIDI_TO_FLOAT2(0);
                        programs[i].fmsg[1]   = 0.6f;
                        programs[i].fch[1]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[1] = MIDI_TO_FLOAT2(0);
                        programs[i].fmsg[2]   = 0.8f;
                        programs[i].fch[2]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[2] = MIDI_TO_FLOAT2(0);
                        programs[i].fmsg[3]   = 0.9f;
                        programs[i].fch[3]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[3] = MIDI_TO_FLOAT2(0);
                        programs[i].fmode     = 0.0f;
                        sprintf(programs[i].name, "Block Other Stuff");
                        break;
                    case 4:
                        programs[i].fmsg[0]   = 0.2f;
                        programs[i].fch[0]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[0] = MIDI_TO_FLOAT2(0);
                        programs[i].fmsg[1]   = 0.0f;
                        programs[i].fch[1]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[1] = MIDI_TO_FLOAT2(0);
                        programs[i].fmsg[2]   = 0.0f;
                        programs[i].fch[2]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[2] = MIDI_TO_FLOAT2(0);
                        programs[i].fmsg[3]   = 0.0f;
                        programs[i].fch[3]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[3] = MIDI_TO_FLOAT2(0);
                        programs[i].fmode     = 0.8f;
                        sprintf(programs[i].name, "Pass Only CCs");
                        break;
                    case 5:
                        programs[i].fmsg[0]   = 0.3f;
                        programs[i].fch[0]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[0] = MIDI_TO_FLOAT2(0);
                        programs[i].fmsg[1]   = 0.0f;
                        programs[i].fch[1]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[1] = MIDI_TO_FLOAT2(0);
                        programs[i].fmsg[2]   = 0.0f;
                        programs[i].fch[2]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[2] = MIDI_TO_FLOAT2(0);
                        programs[i].fmsg[3]   = 0.0f;
                        programs[i].fch[3]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[3] = MIDI_TO_FLOAT2(0);
                        programs[i].fmode     = 0.8f;
                        sprintf(programs[i].name, "Pass Only Notes");
                        break;
                    case 6:
                        programs[i].fmsg[0]   = 0.0f;
                        programs[i].fch[0]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[0] = MIDI_TO_FLOAT2(0);
                        programs[i].fmsg[1]   = 0.0f;
                        programs[i].fch[1]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[1] = MIDI_TO_FLOAT2(0);
                        programs[i].fmsg[2]   = 0.0f;
                        programs[i].fch[2]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[2] = MIDI_TO_FLOAT2(0);
                        programs[i].fmsg[3]   = 0.0f;
                        programs[i].fch[3]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[3] = MIDI_TO_FLOAT2(0);
                        programs[i].fmode     = 0.0f;
                        sprintf(programs[i].name, "Block Nothing");
                        break;
                    case 7:
                        programs[i].fmsg[0]   = 0.0f;
                        programs[i].fch[0]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[0] = MIDI_TO_FLOAT2(0);
                        programs[i].fmsg[1]   = 0.0f;
                        programs[i].fch[1]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[1] = MIDI_TO_FLOAT2(0);
                        programs[i].fmsg[2]   = 0.0f;
                        programs[i].fch[2]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[2] = MIDI_TO_FLOAT2(0);
                        programs[i].fmsg[3]   = 0.0f;
                        programs[i].fch[3]    = CHANNEL_TO_FLOAT016(0);
                        programs[i].fvalue[3] = MIDI_TO_FLOAT2(0);
                        programs[i].fmode     = 0.8f;
                        sprintf(programs[i].name, "Pass Nothing");
                        break;
                    default:
                        sprintf(programs[i].name, "Program %d", i + 1);
                        break;
                }
            }
        }
        setProgram(0);
    }

    init();
}

//-----------------------------------------------------------------------------------------
MidiNotchFilter::~MidiNotchFilter()
{
    if (programs)
    {
        delete[] programs;
    }
}

//------------------------------------------------------------------------
void MidiNotchFilter::setProgram(VstInt32 program)
{
    MidiNotchFilterProgram* ap = &programs[program];

    curProgram = program;
    setParameter(kmsg1, ap->fmsg[0]);
    setParameter(kch1, ap->fch[0]);
    setParameter(kvalue1, ap->fvalue[0]);

    setParameter(kmsg2, ap->fmsg[1]);
    setParameter(kch2, ap->fch[1]);
    setParameter(kvalue2, ap->fvalue[1]);

    setParameter(kmsg3, ap->fmsg[2]);
    setParameter(kch3, ap->fch[2]);
    setParameter(kvalue3, ap->fvalue[2]);

    setParameter(kmsg4, ap->fmsg[3]);
    setParameter(kch4, ap->fch[3]);
    setParameter(kvalue4, ap->fvalue[3]);

    setParameter(kmode, ap->fmode);
    setParameter(koutch, ap->foutch);
}

//------------------------------------------------------------------------
void MidiNotchFilter::setProgramName(char* name)
{
    vst_strncpy(programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiNotchFilter::getProgramName(char* name)
{
    if (! strcmp(programs[curProgram].name, "Init"))
    {
        sprintf(name, "%s %d", programs[curProgram].name, curProgram + 1);
    }
    else
    {
        strcpy(name, programs[curProgram].name);
    }
}

//-----------------------------------------------------------------------------------------
bool MidiNotchFilter::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy(text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiNotchFilter::setParameter(VstInt32 index, float value)
{
    MidiNotchFilterProgram* ap = &programs[curProgram];

    switch (index)
    {
        case kmsg1:
            fmsg[0] = ap->fmsg[0] = value;
            updateDisplay();
            break;
        case kch1:
            fch[0] = ap->fch[0] = value;
            break;
        case kvalue1:
            fvalue[0] = ap->fvalue[0] = value;
            break;
        case kmsg2:
            fmsg[1] = ap->fmsg[1] = value;
            updateDisplay();
            break;
        case kch2:
            fch[1] = ap->fch[1] = value;
            break;
        case kvalue2:
            fvalue[1] = ap->fvalue[1] = value;
            break;
        case kmsg3:
            fmsg[2] = ap->fmsg[2] = value;
            updateDisplay();
            break;
        case kch3:
            fch[2] = ap->fch[2] = value;
            break;
        case kvalue3:
            fvalue[2] = ap->fvalue[2] = value;
            break;
        case kmsg4:
            fmsg[3] = ap->fmsg[3] = value;
            updateDisplay();
            break;
        case kch4:
            fch[3] = ap->fch[3] = value;
            break;
        case kvalue4:
            fvalue[3] = ap->fvalue[3] = value;
            break;
        case kmode:
            fmode = ap->fmode = value;
            break;
        case koutch:
            foutch = ap->foutch = value;
            updateDisplay();
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
float MidiNotchFilter::getParameter(VstInt32 index)
{
    float v = 0;

    switch (index)
    {
        case kmsg1:
            v = fmsg[0];
            break;
        case kch1:
            v = fch[0];
            break;
        case kvalue1:
            v = fvalue[0];
            break;
        case kmsg2:
            v = fmsg[1];
            break;
        case kch2:
            v = fch[1];
            break;
        case kvalue2:
            v = fvalue[1];
            break;
        case kmsg3:
            v = fmsg[2];
            break;
        case kch3:
            v = fch[2];
            break;
        case kvalue3:
            v = fvalue[2];
            break;
        case kmsg4:
            v = fmsg[3];
            break;
        case kch4:
            v = fch[3];
            break;
        case kvalue4:
            v = fvalue[3];
            break;
        case kmode:
            v = fmode;
            break;
        case koutch:
            v = foutch;
            break;
        default:
            break;
    }
    return v;
}

//-----------------------------------------------------------------------------------------
void MidiNotchFilter::getParameterName(VstInt32 index, char* label)
{
    switch (index)
    {
        case kmsg1:
            strcpy(label, "Message 1---");
            break;
        case kch1:
            strcpy(label, "  1.Channel");
            break;
        case kvalue1:
            strcpy(label, "  1.Value");
            break;
        case kmsg2:
            strcpy(label, "Message 2---");
            break;
        case kch2:
            strcpy(label, "  2.Channel");
            break;
        case kvalue2:
            strcpy(label, "  2.Value");
            break;
        case kmsg3:
            strcpy(label, "Message 3---");
            break;
        case kch3:
            strcpy(label, "  3.Channel");
            break;
        case kvalue3:
            strcpy(label, "  3.Value");
            break;
        case kmsg4:
            strcpy(label, "Message 4---");
            break;
        case kch4:
            strcpy(label, "  4.Channel");
            break;
        case kvalue4:
            strcpy(label, "  4.Value");
            break;
        case kmode:
            strcpy(label, "Mode--------");
            break;
        case koutch:
            strcpy(label, "Out Channel");
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiNotchFilter::getParameterDisplay(VstInt32 index, char* text)
{
    const float inc = 1.f / 7.f;
    switch (index)
    {
        case kmsg1:
            if (fmsg[0] < 1 * inc)
            {
                strcpy(text, "None");
            }
            else if (fmsg[0] < 2 * inc)
            {
                strcpy(text, "CC");
            }
            else if (fmsg[0] < 3 * inc)
            {
                strcpy(text, "Note");
            }
            else if (fmsg[0] < 4 * inc)
            {
                strcpy(text, "Pitch Bend");
            }
            else if (fmsg[0] < 5 * inc)
            {
                strcpy(text, "Chan.Pressure");
            }
            else if (fmsg[0] < 6 * inc)
            {
                strcpy(text, "Aftertouch");
            }
            else
            {
                strcpy(text, "Prog.Change");
            }
            break;

        case kch1:
            if (fmsg[0] < 1 * inc)
            {
                strcpy(text, " ");
            }
            else
            {
                if (FLOAT_TO_CHANNEL016(fch[0]) < 1)
                {
                    strcpy(text, "Any");
                }
                else
                {
                    sprintf(text, "Channel %d", FLOAT_TO_CHANNEL016(fch[0]));
                }
            }
            break;

        case kvalue1:
            if (fmsg[0] < 1 * inc)
            {
                strcpy(text, " ");
            }
            else if (fmsg[0] < 2 * inc)
            {
                if (FLOAT_TO_MIDI2(fvalue[0]) == 0.0f)
                {
                    strcpy(text, "All CCs");
                }
                else
                {
                    sprintf(text, "CC %d", (FLOAT_TO_MIDI2(fvalue[0])) - 1);
                }
            }
            else if (fmsg[0] < 3 * inc)
            {
                if (FLOAT_TO_MIDI2(fvalue[0]) == 0.0f)
                {
                    strcpy(text, "All Notes");
                }
                else
                {
                    strcpy(text, getNoteName((FLOAT_TO_MIDI2(fvalue[0])) - 1, bottomOctave));
                }
            }
            else if (fmsg[0] < 4 * inc)
            {
                strcpy(text, "All");
            }
            else if (fmsg[0] < 5 * inc)
            {
                strcpy(text, "All");
            }
            else if (fmsg[0] < 6 * inc)
            {
                if FLOAT_TO_MIDI2 (fvalue[0] == 0.0f)
                {
                    strcpy(text, "All Notes");
                }
                else
                {
                    strcpy(text, getNoteName((FLOAT_TO_MIDI2(fvalue[0])) - 1, bottomOctave));
                }
            }
            else
            {
                strcpy(text, "All");
            }
            break;

        case kmsg2:
            if (fmsg[1] < 1 * inc)
            {
                strcpy(text, "None");
            }
            else if (fmsg[1] < 2 * inc)
            {
                strcpy(text, "CC");
            }
            else if (fmsg[1] < 3 * inc)
            {
                strcpy(text, "Note");
            }
            else if (fmsg[1] < 4 * inc)
            {
                strcpy(text, "Pitch Bend");
            }
            else if (fmsg[1] < 5 * inc)
            {
                strcpy(text, "Chan.Pressure");
            }
            else if (fmsg[1] < 6 * inc)
            {
                strcpy(text, "Aftertouch");
            }
            else
            {
                strcpy(text, "Prog.Change");
            }
            break;

        case kch2:
            if (fmsg[1] < 1 * inc)
            {
                strcpy(text, "");
            }
            else
            {
                if (FLOAT_TO_CHANNEL016(fch[1]) < 1)
                {
                    strcpy(text, "Any");
                }
                else
                {
                    sprintf(text, "Channel %d", FLOAT_TO_CHANNEL016(fch[1]));
                }
            }
            break;

        case kvalue2:
            if (fmsg[1] < 1 * inc)
            {
                strcpy(text, "");
            }
            else if (fmsg[1] < 2 * inc)
            {
                if (FLOAT_TO_MIDI2(fvalue[1]) == 0.0f)
                {
                    strcpy(text, "All CCs");
                }
                else
                {
                    sprintf(text, "CC %d", (FLOAT_TO_MIDI2(fvalue[1])) - 1);
                }
            }
            else if (fmsg[1] < 3 * inc)
            {
                if (FLOAT_TO_MIDI2(fvalue[1]) == 0.0f)
                {
                    strcpy(text, "All Notes");
                }
                else
                {
                    strcpy(text, getNoteName((FLOAT_TO_MIDI2(fvalue[1])) - 1, bottomOctave));
                }
            }
            else if (fmsg[1] < 4 * inc)
            {
                strcpy(text, "All");
            }
            else if (fmsg[1] < 5 * inc)
            {
                strcpy(text, "All");
            }
            else if (fmsg[1] < 6 * inc)
            {
                if (FLOAT_TO_MIDI2(fvalue[1]) == 0.0f)
                {
                    strcpy(text, "All Notes");
                }
                else
                {
                    strcpy(text, getNoteName((FLOAT_TO_MIDI2(fvalue[1])) - 1, bottomOctave));
                }
            }
            else
            {
                strcpy(text, "All");
            }
            break;

        case kmsg3:
            if (fmsg[2] < 1 * inc)
            {
                strcpy(text, "None");
            }
            else if (fmsg[2] < 2 * inc)
            {
                strcpy(text, "CC");
            }
            else if (fmsg[2] < 3 * inc)
            {
                strcpy(text, "Note");
            }
            else if (fmsg[2] < 4 * inc)
            {
                strcpy(text, "Pitch Bend");
            }
            else if (fmsg[2] < 5 * inc)
            {
                strcpy(text, "Chan.Pressure");
            }
            else if (fmsg[2] < 6 * inc)
            {
                strcpy(text, "Aftertouch");
            }
            else
            {
                strcpy(text, "Prog.Change");
            }
            break;

        case kch3:
            if (fmsg[2] < 1 * inc)
            {
                strcpy(text, "");
            }
            else
            {
                if (FLOAT_TO_CHANNEL016(fch[2]) < 1)
                {
                    strcpy(text, "Any");
                }
                else
                {
                    sprintf(text, "Channel %d", FLOAT_TO_CHANNEL016(fch[2]));
                }
            }
            break;

        case kvalue3:
            if (fmsg[2] < 1 * inc)
            {
                strcpy(text, "");
            }
            else if (fmsg[2] < 2 * inc)
            {
                if (FLOAT_TO_MIDI2(fvalue[2]) == 0.0f)
                {
                    strcpy(text, "All CCs");
                }
                else
                {
                    sprintf(text, "CC %d", (FLOAT_TO_MIDI2(fvalue[2])) - 1);
                }
            }
            else if (fmsg[2] < 3 * inc)
            {
                if (FLOAT_TO_MIDI2(fvalue[2]) == 0.0f)
                {
                    strcpy(text, "All Notes");
                }
                else
                {
                    strcpy(text, getNoteName((FLOAT_TO_MIDI2(fvalue[2])) - 1, bottomOctave));
                }
            }
            else if (fmsg[2] < 4 * inc)
            {
                strcpy(text, "All");
            }
            else if (fmsg[2] < 5 * inc)
            {
                strcpy(text, "All");
            }
            else if (fmsg[2] < 6 * inc)
            {
                if (FLOAT_TO_MIDI2(fvalue[2]) == 0.0f)
                {
                    strcpy(text, "All Notes");
                }
                else
                {
                    strcpy(text, getNoteName((FLOAT_TO_MIDI2(fvalue[2])) - 1, bottomOctave));
                }
            }
            else
            {
                strcpy(text, "All");
            }
            break;

        case kmsg4:
            if (fmsg[3] < 1 * inc)
            {
                strcpy(text, "None");
            }
            else if (fmsg[3] < 2 * inc)
            {
                strcpy(text, "CC");
            }
            else if (fmsg[3] < 3 * inc)
            {
                strcpy(text, "Note");
            }
            else if (fmsg[3] < 4 * inc)
            {
                strcpy(text, "Pitch Bend");
            }
            else if (fmsg[3] < 5 * inc)
            {
                strcpy(text, "Chan.Pressure");
            }
            else if (fmsg[3] < 6 * inc)
            {
                strcpy(text, "Aftertouch");
            }
            else
            {
                strcpy(text, "Prog.Change");
            }
            break;

        case kch4:
            if (fmsg[3] < 1 * inc)
            {
                strcpy(text, "");
            }
            else
            {
                if (FLOAT_TO_CHANNEL016(fch[3]) < 1)
                {
                    strcpy(text, "Any");
                }
                else
                {
                    sprintf(text, "Channel %d", FLOAT_TO_CHANNEL016(fch[3]));
                }
            }
            break;

        case kvalue4:
            if (fmsg[3] < 1 * inc)
            {
                strcpy(text, "");
            }
            else if (fmsg[3] < 2 * inc)
            {
                if (FLOAT_TO_MIDI2(fvalue[3]) == 0.0f)
                {
                    strcpy(text, "All CCs");
                }
                else
                {
                    sprintf(text, "CC %d", (FLOAT_TO_MIDI2(fvalue[3])) - 1);
                }
            }
            else if (fmsg[3] < 3 * inc)
            {
                if (FLOAT_TO_MIDI2(fvalue[3]) == 0.0f)
                {
                    strcpy(text, "All Notes");
                }
                else
                {
                    strcpy(text, getNoteName((FLOAT_TO_MIDI2(fvalue[3])) - 1, bottomOctave));
                }
            }
            else if (fmsg[3] < 4 * inc)
            {
                strcpy(text, "All");
            }
            else if (fmsg[3] < 5 * inc)
            {
                strcpy(text, "All");
            }
            else if (fmsg[3] < 6 * inc)
            {
                if (FLOAT_TO_MIDI2(fvalue[3]) == 0.0f)
                {
                    strcpy(text, "All Notes");
                }
                else
                {
                    strcpy(text, getNoteName((FLOAT_TO_MIDI2(fvalue[3])) - 1, bottomOctave));
                }
            }
            else
            {
                strcpy(text, "All");
            }
            break;

        case kmode:
            if (fmode < 0.5)
            {
                if (FLOAT_TO_CHANNEL016(foutch) > 0)
                {
                    strcpy(text, "Channelize");
                }
                else
                {
                    strcpy(text, "Block");
                }
            }
            else
            {
                strcpy(text, "Pass");
            }
            break;

        case koutch:
            if (FLOAT_TO_CHANNEL016(foutch) > 0)
            {
                sprintf(text, "%d", FLOAT_TO_CHANNEL016(foutch));
            }
            else
            {
                strcpy(text, "");
            }
            break;
        default:
            break;
    }
}

void MidiNotchFilter::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];
        short status       = tomod.midiData[0] & 0xf0;       // scraping  channel
        short channel      = (tomod.midiData[0] & 0x0f) + 1; // isolating channel (1-16)
        short data1        = tomod.midiData[1] & 0x7f;
        //short data2      = tomod.midiData[2] & 0x7f;

        bool discard = false;
        bool match   = false;

        int msg[4];
        int ch[4];
        int value[4];
        const float inc = 1.f / 7.f;
        for (int n = 0; n < 4; n++)
        {
            if (fmsg[n] < inc)
            {
                break;
            }
            else if (fmsg[n] < 2 * inc)
            {
                msg[n] = MIDI_CONTROLCHANGE;
            }
            else if (fmsg[n] < 3 * inc)
            {
                msg[n] = MIDI_NOTEON;
            }
            else if (fmsg[n] < 4 * inc)
            {
                msg[n] = MIDI_PITCHBEND;
            }
            else if (fmsg[n] < 5 * inc)
            {
                msg[n] = MIDI_CHANNELPRESSURE;
            }
            else if (fmsg[n] < 6 * inc)
            {
                msg[n] = MIDI_POLYKEYPRESSURE;
            }
            else
            {
                msg[n] = MIDI_PROGRAMCHANGE;
            }

            ch[n] = FLOAT_TO_CHANNEL016(fch[n]);
            if (ch[n] == 0)
            {
                ch[n] = channel;
            }
            value[n] = FLOAT_TO_MIDI2(fvalue[n]); // 1-128, 0=all
            if (value[n] == 0)
            {
                value[n] = data1;
            }
            else
            {
                value[n] = value[n] - 1;
            }

            if (channel == ch[n])
            {
                if (status == msg[n])
                {
                    if (msg[n] == MIDI_CONTROLCHANGE || msg[n] == MIDI_NOTEON || msg[n] == MIDI_POLYKEYPRESSURE)
                    {
                        if (data1 == value[n])
                        {
                            match = true;
                        }
                    }
                    else if (msg[n] == MIDI_PITCHBEND || msg[n] == MIDI_CHANNELPRESSURE || msg[n] == MIDI_PROGRAMCHANGE)
                    {
                        match = true;
                    }
                }
                else if (msg[n] == MIDI_NOTEON && status == MIDI_NOTEOFF)
                {
                    if (data1 == value[n])
                    {
                        match = true;
                    }
                }
            }
        }

        if (fmode < 0.5f)
        {
            if (match)
            {
                discard = true;
                if (FLOAT_TO_CHANNEL016(foutch) > 0)
                {
                    tomod.midiData[0] = status | ((FLOAT_TO_CHANNEL016(foutch)) - 1);
                    discard           = false;
                }
            }
        }
        else if (fmode >= 0.5f)
        {
            if (! match)
            {
                discard = true;
            }
            if (match && FLOAT_TO_CHANNEL016(foutch) > 0)
            {
                tomod.midiData[0] = status | ((FLOAT_TO_CHANNEL016(foutch)) - 1);
            }
        }
        if (! discard)
        {
            outputs[0].push_back(tomod);
        }
    }
}
