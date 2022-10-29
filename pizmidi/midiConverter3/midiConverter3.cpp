#include "midiConverter3.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiConverter(audioMaster);
}

MidiConverterProgram::MidiConverterProgram()
{
    // default Program Values
    param[kIn]        = 0.f;                //CC in
    param[kCCin]      = MIDI_TO_FLOAT(2.1); //CC 1
    param[kNRPNin]    = 0.f;
    param[kChin]      = CHANNEL_TO_FLOAT(-1); //Any Channel
    param[kLowLimit]  = 0.f;                  //
    param[kHighLimit] = 1.f;                  //   full
    param[kLow]       = 0.f;                  //   range
    param[kHigh]      = 1.f;                  //
    param[kRangeMode] = 0.f;
    param[kOffset]    = MIDI_TO_FLOAT(63.1); //no offset
    param[kOut]       = 0.02f;               //CC out
    param[kCCout]     = MIDI_TO_FLOAT2(2.1); //really 1
    param[kNRPNout]   = 0.f;
    param[kChout]     = CHANNEL_TO_FLOAT(-1); //same channel out
    param[kThru]      = 0.f;                  //don't pass on original message
    // default program name
    strcpy(name, "Default");
}

//-----------------------------------------------------------------------------
MidiConverter::MidiConverter(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams), programs(0)
{
    programs = new MidiConverterProgram[numPrograms];
    outmode  = drop;
    inmode   = cc;

    if (programs)
    {
        CFxBank* defaultBank = new CFxBank(kNumPrograms, kNumParams);
        if (readDefaultBank(PLUG_NAME, defaultBank))
        {
            if ((VstInt32) defaultBank->GetFxID() == PLUG_IDENT)
            {
                int i;
                for (i = 0; i < kNumPrograms; i++)
                {
                    for (int p = 0; p < kNumParams; p++)
                    {
                        programs[i].param[p] = defaultBank->GetProgParm(i, p);
                    }
                    strcpy(programs[i].name, defaultBank->GetProgramName(i));
                }
            }
        }
        else
        {
            // built-in programs
            int i;
            for (i = 0; i < kNumPrograms; i++)
            {
                switch (i)
                {
                    case 0:
                        sprintf(programs[i].name, "Nothing");
                        break;
                    default:
                        sprintf(programs[i].name, "Program %d", i + 1);
                        break;
                }
            }
        }
        setProgram(0);
    }

    for (int ch = 0; ch < 16; ch++)
    {
        nrpn[ch]       = -1;
        nrpncoarse[ch] = -1;
        rpn[ch]        = -1;
        rpncoarse[ch]  = -1;
        datafine[ch]   = 0;
        datacoarse[ch] = 0;
        data[ch]       = 0;
        done[ch]       = true;
        smoothcc[ch]   = -1;
        lastPC[ch]     = 0;
        lastSentPC[ch] = 0;
        for (int i = 0; i < 32; i++)
        {
            cc14msb[i][ch] = 0;
            cc14lsb[i][ch] = 0;
        }
    }

    //smoothing stuff
    counter  = roundToInt(sampleRate * 0.001f);
    lastpb   = 0x2000;
    targetpb = 0x2000;
    lastcc   = 0;
    targetcc = 0;

    init();
}

//-----------------------------------------------------------------------------------------
MidiConverter::~MidiConverter()
{
    if (programs)
    {
        delete[] programs;
    }
}

//------------------------------------------------------------------------
void MidiConverter::setProgram(VstInt32 program)
{
    MidiConverterProgram* ap = &programs[program];

    curProgram = program;
    for (int i = 0; i < kNumParams; i++)
    {
        setParameter(i, ap->param[i]);
    }
}

//------------------------------------------------------------------------
void MidiConverter::setProgramName(char* name)
{
    vst_strncpy(programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiConverter::getProgramName(char* name)
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
bool MidiConverter::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy(text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiConverter::resume()
{
    AudioEffectX::resume();
    lastpb   = 0x2000;
    targetpb = 0x2000;
    counter  = roundToInt(sampleRate * 0.001f);
    for (int i = 0; i < 16; i++)
    {
        done[i] = true;
    }
}

//-----------------------------------------------------------------------------------------
void MidiConverter::setParameter(VstInt32 index, float value)
{
    MidiConverterProgram* ap = &programs[curProgram];

    int lastoutmode = outmode;
    int lastinmode  = inmode;
    float inc       = 1.f / 18.f;
    switch (index)
    {
        case kIn:
            param[kIn] = ap->param[kIn] = value;
            if (param[kIn] < 1 * inc)
            {
                inmode = cc;
            }
            else if (param[kIn] < 2 * inc)
            {
                inmode = cc14;
            }
            else if (param[kIn] < 3 * inc)
            {
                inmode = pc;
            }
            else if (param[kIn] < 4 * inc)
            {
                inmode = pcdec;
            }
            else if (param[kIn] < 5 * inc)
            {
                inmode = pcinc;
            }
            else if (param[kIn] < 6 * inc)
            {
                inmode = cp;
            }
            else if (param[kIn] < 7 * inc)
            {
                inmode = pa;
            }
            else if (param[kIn] < 8 * inc)
            {
                inmode = pb;
            }
            else if (param[kIn] < 9 * inc)
            {
                inmode = pblsb;
            }
            else if (param[kIn] < 10 * inc)
            {
                inmode = NRPN;
            }
            else if (param[kIn] < 11 * inc)
            {
                inmode = NRPNlsb;
            }
            else if (param[kIn] < 12 * inc)
            {
                inmode = RPN;
            }
            else if (param[kIn] < 13 * inc)
            {
                inmode = RPNlsb;
            }
            else if (param[kIn] < 14 * inc)
            {
                inmode = nonn;
            }
            else if (param[kIn] < 15 * inc)
            {
                inmode = nonv;
            }
            else if (param[kIn] < 16 * inc)
            {
                inmode = noffn;
            }
            else if (param[kIn] < 17 * inc)
            {
                inmode = noffv;
            }
            else
            {
                inmode = clock;
            }
            //else if (param[kIn]<0.9f)  inmode=undefined;
            //else if (param[kIn]<0.96f) inmode=undefined;
            //else if (param[kIn]<1.0f)  inmode=undefined;
            //else                       inmode=undefined;
            if (lastinmode != inmode)
            {
                updateDisplay();
            }
            break;
        case kCCin:
            param[kCCin] = ap->param[kCCin] = value;
            updateDisplay();
            break;
        case kNRPNin:
            param[kNRPNin] = ap->param[kNRPNin] = value;
            break;
        case kChin:
            param[kChin] = ap->param[kChin] = value;
            break;
        case kLowLimit:
            if (value > param[kHighLimit])
            {
                setParameterAutomated(kHighLimit, value);
            }
            param[kLowLimit] = ap->param[kLowLimit] = value;
            break;
        case kHighLimit:
            if (param[kLowLimit] > value)
            {
                setParameterAutomated(kLowLimit, value);
            }
            param[kHighLimit] = ap->param[kHighLimit] = value;
            break;
        case kLow:
            param[kLow] = ap->param[kLow] = value;
            break;
        case kHigh:
            param[kHigh] = ap->param[kHigh] = value;
            break;
        case kRangeMode:
            param[kRangeMode] = ap->param[kRangeMode] = value;
            break;
        case kOffset:
            param[kOffset] = ap->param[kOffset] = value;
            break;
        case kOut:
            param[kOut] = ap->param[kOut] = value;
            inc                           = 1.f / 20.f;
            if (param[kOut] < 1 * inc)
            {
                outmode = drop;
            }
            else if (param[kOut] < 2 * inc)
            {
                outmode = cc;
            }
            else if (param[kOut] < 3 * inc)
            {
                outmode = cc14;
            }
            else if (param[kOut] < 4 * inc)
            {
                outmode = pc;
            }
            else if (param[kOut] < 5 * inc)
            {
                outmode = pcdec;
            }
            else if (param[kOut] < 6 * inc)
            {
                outmode = pcinc;
            }
            else if (param[kOut] < 7 * inc)
            {
                outmode = cp;
            }
            else if (param[kOut] < 8 * inc)
            {
                outmode = pa;
            }
            else if (param[kOut] < 9 * inc)
            {
                outmode = pb;
            }
            else if (param[kOut] < 10 * inc)
            {
                outmode = pblsb;
            }
            else if (param[kOut] < 11 * inc)
            {
                outmode = NRPN;
            }
            else if (param[kOut] < 12 * inc)
            {
                outmode = NRPNlsb;
            }
            else if (param[kOut] < 13 * inc)
            {
                outmode = RPN;
            }
            else if (param[kOut] < 14 * inc)
            {
                outmode = RPNlsb;
            }
            else if (param[kOut] < 15 * inc)
            {
                outmode = nonn;
            }
            else if (param[kOut] < 16 * inc)
            {
                outmode = nonv;
            }
            else if (param[kOut] < 17 * inc)
            {
                outmode = noffn;
            }
            else if (param[kOut] < 18 * inc)
            {
                outmode = noffv;
            }
            else if (param[kOut] < 19 * inc)
            {
                outmode = clock;
            }
            else
            {
                outmode = songselect;
            }
            //else if (param[kOut]<1.0f)  outmode=undefined;
            //else                        outmode=undefined;
            if (lastoutmode != outmode)
            {
                updateDisplay();
            }
            break;
        case kCCout:
            param[kCCout] = ap->param[kCCout] = value;
            updateDisplay();
            break;
        case kNRPNout:
            param[kNRPNout] = ap->param[kNRPNout] = value;
            break;
        case kChout:
            param[kChout] = ap->param[kChout] = value;
            break;
        case kThru:
            param[kThru] = ap->param[kThru] = value;
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
float MidiConverter::getParameter(VstInt32 index)
{
    if (index < kNumParams)
    {
        return param[index];
    }
    return 0.f;
}

//-----------------------------------------------------------------------------------------
void MidiConverter::getParameterName(VstInt32 index, char* label)
{
    switch (index)
    {
        case kIn:
            strcpy(label, "Input Type");
            break;
        case kCCin:
            strcpy(label, "In Param 1");
            break;
        case kNRPNin:
            strcpy(label, "In Param 2");
            break;
        case kChin:
            strcpy(label, "Channel in");
            break;
        case kLowLimit:
            strcpy(label, "Low Input");
            break;
        case kHighLimit:
            strcpy(label, "High Input");
            break;

        case kRangeMode:
            strcpy(label, "Map Mode");
            break;

        case kLow:
            strcpy(label, "Low Output");
            break;
        case kHigh:
            strcpy(label, "High Output");
            break;
        case kOffset:
            strcpy(label, "Offset");
            break;
        case kOut:
            strcpy(label, "Output Type");
            break;
        case kCCout:
            strcpy(label, "Out Param 1");
            break;
        case kNRPNout:
            strcpy(label, "Out Param 2");
            break;
        case kChout:
            strcpy(label, "Channel out");
            break;
        case kThru:
            strcpy(label, "Thru");
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiConverter::getParameterDisplay(VstInt32 index, char* text)
{
    switch (index)
    {
        case kIn:
            switch (inmode)
            {
                case cc:
                    strcpy(text, "CC");
                    break;
                case cc14:
                    strcpy(text, "14-bit CC");
                    break;
                case pc:
                    strcpy(text, "Program Change");
                    break;
                case pcinc:
                    strcpy(text, "ProgChg Inc");
                    break;
                case pcdec:
                    strcpy(text, "ProgChg Dec");
                    break;
                case cp:
                    strcpy(text, "Channel Pressure");
                    break;
                case pa:
                    strcpy(text, "Poly Aftertouch");
                    break;
                case pb:
                    strcpy(text, "Pitch Bend");
                    break;
                case pblsb:
                    strcpy(text, "Pitch Bend (LSB)");
                    break;
                case NRPN:
                    strcpy(text, "NRPN");
                    break;
                case NRPNlsb:
                    strcpy(text, "NRPN (LSB)");
                    break;
                case RPN:
                    strcpy(text, "RPN");
                    break;
                case RPNlsb:
                    strcpy(text, "RPN (LSB)");
                    break;
                case nonn:
                    strcpy(text, "Note On #");
                    break;
                case nonv:
                    strcpy(text, "Note On Velocity");
                    break;
                case noffn:
                    strcpy(text, "Note Off #");
                    break;
                case noffv:
                    strcpy(text, "Note Off Velocity");
                    break;
                case clock:
                    strcpy(text, "MIDI Clock");
                    break;
                default:
                    strcpy(text, "???");
                    break;
            }
            break;
        case kCCin:
            if (inmode == cc)
            {
                if (FLOAT_TO_MIDI_X(param[kCCin]) == -1)
                {
                    strcpy(text, "Any CC");
                }
                else
                {
                    sprintf(text, "CC %d", FLOAT_TO_MIDI_X(param[kCCin]));
                }
            }
            else if (inmode == cc14)
            {
                if (FLOAT_TO_MIDI_X(param[kCCin]) == -1)
                {
                    strcpy(text, "Any CC");
                }
                else if (FLOAT_TO_MIDI_X(param[kCCin]) < 32)
                {
                    sprintf(text, "CC %d / %d", FLOAT_TO_MIDI_X(param[kCCin]), FLOAT_TO_MIDI_X(param[kCCin]) + 32);
                }
                else
                {
                    sprintf(text, "CC %d", FLOAT_TO_MIDI_X(param[kCCin]));
                }
            }
            else if (inmode == pa || inmode == nonv || inmode == noffv)
            {
                if (FLOAT_TO_MIDI_X(param[kCCin]) == -1)
                {
                    strcpy(text, "Any Note");
                }
                else
                {
                    sprintf(text, "Note %d (%s)", FLOAT_TO_MIDI_X(param[kCCin]), getNoteName(FLOAT_TO_MIDI_X(param[kCCin]), bottomOctave));
                }
            }
            else if (inmode == NRPN || inmode == NRPNlsb || inmode == RPN || inmode == RPNlsb)
            {
                if (FLOAT_TO_MIDI_X(param[kCCin]) == -1)
                {
                    strcpy(text, "?");
                }
                else if (FLOAT_TO_MIDI_X(param[kCCin]) == 0)
                {
                    strcpy(text, "0x0000");
                }
                else
                {
                    sprintf(text, "%#.4x", (FLOAT_TO_MIDI_X(param[kCCin])) << 7);
                }
            }
            else
            {
                strcpy(text, " ");
            }
            break;
        case kNRPNin:
            if (FLOAT_TO_MIDI_X(param[kCCin]) == -1)
            {
                strcpy(text, " ");
            }
            else if (inmode == NRPN || inmode == NRPNlsb)
            {
                if (((FLOAT_TO_MIDI(param[kNRPNin])) | ((FLOAT_TO_MIDI_X(param[kCCin])) << 7)) == 0)
                {
                    strcpy(text, "NRPN 0x0000");
                }
                else
                {
                    sprintf(text, "NRPN %#.4x", (FLOAT_TO_MIDI(param[kNRPNin])) | ((FLOAT_TO_MIDI_X(param[kCCin])) << 7));
                }
            }
            else if (inmode == RPN || inmode == RPNlsb)
            {
                if ((FLOAT_TO_MIDI(param[kNRPNin]) | ((FLOAT_TO_MIDI_X(param[kCCin])) << 7)) == 0)
                {
                    strcpy(text, "RPN 0x0000");
                }
                else
                {
                    sprintf(text, "RPN %#.4x", (FLOAT_TO_MIDI(param[kNRPNin])) | ((FLOAT_TO_MIDI_X(param[kCCin])) << 7));
                }
            }
            else
            {
                strcpy(text, " ");
            }
            break;
        case kChin:
            if (inmode == clock || inmode == songposition || inmode == songselect)
            {
                strcpy(text, " ");
            }
            else
            {
                if (FLOAT_TO_CHANNEL(param[kChin]) == -1)
                {
                    strcpy(text, "Any");
                }
                else
                {
                    sprintf(text, "Channel %d", FLOAT_TO_CHANNEL(param[kChin]) + 1);
                }
                break;
            }
        case kLowLimit:
            if (inmode == pb)
            {
                sprintf(text, "%d", (FLOAT_TO_MIDI(param[kLowLimit]) << 7) - 0x2000);
            }
            else if (inmode == nonn || inmode == noffn)
            {
                sprintf(text, "%d (%s)", FLOAT_TO_MIDI(param[kLowLimit]), getNoteName(FLOAT_TO_MIDI(param[kLowLimit]), bottomOctave));
            }
            else if (inmode == NRPN || inmode == RPN || inmode == cc14)
            {
                sprintf(text, "%d", (FLOAT_TO_MIDI(param[kLowLimit]) << 7) & 0x3f80 | FLOAT_TO_MIDI(param[kLowLimit]) & 0x007f);
            }
            else if (inmode == clock || inmode == pcinc || inmode == pcdec)
            {
                strcpy(text, " ");
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_MIDI(param[kLowLimit]));
            }
            break;
        case kHighLimit:
            if (inmode == pb)
            {
                sprintf(text, "%d", (FLOAT_TO_MIDI(param[kHighLimit]) << 7) + FLOAT_TO_MIDI(param[kHighLimit]) - 0x2000);
            }
            else if (inmode == nonn || inmode == noffn)
            {
                sprintf(text, "%d (%s)", FLOAT_TO_MIDI(param[kHighLimit]), getNoteName(FLOAT_TO_MIDI(param[kHighLimit]), bottomOctave));
            }
            else if (inmode == NRPN || inmode == RPN || inmode == cc14)
            {
                sprintf(text, "%d", (FLOAT_TO_MIDI(param[kHighLimit]) << 7) & 0x3f80 | FLOAT_TO_MIDI(param[kHighLimit]) & 0x007f);
            }
            else if (inmode == clock || inmode == pcinc || inmode == pcdec)
            {
                strcpy(text, " ");
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_MIDI(param[kHighLimit]));
            }
            break;
        case kLow:
            if (outmode == pb)
            {
                sprintf(text, "%d", (FLOAT_TO_MIDI(param[kLow]) << 7) - 8192);
            }
            else if (outmode == NRPN || outmode == RPN || outmode == cc14)
            {
                sprintf(text, "%d", (FLOAT_TO_MIDI(param[kLow]) << 7));
            }
            else if (outmode == clock)
            {
                strcpy(text, " ");
            }
            else if (outmode == nonn || outmode == noffn)
            {
                sprintf(text, "%d (%s)", FLOAT_TO_MIDI(param[kLow]), getNoteName(FLOAT_TO_MIDI(param[kLow]), bottomOctave));
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_MIDI(param[kLow]));
            }
            break;
        case kHigh:
            if (outmode == pb)
            {
                sprintf(text, "%d", (FLOAT_TO_MIDI(param[kHigh]) << 7) + FLOAT_TO_MIDI(param[kHigh]) - 8192);
            }
            else if (outmode == NRPN || outmode == RPN || outmode == cc14)
            {
                sprintf(text, "%d", (FLOAT_TO_MIDI(param[kHigh]) << 7) & 0x3f80 | FLOAT_TO_MIDI(param[kHigh]) & 0x007f);
            }
            else if (outmode == clock)
            {
                strcpy(text, " ");
            }
            else if (outmode == nonn || outmode == noffn)
            {
                sprintf(text, "%d (%s)", FLOAT_TO_MIDI(param[kHigh]), getNoteName(FLOAT_TO_MIDI(param[kHigh]), bottomOctave));
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_MIDI(param[kHigh]));
            }
            break;
        case kRangeMode:
            if (param[kRangeMode] < 0.3f)
            {
                strcpy(text, "Clip/Stretch/Limit");
            }
            else if (param[kRangeMode] < 0.5)
            {
                strcpy(text, "Clip/Stretch");
            }
            else if (param[kRangeMode] < 0.8)
            {
                strcpy(text, "Stretch/Stretch");
            }
            else
            {
                strcpy(text, "Clip/Limit");
            }
            break;
        case kOffset:
            if (outmode == clock)
            {
                strcpy(text, " ");
            }
            else
            {
                sprintf(text, "%d", (signed int) FLOAT_TO_MIDI(param[kOffset]) - 63);
            }
            break;
        case kOut:
            switch (outmode)
            {
                case drop:
                    strcpy(text, "Discard");
                    break;
                case cc:
                    strcpy(text, "CC");
                    break;
                case cc14:
                    strcpy(text, "14-bit CC");
                    break;
                case pc:
                    strcpy(text, "Program Change");
                    break;
                case pcinc:
                    strcpy(text, "ProgChg Inc");
                    break;
                case pcdec:
                    strcpy(text, "ProgChg Dec");
                    break;
                case cp:
                    strcpy(text, "Channel Pressure");
                    break;
                case pa:
                    strcpy(text, "Poly Aftertouch");
                    break;
                case pb:
                    strcpy(text, "Pitch Bend");
                    break;
                case pblsb:
                    strcpy(text, "Pitch Bend (LSB)");
                    break;
                case NRPN:
                    strcpy(text, "NRPN");
                    break;
                case NRPNlsb:
                    strcpy(text, "NRPN (LSB)");
                    break;
                case RPN:
                    strcpy(text, "RPN");
                    break;
                case RPNlsb:
                    strcpy(text, "RPN (LSB)");
                    break;
                case nonn:
                    strcpy(text, "Note On #");
                    break;
                case nonv:
                    strcpy(text, "Note On Velocity");
                    break;
                case noffn:
                    strcpy(text, "Note Off #");
                    break;
                case noffv:
                    strcpy(text, "Note Off Velocity");
                    break;
                case clock:
                    strcpy(text, "MIDI Clock");
                    break;
                default:
                    strcpy(text, "???");
                    break;
            }
            break;
        case kCCout:
            switch (outmode)
            {
                case cc:
                    if (FLOAT_TO_MIDI_X(param[kCCout]) == -1)
                    {
                        strcpy(text, "As Input");
                    }
                    else
                    {
                        sprintf(text, "CC %d", FLOAT_TO_MIDI_X(param[kCCout]));
                    }
                    break;
                case cc14:
                    if (FLOAT_TO_MIDI_X(param[kCCout]) == -1)
                    {
                        strcpy(text, "As Input");
                    }
                    else if (FLOAT_TO_MIDI_X(param[kCCout]) < 32)
                    {
                        sprintf(text, "CC %d / %d", FLOAT_TO_MIDI_X(param[kCCout]), FLOAT_TO_MIDI_X(param[kCCout]) + 32);
                    }
                    else
                    {
                        sprintf(text, "CC %d", FLOAT_TO_MIDI_X(param[kCCout]));
                    }
                    break;
                case pa:
                case nonv:
                case noffv:
                    if (FLOAT_TO_MIDI_X(param[kCCout]) == -1)
                    {
                        strcpy(text, "As Input");
                    }
                    else
                    {
                        sprintf(text, "Note %d (%s)", FLOAT_TO_MIDI_X(param[kCCout]), getNoteName(FLOAT_TO_MIDI_X(param[kCCout]), bottomOctave));
                    }
                    break;
                case pb:
                    if (param[kCCout] < 0.5f)
                    {
                        strcpy(text, "Rough");
                    }
                    else
                    {
                        strcpy(text, "Smooth");
                    }
                    break;
                case pblsb:
                    if (FLOAT_TO_MIDI_X(param[kCCout]) == -1)
                    {
                        strcpy(text, "As Input");
                    }
                    else
                    {
                        sprintf(text, "MSB: %d", (FLOAT_TO_MIDI_X(param[kCCout]) << 7) - 0x2000);
                    }
                    break;
                case NRPN:
                case NRPNlsb:
                case RPN:
                case RPNlsb:
                    if (FLOAT_TO_MIDI_X(param[kCCout]) == -1)
                    {
                        strcpy(text, "As Input");
                    }
                    else if (FLOAT_TO_MIDI_X(param[kCCout]) == 0)
                    {
                        strcpy(text, "0x0000");
                    }
                    else
                    {
                        sprintf(text, "%#.4x", (FLOAT_TO_MIDI_X(param[kCCout])) << 7);
                    }
                    break;
                case nonn:
                case noffn:
                    if (FLOAT_TO_MIDI_X(param[kCCout]) == -1)
                    {
                        strcpy(text, "As Input");
                    }
                    else
                    {
                        sprintf(text, "Vel %d", FLOAT_TO_MIDI_X(param[kCCout]));
                    }
                    break;
                default:
                    strcpy(text, " ");
                    break;
            }
            break;
        case kNRPNout:
            if (outmode == pb && param[kCCout] >= 0.5f)
            {
                sprintf(text, "Inertia: %d", roundToInt(param[kNRPNout] * 100.f));
            }
            else if ((outmode == cc || outmode == cc14) && FLOAT_TO_MIDI_X(param[kCCout]) != -1)
            {
                sprintf(text, "Inertia: %d", roundToInt(param[kNRPNout] * 100.f));
            }
            else if (FLOAT_TO_MIDI_X(param[kCCout]) == -1)
            {
                strcpy(text, " ");
            }
            else if (outmode == NRPN || outmode == NRPNlsb)
            {
                if ((FLOAT_TO_MIDI(param[kNRPNout]) | ((FLOAT_TO_MIDI_X(param[kCCout])) << 7)) == 0)
                {
                    strcpy(text, "NRPN 0x0000");
                }
                else
                {
                    sprintf(text, "NRPN %#.4x", FLOAT_TO_MIDI(param[kNRPNout]) | ((FLOAT_TO_MIDI_X(param[kCCout])) << 7));
                }
            }
            else if (outmode == RPN || outmode == RPNlsb)
            {
                if ((FLOAT_TO_MIDI(param[kNRPNout]) | ((FLOAT_TO_MIDI_X(param[kCCout])) << 7)) == 0)
                {
                    strcpy(text, "RPN 0x0000");
                }
                else
                {
                    sprintf(text, "RPN %#.4x", FLOAT_TO_MIDI(param[kNRPNout]) | ((FLOAT_TO_MIDI_X(param[kCCout])) << 7));
                }
            }
            else if (outmode == nonn)
            {
                if (FLOAT_TO_MIDI(param[kNRPNout]) == 0)
                {
                    strcpy(text, "NoteOn only");
                }
                else
                {
                    strcpy(text, "Add NoteOff");
                }
            }
            else
            {
                strcpy(text, " ");
            }
            break;
        case kChout:
            if (outmode == drop || outmode == clock || outmode == songposition || outmode == songselect)
            {
                strcpy(text, " ");
            }
            else
            {
                if (FLOAT_TO_CHANNEL(param[kChout]) == -1)
                {
                    strcpy(text, "No Change");
                }
                else
                {
                    sprintf(text, "Channel %d", FLOAT_TO_CHANNEL(param[kChout]) + 1);
                }
            }
            break;
        case kThru:
            if (param[kThru] == 0.f)
            {
                strcpy(text, "Block All");
            }
            else if (param[kThru] < 0.5f)
            {
                strcpy(text, "Block Converted");
            }
            else if (param[kThru] < 1.0f)
            {
                strcpy(text, "Thru All");
            }
            else
            {
                strcpy(text, "Converted Only");
            }
            break;
        default:
            break;
    }
}

void MidiConverter::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];
        short status       = tomod.midiData[0] & 0xf0; // scraping  channel
        short channel      = tomod.midiData[0] & 0x0f; // isolating channel
        short data1        = tomod.midiData[1] & 0x7f;
        short data2        = tomod.midiData[2] & 0x7f;

        if (status == MIDI_NOTEON && data2 == 0)
        {
            status = MIDI_NOTEOFF;
        }

        int incc1  = FLOAT_TO_MIDI_X(param[kCCin]);  //-1 == any
        int outcc1 = FLOAT_TO_MIDI_X(param[kCCout]); //-1 == no change

        int lolimit1 = FLOAT_TO_MIDI(param[kLowLimit]);
        int hilimit1 = FLOAT_TO_MIDI(param[kHighLimit]);

        int low1  = FLOAT_TO_MIDI(param[kLow]);
        int high1 = FLOAT_TO_MIDI(param[kHigh]);

        int offset1 = FLOAT_TO_MIDI(param[kOffset]) - 63;

        int chin1  = FLOAT_TO_CHANNEL(param[kChin]);
        int chout1 = FLOAT_TO_CHANNEL(param[kChout]);

        int inputnrpn1  = FLOAT_TO_MIDI(param[kNRPNin]) | (incc1 << 7);
        int outputnrpn1 = FLOAT_TO_MIDI(param[kNRPNout]) | (outcc1 << 7);

        if (chout1 == -1)
        {
            chout1 = channel;
        }
        if (chin1 == -1)
        {
            chin1 = channel;
        }
        if (incc1 == -1)
        {
            incc1 = data1;
        }
        if (outcc1 == -1)
        {
            outcc1 = data1;
        }
        bool discard = false;

        int idata    = data2;
        int odata    = data2;
        bool sendout = false;
        int send     = 1;

        int idata14    = data1 | (data2 << 7);
        int odata14    = idata14;
        int olsb       = 0;
        bool inis14bit = false;
        bool inisRPN   = false;
        bool inisNRPN  = false;
        bool proginc   = false;
        bool progdec   = false;

        //pre-processing:
        //keep track of all (N)RPNs
        if (inisNRPN || inisRPN)
        {
            send              = 0;
            const int ncoarse = inisNRPN ? 99 : 101;
            const int nfine   = inisNRPN ? 98 : 100;
            int* c            = inisNRPN ? nrpncoarse : rpncoarse;
            int* n            = inisNRPN ? nrpn : rpn;
            // only send nprn stuff through if "thru" is on
            if (param[kThru] < 0.5 && (data1 == nfine || data1 == ncoarse || data1 == 6 || data1 == 38 || data1 == 96 || data1 == 97))
            {
                discard = true;
            }
            if (data1 == ncoarse)
            {
                c[channel] = data2;
            }
            else if (data1 == nfine && c[channel] >= 0)
            {
                n[channel] = data2 | (c[channel] << 7);
            }
            if (n[channel] == inputnrpn1)
            {
                if (data1 == 6)
                { //data entry slider (coarse)
                    datacoarse[channel] = data2;
                    data[channel]       = datafine[channel] | (datacoarse[channel] << 7);
                    send                = 1;
                }
                else if (data1 == 38)
                { //data entry slider (fine)
                    datafine[channel] = data2;
                    data[channel]     = datafine[channel] | (datacoarse[channel] << 7);
                    send              = 2;
                }
                else if (data1 == 96)
                { //data increment button
                    if (outmode == cc)
                    {
                        datacoarse[channel] += 1;
                        if (datacoarse[channel] > 127)
                        {
                            datacoarse[channel] = 127;
                        }
                        data[channel] = datafine[channel] | (datacoarse[channel] << 7);
                        send          = 1;
                    }
                    else
                    {
                        data[channel] += 1;
                        if (data[channel] > 127)
                        {
                            data[channel] = 127;
                        }
                        datacoarse[channel] = (data[channel] & 0x3f80) >> 7;
                        datafine[channel]   = data[channel] & 0x007f;
                        send                = 2;
                    }
                }
                else if (data1 == 97)
                { //data decrement button
                    if (outmode == cc)
                    {
                        datacoarse[channel] -= 1;
                        if (datacoarse[channel] < 0)
                        {
                            datacoarse[channel] = 0;
                        }
                        data[channel] = datafine[channel] | (datacoarse[channel] << 7);
                        send          = 1;
                    }
                    else
                    {
                        data[channel] -= 1;
                        if (data[channel] < 0)
                        {
                            data[channel] = 0;
                        }
                        datacoarse[channel] = (data[channel] & 0x3f80) >> 7;
                        datafine[channel]   = data[channel] & 0x007f;
                        send                = 2;
                    }
                }
                idata14 = data[channel];
                if ((idata14 >= (lolimit1 << 7) && idata14 <= (hilimit1 | (hilimit1 << 7))) || param[kRangeMode] >= 0.5f && param[kRangeMode] < 0.8f)
                {
                    if (lolimit1 == hilimit1)
                    {
                        odata14 = (low1 | (low1 << 7) + high1 | (high1 << 7)) / 2 + offset1 | (offset1 << 7);
                    }
                    else if (param[kRangeMode] < 0.3f)
                    {
                        odata14 = MAP_TO_MIDI(idata14, low1 | (low1 << 7), high1 | (high1 << 7), 0, 16383) + offset1 | (offset1 << 7);
                    }
                    else if (param[kRangeMode] < 0.5f)
                    {
                        odata14 = MAP_TO_MIDI(idata14, low1 | (low1 << 7), high1 | (high1 << 7), lolimit1 | (lolimit1 << 7), hilimit1 | (hilimit1 << 7)) + offset1 | (offset1 << 7);
                    }
                    else if (param[kRangeMode] < 0.8f)
                    {
                        odata14 = MAP_TO_MIDI(idata14, 0, 16383, low1 | (low1 << 7), high1 | (high1 << 7));
                        odata14 = MAP_TO_MIDI(odata14, low1 | (low1 << 7), high1 | (high1 << 7), lolimit1 | (lolimit1 << 7), hilimit1 | (hilimit1 << 7)) + offset1 | (offset1 << 7);
                    }
                    if (odata14 > 16383)
                    {
                        odata14 = 16383;
                    }
                    else if (odata14 < 0)
                    {
                        odata14 = 0;
                    }
                    odata = (odata14 & 0x3f80) >> 7;
                    //odata = roundToInt((double)odata14*0.0077519379844961240310077519379845);
                    olsb      = odata14 & 0x007f;
                    inis14bit = true;
                }
            }
            if (inmode == NRPNlsb || inmode == RPNlsb)
            {
                idata = roundToInt((double) (data[channel]) * 0.0077519379844961240310077519379845);
            }
        }
        //keep track of program changes
        else if (status == MIDI_PROGRAMCHANGE)
        {
            if ((lastPC[channel] == 127 && data1 == 0) || (data1 == lastPC[channel] + 1))
            {
                if (channel == chin1)
                {
                    proginc = true;
                }
            }
            else if ((lastPC[channel] == 0 && data1 == 127) || (data1 == lastPC[channel] - 1))
            {
                if (channel == chin1)
                {
                    progdec = true;
                }
            }
            lastPC[channel] = data1;
        }

        //------------------------------------------------------------------------------
        if (((inmode == cc || inmode == cc14 || inmode == NRPN || inmode == NRPNlsb || inmode == RPN || inmode == RPNlsb) && status == MIDI_CONTROLCHANGE)
            || ((inmode == pc || inmode == pcinc || inmode == pcdec) && status == MIDI_PROGRAMCHANGE)
            || (inmode == cp && status == MIDI_CHANNELPRESSURE)
            || (inmode == pa && status == MIDI_POLYKEYPRESSURE)
            || ((inmode == pblsb || inmode == pb) && status == MIDI_PITCHBEND)
            || (inmode == nonn && status == MIDI_NOTEON)
            || (inmode == nonv && (status == MIDI_NOTEON || status == MIDI_NOTEOFF))
            || ((inmode == noffn || inmode == noffv) && status == MIDI_NOTEOFF)
            || (inmode == clock && status == MIDI_TIMINGCLOCK))
        {
            switch (inmode)
            {
                case pc:
                case pcinc:
                case pcdec:
                case cp:
                case pblsb:
                case noffn:
                    incc1 = data1;
                    idata = data1;
                    odata = data1;
                    break;
                case nonn:
                    idata = data1;
                    break;
                case nonv:
                    if (status == MIDI_NOTEOFF)
                    {
                        data2 = 0;
                    }
                    break;
                case NRPN:
                    inisNRPN = true;
                    break;
                case NRPNlsb:
                    inisNRPN = true;
                    incc1    = data1;
                    break;
                case RPN:
                    inisRPN = true;
                    break;
                case RPNlsb:
                    inisRPN = true;
                    incc1   = data1;
                    break;
                case clock:
                case songposition:
                case songselect:
                    chin1 = channel;
                    break;

                default:
                    break;
            }

            if (channel == chin1)
            {
                if (inmode == pb)
                {
                    if ((idata14 >= (lolimit1 << 7) && idata14 <= (hilimit1 | (hilimit1 << 7))) || param[kRangeMode] >= 0.5f && param[kRangeMode] < 0.8f)
                    {
                        odata14 = idata14;
                        if (lolimit1 == hilimit1)
                        {
                            odata14 = (low1 | (low1 << 7) + high1 | (high1 << 7)) / 2 + offset1 | (offset1 << 7);
                        }
                        else if (param[kRangeMode] < 0.3f)
                        {
                            //stretch clipped input to full range
                            odata14 = MAP_TO_MIDI(idata14, 0, 16383, lolimit1 | (lolimit1 << 7), hilimit1 | (hilimit1 << 7)) + offset1 | (offset1 << 7);
                            //then limit output
                            if (odata14 > (high1 | (high1 << 7)))
                            {
                                odata14 = (high1 | (high1 << 7));
                            }
                            else if (odata14 < (low1 | (low1 << 7)))
                            {
                                odata14 = (low1 | (low1 << 7));
                            }
                        }
                        else if (param[kRangeMode] < 0.5f)
                        {
                            odata14 = MAP_TO_MIDI(idata14, low1 | (low1 << 7), high1 | (high1 << 7), lolimit1 | (lolimit1 << 7), hilimit1 | (hilimit1 << 7)) + offset1 | (offset1 << 7);
                        }
                        else if (param[kRangeMode] < 0.8f)
                        {
                            odata14 = MAP_TO_MIDI(idata14, 0, 16383, low1 | (low1 << 7), high1 | (high1 << 7));
                            odata14 = MAP_TO_MIDI(idata14, low1 | (low1 << 7), high1 | (high1 << 7), lolimit1 | (lolimit1 << 7), hilimit1 | (hilimit1 << 7)) + offset1 | (offset1 << 7);
                        }
                        else
                        {
                            if (odata14 > (high1 | (high1 << 7)))
                            {
                                odata14 = (high1 | (high1 << 7));
                            }
                            else if (odata14 < (low1 | (low1 << 7)))
                            {
                                odata14 = (low1 | (low1 << 7));
                            }
                        }
                        if (odata14 > 16383)
                        {
                            odata14 = 16383;
                        }
                        else if (odata14 < 0)
                        {
                            odata14 = 0;
                        }
                        odata     = (odata14 & 0x3f80) >> 7;
                        olsb      = odata14 & 0x007f;
                        inis14bit = true;
                        // send old message if "thru" is on
                        if (channel == chout1 && outmode == pb)
                        {
                            discard = true;
                        }
                        sendout = true;
                    }
                    else if (param[kThru] < 0.5f)
                    {
                        discard = true; //outside range
                    }
                }
                else if (inmode == pcinc && proginc)
                {
                    sendout = true;
                }
                else if (inmode == pcdec && progdec)
                {
                    sendout = true;
                }
                else if (inmode == cc14 && incc1 < 32 && (data1 == incc1 || data1 == (incc1 + 32)))
                {
                    int finecc = incc1 + 32;
                    if (param[kThru] < 0.5)
                    {
                        discard = true;
                    }
                    if (data1 == incc1)
                    {
                        cc14msb[incc1][channel] = data2;
                    }
                    else if (data1 == finecc)
                    {
                        cc14lsb[incc1][channel] = data2;
                    }

                    idata14 = (cc14lsb[incc1][channel]) | (cc14msb[incc1][channel] << 7);
                    if ((idata14 >= (lolimit1 << 7) && idata14 <= (hilimit1 | (hilimit1 << 7))) || param[kRangeMode] >= 0.5f && param[kRangeMode] < 0.8f)
                    {
                        if (lolimit1 == hilimit1)
                        {
                            odata14 = (low1 | (low1 << 7) + high1 | (high1 << 7)) / 2 + offset1 | (offset1 << 7);
                        }
                        else if (param[kRangeMode] < 0.3f)
                        {
                            //stretch clipped input to full range
                            odata14 = MAP_TO_MIDI(idata14, 0, 16383, lolimit1 | (lolimit1 << 7), hilimit1 | (hilimit1 << 7)) + offset1 | (offset1 << 7);
                            //then limit output
                            if (odata14 > (high1 | (high1 << 7)))
                            {
                                odata14 = (high1 | (high1 << 7));
                            }
                            else if (odata14 < (low1 | (low1 << 7)))
                            {
                                odata14 = (low1 | (low1 << 7));
                            }
                        }
                        else if (param[kRangeMode] < 0.5f)
                        {
                            odata14 = MAP_TO_MIDI(idata14, low1 | (low1 << 7), high1 | (high1 << 7), lolimit1 | (lolimit1 << 7), hilimit1 | (hilimit1 << 7)) + offset1 | (offset1 << 7);
                        }
                        else if (param[kRangeMode] < 0.8f)
                        {
                            odata14 = MAP_TO_MIDI(idata14, 0, 16383, low1 | (low1 << 7), high1 | (high1 << 7));
                            odata14 = MAP_TO_MIDI(odata14, low1 | (low1 << 7), high1 | (high1 << 7), lolimit1 | (lolimit1 << 7), hilimit1 | (hilimit1 << 7)) + offset1 | (offset1 << 7);
                        }
                        if (odata14 > 16383)
                        {
                            odata14 = 16383;
                        }
                        else if (odata14 < 0)
                        {
                            odata14 = 0;
                        }
                        odata = (odata14 & 0x3f80) >> 7;
                        //odata = roundToInt((double)odata14*0.0077519379844961240310077519379845);
                        olsb      = odata14 & 0x007f;
                        inis14bit = true;
                        sendout   = true;
                    }
                    else if (param[kThru] < 0.5f)
                    {
                        discard = true; //outside range
                    }
                }
                else if (inmode == clock)
                {
                    sendout = true;
                }
                else if (data1 == incc1)
                {
                    //clip input here, unless stretching input to fit in input range
                    if ((idata >= lolimit1 && idata <= hilimit1) || param[kRangeMode] >= 0.5f && param[kRangeMode] < 0.8f)
                    {
                        if (inmode == NRPNlsb)
                        {
                            idata = data1;
                        }
                        if (lolimit1 == hilimit1)
                        {
                            odata = (low1 + high1) / 2 + offset1;
                        }
                        else if (param[kRangeMode] < 0.3f) //clip/limit
                        {
                            //stretch clipped input to full range
                            odata = MAP_TO_MIDI(idata, 0, 127, lolimit1, hilimit1) + offset1;
                            //then limit output
                            if (odata > high1)
                            {
                                odata = high1;
                            }
                            else if (odata < low1)
                            {
                                odata = low1;
                            }
                        }
                        else if (param[kRangeMode] < 0.5f) //clip/stretch
                        {
                            //stretch clipped input to output range
                            odata = MAP_TO_MIDI(idata, low1, high1, lolimit1, hilimit1) + offset1;
                        }
                        else if (param[kRangeMode] < 0.8f)
                        { //stretch/stretch
                            //stretch 0-127 to input range
                            odata = MAP_TO_MIDI(idata, lolimit1, hilimit1, 0, 127);
                            //then stretch input range to output range
                            odata = MAP_TO_MIDI(odata, low1, high1, lolimit1, hilimit1) + offset1;
                        }
                        else
                        {
                            if (odata > high1)
                            {
                                odata = high1;
                            }
                            else if (odata < low1)
                            {
                                odata = low1;
                            }
                        }
                        if (odata > 127)
                        {
                            odata = 127;
                        }
                        else if (odata < 0)
                        {
                            odata = 0;
                        }
                        if (inmode == nonv && status == MIDI_NOTEOFF)
                        {
                            odata = 0;
                        }
                        // send old message if "thru" is on
                        switch (inmode)
                        {
                            case cc:
                            case pa:
                                if (channel == chout1 && data1 == outcc1 && outmode == inmode)
                                {
                                    discard = true;
                                }
                                break;
                            case pc:
                            case cp:
                                if (channel == chout1 && outmode == inmode)
                                {
                                    discard = true;
                                }
                                break;
                            case pblsb:
                                if (channel == chout1 && (outmode == pb || outmode == pblsb))
                                {
                                    discard = true;
                                }
                                break;
                            case NRPNlsb:
                                if (channel == chout1 && (outmode == NRPN || outmode == NRPNlsb))
                                {
                                    discard = true;
                                }
                                break;
                            default:
                                break;
                        }
                        //create new message
                        sendout = true;
                    }
                    else if (param[kThru] < 0.5f)
                    {
                        discard = true; //outside range
                    }
                }
            }
        }

        if (sendout)
        {
            // create new message
            VstMidiEvent outmsg = tomod;
            if (outmode == drop)
            {
                sendout = false;
            }
            if (outmode == cc)
            {
                if (roundToInt(param[kNRPNout] * 100.f) == 0 || (FLOAT_TO_MIDI_X(param[kCCout]) == -1))
                {
                    outmsg.midiData[0] = MIDI_CONTROLCHANGE | chout1;
                    outmsg.midiData[1] = outcc1;
                    outmsg.midiData[2] = odata;
                }
                else
                {
                    targetcc         = odata;
                    sendout          = false;
                    smoothcc[chout1] = outcc1;
                }
            }
            if (outmode == cc14)
            {
                if (roundToInt(param[kNRPNout] * 100.f) == 0)
                {
                    if (outcc1 < 32)
                    {
                        outmsg.midiData[0] = MIDI_CONTROLCHANGE | chout1;
                        outmsg.midiData[1] = outcc1;
                        outmsg.midiData[2] = odata;
                        outputs[0].push_back(outmsg);
                        outmsg.midiData[1] = outcc1 + 32;
                        if (! inis14bit)
                        {
                            if (odata > 64)
                            {
                                olsb = odata;
                            }
                            else
                            {
                                olsb = 0;
                            }
                        }
                        outmsg.midiData[2] = olsb;
                        outputs[0].push_back(outmsg);
                        sendout = false;
                    }
                    else
                    {
                        outmsg.midiData[0] = MIDI_CONTROLCHANGE | chout1;
                        outmsg.midiData[1] = outcc1;
                        outmsg.midiData[2] = odata;
                    }
                }
                else
                {
                    targetcc         = olsb | (odata << 7);
                    sendout          = false;
                    smoothcc[chout1] = outcc1 + 1000;
                }
            }
            else if (outmode == pc)
            { //program change
                outmsg.midiData[0] = MIDI_PROGRAMCHANGE | chout1;
                outmsg.midiData[1] = odata;
                outmsg.midiData[2] = 0;
            }
            else if (outmode == pc)
            { //program change
                outmsg.midiData[0] = MIDI_PROGRAMCHANGE | chout1;
                outmsg.midiData[1] = odata;
                outmsg.midiData[2] = 0;
            }
            else if (outmode == pcinc)
            { //program change++
                odata = lastSentPC[chout1] + 1;
                if (odata > high1)
                {
                    odata = low1;
                }
                outmsg.midiData[0] = MIDI_PROGRAMCHANGE | chout1;
                outmsg.midiData[1] = odata;
                outmsg.midiData[2] = 0;
            }
            else if (outmode == pcdec)
            { //program change--
                odata = lastSentPC[chout1] - 1;
                if (odata < low1)
                {
                    odata = high1;
                }
                outmsg.midiData[0] = MIDI_PROGRAMCHANGE | chout1;
                outmsg.midiData[1] = odata;
                outmsg.midiData[2] = 0;
            }
            else if (outmode == cp)
            { //channel pressure
                outmsg.midiData[0] = MIDI_CHANNELPRESSURE | chout1;
                outmsg.midiData[1] = odata;
                outmsg.midiData[2] = 0;
            }
            else if (outmode == pa)
            { //aftertouch
                outmsg.midiData[0] = MIDI_POLYKEYPRESSURE | chout1;
                outmsg.midiData[1] = outcc1;
                outmsg.midiData[2] = odata;
            }
            else if (outmode == pb)
            { //pitch bend
                if (! inis14bit)
                {
                    if (odata > 64)
                    {
                        olsb = odata;
                    }
                    else
                    {
                        olsb = 0;
                    }
                }
                if (param[kCCout] < 0.5f)
                {
                    outmsg.midiData[0] = MIDI_PITCHBEND | chout1;
                    outmsg.midiData[1] = olsb;
                    outmsg.midiData[2] = odata;
                }
                else
                {
                    targetpb     = olsb | (odata << 7);
                    sendout      = false;
                    done[chout1] = false;
                }
            }
            else if (outmode == pblsb)
            { //pitch bend (LSB)
                outmsg.midiData[0] = MIDI_PITCHBEND | chout1;
                outmsg.midiData[1] = odata;
                outmsg.midiData[2] = outcc1;
            }
            else if (outmode == NRPN)
            { //NRPN
                if (outcc1 >= 0 && send > 0)
                {
                    VstMidiEvent ncoarse = inputs[0][i];
                    ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | chout1;
                    ncoarse.midiData[1]  = 99;
                    ncoarse.midiData[2]  = outcc1;
                    outputs[0].push_back(ncoarse);

                    VstMidiEvent nfine = inputs[0][i];
                    nfine.midiData[0]  = MIDI_CONTROLCHANGE | chout1;
                    nfine.midiData[1]  = 98;
                    nfine.midiData[2]  = outputnrpn1;
                    outputs[0].push_back(nfine);

                    VstMidiEvent dcoarse = inputs[0][i];
                    dcoarse.midiData[0]  = MIDI_CONTROLCHANGE | chout1;
                    dcoarse.midiData[1]  = 6;
                    dcoarse.midiData[2]  = odata;
                    outputs[0].push_back(dcoarse);

                    VstMidiEvent dfine = inputs[0][i];
                    dfine.midiData[0]  = MIDI_CONTROLCHANGE | chout1;
                    dfine.midiData[1]  = 38;
                    if (! inis14bit)
                    {
                        if (odata > 64)
                        {
                            olsb = odata;
                        }
                        else
                        {
                            olsb = 0;
                        }
                    }
                    dfine.midiData[2] = olsb;
                    outputs[0].push_back(dfine);

                    sendout = false;
                }
            }
            else if (outmode == NRPNlsb)
            { //NRPN LSB
                if (outcc1 >= 0 && send > 0)
                {
                    VstMidiEvent ncoarse = inputs[0][i];
                    ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | chout1;
                    ncoarse.midiData[1]  = 99;
                    ncoarse.midiData[2]  = outcc1;
                    outputs[0].push_back(ncoarse);

                    VstMidiEvent nfine = inputs[0][i];
                    nfine.midiData[0]  = MIDI_CONTROLCHANGE | chout1;
                    nfine.midiData[1]  = 98;
                    nfine.midiData[2]  = outputnrpn1;
                    outputs[0].push_back(nfine);

                    VstMidiEvent dfine = inputs[0][i];
                    dfine.midiData[0]  = MIDI_CONTROLCHANGE | chout1;
                    dfine.midiData[1]  = 38;
                    dfine.midiData[2]  = odata;
                    outputs[0].push_back(dfine);

                    sendout = false;
                }
            }
            else if (outmode == RPN)
            { //RPN
                if (outcc1 >= 0 && send > 0)
                {
                    VstMidiEvent ncoarse = inputs[0][i];
                    ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | chout1;
                    ncoarse.midiData[1]  = 101;
                    ncoarse.midiData[2]  = outcc1;
                    outputs[0].push_back(ncoarse);

                    VstMidiEvent nfine = inputs[0][i];
                    nfine.midiData[0]  = MIDI_CONTROLCHANGE | chout1;
                    nfine.midiData[1]  = 100;
                    nfine.midiData[2]  = outputnrpn1;
                    outputs[0].push_back(nfine);

                    VstMidiEvent dcoarse = inputs[0][i];
                    dcoarse.midiData[0]  = MIDI_CONTROLCHANGE | chout1;
                    dcoarse.midiData[1]  = 6;
                    dcoarse.midiData[2]  = odata;
                    outputs[0].push_back(dcoarse);

                    VstMidiEvent dfine = inputs[0][i];
                    dfine.midiData[0]  = MIDI_CONTROLCHANGE | chout1;
                    dfine.midiData[1]  = 38;
                    if (! inis14bit)
                    {
                        if (odata > 64)
                        {
                            olsb = odata;
                        }
                        else
                        {
                            olsb = 0;
                        }
                    }
                    dfine.midiData[2] = olsb;
                    outputs[0].push_back(dfine);

                    sendout = false;
                }
            }
            else if (outmode == RPNlsb)
            { //RPN LSB
                if (outcc1 >= 0 && send > 0)
                {
                    VstMidiEvent ncoarse = inputs[0][i];
                    ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | chout1;
                    ncoarse.midiData[1]  = 101;
                    ncoarse.midiData[2]  = outcc1;
                    outputs[0].push_back(ncoarse);

                    VstMidiEvent nfine = inputs[0][i];
                    nfine.midiData[0]  = MIDI_CONTROLCHANGE | chout1;
                    nfine.midiData[1]  = 100;
                    nfine.midiData[2]  = outputnrpn1;
                    outputs[0].push_back(nfine);

                    VstMidiEvent dfine = inputs[0][i];
                    dfine.midiData[0]  = MIDI_CONTROLCHANGE | chout1;
                    dfine.midiData[1]  = 38;
                    dfine.midiData[2]  = odata;
                    outputs[0].push_back(dfine);

                    sendout = false;
                }
            }
            else if (outmode == nonn)
            { //Note On #
                outmsg.midiData[0] = MIDI_NOTEON | chout1;
                outmsg.midiData[1] = odata;
                outmsg.midiData[2] = outcc1;
                if (FLOAT_TO_MIDI(param[kNRPNout]) > 0)
                {
                    //fix this part... queue a noteoff for later
                    VstMidiEvent noteoff = outmsg;
                    noteoff.midiData[0]  = MIDI_NOTEOFF | chout1;
                    noteoff.midiData[2]  = 0;
                    //noteoff.deltaFrames += FLOAT_TO_MIDI(param[kNRPNout]);
                    outputs[0].push_back(outmsg);
                    outputs[0].push_back(noteoff);
                    sendout = false;
                }
            }
            else if (outmode == nonv)
            { //Note On Vel
                if (status == MIDI_NOTEOFF && inmode == nonv)
                {
                    odata = 0;
                }
                outmsg.midiData[0] = MIDI_NOTEON | chout1;
                outmsg.midiData[1] = outcc1;
                outmsg.midiData[2] = odata;
            }
            else if (outmode == noffn)
            { //Note Off #
                outmsg.midiData[0] = MIDI_NOTEOFF | chout1;
                outmsg.midiData[1] = odata;
                outmsg.midiData[2] = outcc1;
            }
            else if (outmode == noffv)
            { //Note Off Vel
                outmsg.midiData[0] = MIDI_NOTEOFF | chout1;
                outmsg.midiData[1] = outcc1;
                outmsg.midiData[2] = odata;
            }
            else if (outmode == clock)
            {
                outmsg.midiData[0] = (char) MIDI_TIMINGCLOCK;
                outmsg.midiData[1] = 0;
                outmsg.midiData[2] = 0;
            }
            else if (outmode == songselect)
            {
                outmsg.midiData[0] = (char) MIDI_SONGSELECT;
                outmsg.midiData[1] = odata;
                outmsg.midiData[2] = 0;
            }
            if (sendout)
            {
                outputs[0].push_back(outmsg);
                if (outmode == pc || outmode == pcinc || outmode == pcdec)
                {
                    lastSentPC[chout1] = outmsg.midiData[1];
                }
            }

            if (param[kThru] < 0.5f)
            {
                discard = true; //block converted
            }
        }
        else if (param[kThru] == 1.0f)
        {
            discard = true; //thru converted only
        }
        if (param[kThru] == 0.f)
        {
            discard = true; //block all
        }
        //send the original message
        if (! discard)
        {
            outputs[0].push_back(tomod);
        }
    }
    if (outmode == pb)
    {
        for (int ch = 0; ch < 16; ch++)
        {
            if (! done[ch])
            { //pitch bend smoothing
                for (int j = 0; j < sampleFrames; j++)
                {
                    if (counter == 0)
                    {
                        counter = roundToInt(sampleRate * 0.002f);
                        lastpb  = smooth(targetpb, lastpb, param[kNRPNout]);
                        int pb2 = (lastpb & 0x3F80) >> 7;
                        int pb1 = (lastpb & 0x007F);
                        VstMidiEvent pb;
                        pb.deltaFrames = j;
                        pb.midiData[0] = MIDI_PITCHBEND | ch;
                        pb.midiData[1] = pb1;
                        pb.midiData[2] = pb2;
                        if (! done[ch])
                        {
                            outputs[0].push_back(pb);
                        }
                        if (lastpb == targetpb)
                        {
                            done[ch] = true;
                        }
                    }
                    counter--;
                }
            }
        }
    }
    else if (outmode == cc || outmode == cc14)
    {
        for (int ch = 0; ch < 16; ch++)
        {
            if (smoothcc[ch] > -1)
            { //cc smoothing
                bool is14bit = outmode == cc14;
                for (int j = 0; j < sampleFrames; j++)
                {
                    if (counter == 0)
                    {
                        counter = roundToInt(sampleRate * (0.005f + 0.002 * param[kNRPNout]));
                        lastcc  = smooth(targetcc, lastcc, param[kNRPNout], is14bit);
                        if (is14bit)
                        {
                            VstMidiEvent msb;
                            msb.deltaFrames = j;
                            msb.midiData[0] = MIDI_CONTROLCHANGE | ch;
                            msb.midiData[1] = smoothcc[ch] - 1000;
                            msb.midiData[2] = (lastcc & 0x3f80) >> 7;
                            VstMidiEvent lsb;
                            lsb.deltaFrames = j;
                            lsb.midiData[0] = MIDI_CONTROLCHANGE | ch;
                            lsb.midiData[1] = smoothcc[ch] - 1000 + 32;
                            lsb.midiData[2] = lastcc & 0x007f;
                            if (smoothcc[ch] > -1)
                            {
                                outputs[0].push_back(msb);
                                outputs[0].push_back(lsb);
                            }
                        }
                        else
                        {
                            VstMidiEvent cc;
                            cc.deltaFrames = j;
                            cc.midiData[0] = MIDI_CONTROLCHANGE | ch;
                            cc.midiData[1] = smoothcc[ch];
                            cc.midiData[2] = lastcc & 0x007f;
                            if (smoothcc[ch] > -1)
                            {
                                outputs[0].push_back(cc);
                            }
                        }
                        if (lastcc == targetcc)
                        {
                            smoothcc[ch] = -1;
                        }
                    }
                    counter--;
                }
            }
        }
    }
}

int MidiConverter::smooth(int newvalue, int oldvalue, float inertia, bool is14bit)
{
    float change = (float) (newvalue - oldvalue) * (1.0f - inertia * 0.95f);

    //make sure change isn't smaller than 1
    if (change < 1.0f && change > 0.0f)
    {
        change = 1.0f;
    }
    else if (change < 0.0f && change > -1.0f)
    {
        change = -1.0f;
    }

    if (change < 0.f)
    {
        newvalue = oldvalue - roundToInt(-change);
    }
    else
    {
        newvalue = oldvalue + roundToInt(change);
    }
    if (is14bit)
    {
        if (newvalue > 0x7fff)
        {
            newvalue = 0x7fff;
        }
    }
    else
    {
        if (newvalue > 0x7f)
        {
            newvalue = 0x7f;
        }
    }
    if (newvalue < 0)
    {
        newvalue = 0;
    }
    return newvalue;
}
