#include "midiProgramChange.hpp"

//for fmod()
#include <cmath>

//for seeding rand()
#include <time.h>

//for sprintf()
#include <string>

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiProgramChange(audioMaster);
}

MidiProgramChangeProgram::MidiProgramChangeProgram()
{
    // default Program Values
    for (int i = 0; i < kNumParams; i++)
    {
        param[i] = 0.f;
    }
    param[kMode]        = 1.f;
    param[kThru]        = 1.f;
    param[kTrigger]     = 0.4f;
    param[kBankTrigger] = 0.4f;
    param[kInc]         = 0.4f;
    param[kDec]         = 0.4f;
    // default program name
    strcpy(name, "Default");
}

//-----------------------------------------------------------------------------
MidiProgramChange::MidiProgramChange(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams), programs(0)
{
    programs = new MidiProgramChangeProgram[numPrograms];

    if (programs)
    {
        CFxBank* defaultBank = new CFxBank(kNumPrograms, kNumParams);
        if (readDefaultBank(PLUG_NAME, defaultBank))
        {
            if ((VstInt32) defaultBank->GetFxID() == PLUG_IDENT)
            {
                for (int i = 0; i < kNumPrograms; i++)
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
            for (int i = 0; i < kNumPrograms; i++)
            {
                sprintf(programs[i].name, "Program %d", i + 1);
            }
        }
        setProgram(0);
    }

    trigger     = false;
    triggerbank = false;
    inc         = false;
    dec         = false;
    program     = 0;
    bankmsb     = 0;
    banklsb     = 0;
    sentinc     = false;
    sentdec     = false;
    senttrig    = false;
    sentbank    = false;
    for (int i = 0; i < kNumParams; i++)
    {
        automated[i] = false;
    }
    mode = continuous;

    wait         = false;
    delaytime    = (int) (sampleRate * 0.002f);
    counter      = 0;
    triggerdelta = 0;

    if (programs)
    {
        setProgram(0);
    }

    init();
}

//-----------------------------------------------------------------------------------------
MidiProgramChange::~MidiProgramChange()
{
    if (programs)
    {
        delete[] programs;
    }
}

//------------------------------------------------------------------------
void MidiProgramChange::setProgram(VstInt32 prog)
{
    MidiProgramChangeProgram* ap = &programs[prog];
    curProgram                   = prog;
    for (int i = 0; i < kNumParams; i++)
    {
        setParameter(i, ap->param[i]);
    }
    if (program > 0)
    {
        trigger = true;
    }
    if (banklsb > 0 || bankmsb > 0)
    {
        triggerbank = true;
    }
}

//------------------------------------------------------------------------
void MidiProgramChange::setProgramName(char* name)
{
    vst_strncpy(programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiProgramChange::getProgramName(char* name)
{
    strcpy(name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool MidiProgramChange::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy(text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiProgramChange::setSampleRate(float sampleRateIn)
{
    PizMidi::setSampleRate(sampleRateIn);
    delaytime = (int) (sampleRate * 0.002f);
}

//-----------------------------------------------------------------------------------------
void MidiProgramChange::setParameter(VstInt32 index, float value)
{
    MidiProgramChangeProgram* ap = &programs[curProgram];
    switch (index)
    {
        case kMode:
            if (value < 0.5f)
            {
                mode = continuous;
            }
            else
            {
                mode = triggered;
            }
            param[index] = ap->param[index] = value;
            break;
        case kPCListen:
            pclisten     = value >= 0.5f;
            param[index] = ap->param[index] = value;
            break;
        case kThru:
            thru         = value >= 0.5f;
            param[index] = ap->param[index] = value;
            break;
        case kTrigger:
            param[index] = value;
            if (value == 1.f)
            { // && !senttrig) {
                trigger  = true;
                senttrig = true;
            }
            else if (value < 1.f && senttrig)
            {
                senttrig = false;
            }
            break;
        case kBankTrigger:
            param[index] = value;
            if (value == 1.f)
            { // && !sentbank) {
                triggerbank = true;
                sentbank    = true;
            }
            else if (value < 1.f && sentbank)
            {
                sentbank = false;
            }
            break;
        case kInc:
            param[index] = value;
            if (value == 1.f)
            { // && !sentinc) {
                inc     = true;
                sentinc = true;
            }
            else if (value < 1.f && sentinc)
            {
                sentinc = false;
            }
            break;
        case kDec:
            param[index] = value;
            if (value == 1.f)
            { // && !sentdec) {
                dec     = true;
                sentdec = true;
            }
            else if (value < 1.f && sentdec)
            {
                sentdec = false;
            }
            break;
        case kProgram:
            program = FLOAT_TO_MIDI2(value);
            if (mode == continuous && ! automated[index])
            {
                trigger = true;
            }
            param[index] = ap->param[index] = value;
            break;
        case kBankMSB:
            bankmsb = FLOAT_TO_MIDI2(value);
            if (mode == continuous)
            {
                triggerbank = true;
            }
            param[index] = ap->param[index] = value;
            break;
        case kBankLSB:
            banklsb = FLOAT_TO_MIDI2(value);
            if (mode == continuous)
            {
                triggerbank = true;
            }
            param[index] = ap->param[index] = value;
            break;
        default:
            param[index] = ap->param[index] = value;
            break;
    }
    if (index == kMode || index > kBankTrigger)
    {
        for (int p = 0; p < kNumPrograms; p++)
        {
            programs[p].param[index] = param[index];
        }
    }
}

//-----------------------------------------------------------------------------------------
float MidiProgramChange::getParameter(VstInt32 index)
{
    return param[index];
}

//-----------------------------------------------------------------------------------------
void MidiProgramChange::getParameterName(VstInt32 index, char* label)
{
    switch (index)
    {
        case kProgram:
            strcpy(label, "Program");
            break;
        case kBankMSB:
            strcpy(label, "Bank MSB");
            break;
        case kBankLSB:
            strcpy(label, "Bank LSB");
            break;
        case kMode:
            strcpy(label, "Mode");
            break;
        case kTrigger:
            strcpy(label, "PC Trig.");
            break;
        case kBankTrigger:
            strcpy(label, "Bank Trig.");
            break;
        case kInc:
            strcpy(label, "PC Incr.");
            break;
        case kDec:
            strcpy(label, "PC Decr.");
            break;
        case kChannel:
            strcpy(label, "Channel");
            break;
        case kPCListen:
            strcpy(label, "PC Listen");
            break;
        case kThru:
            strcpy(label, "Thru");
            break;
        default:
            sprintf(label, "param[%d]", index);
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiProgramChange::getParameterDisplay(VstInt32 index, char* text)
{
    switch (index)
    {
        case kMode:
            if (mode == continuous)
            {
                strcpy(text, "Direct");
            }
            else
            {
                strcpy(text, "Triggered");
            }
            break;
        case kPCListen:
            if (pclisten)
            {
                strcpy(text, "Yes");
            }
            else
            {
                strcpy(text, "No");
            }
            break;
        case kThru:
            if (thru)
            {
                strcpy(text, "Yes");
            }
            else
            {
                strcpy(text, "No");
            }
            break;
        case kProgram:
            if (program == 0)
            {
                strcpy(text, "Off");
            }
            else
            {
                sprintf(text, "%d", program);
            }
            break;
        case kBankMSB:
            if (bankmsb == 0)
            {
                strcpy(text, "Off");
            }
            else
            {
                sprintf(text, "%d", bankmsb);
            }
            break;
        case kBankLSB:
            if (banklsb == 0)
            {
                strcpy(text, "Off");
            }
            else
            {
                sprintf(text, "%d", banklsb);
            }
            break;
        case kChannel:
            sprintf(text, "%d", FLOAT_TO_CHANNEL015(param[index]) + 1);
            break;
        case kTrigger:
            if (param[index] == 1.f)
            {
                strcpy(text, "Triggered!");
            }
            else
            {
                strcpy(text, "Trigger-->");
            }
            break;
        case kBankTrigger:
            if (param[index] < 1.f)
            {
                strcpy(text, "Trigger-->");
            }
            else
            {
                strcpy(text, "Triggered!");
            }
            break;
        case kInc:
            if (param[index] < 1.f)
            {
                strcpy(text, "Trigger-->");
            }
            else
            {
                strcpy(text, "Triggered!");
            }
            break;
        case kDec:
            if (param[index] < 1.f)
            {
                strcpy(text, "Trigger-->");
            }
            else
            {
                strcpy(text, "Triggered!");
            }
            break;
        default:
            sprintf(text, "%d", roundToInt(param[index] * 100.0f));
            break;
    }
}

void MidiProgramChange::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 samples)
{
    char listenchannel = FLOAT_TO_CHANNEL015(param[kChannel]);

    //process incoming events-------------------------------------------------------
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        unsigned char status = tomod.midiData[0] & 0xf0; // scraping  channel
        char channel         = tomod.midiData[0] & 0x0f; // isolating channel (0-15)
        char data1           = tomod.midiData[1] & 0x7f;
        //char data2	    = tomod.midiData[2] & 0x7f;

        bool discard = ! thru;

        //only look at the selected channel
        if (channel == listenchannel)
        {
            if (status == MIDI_PROGRAMCHANGE)
            {
                if (pclisten)
                {
                    setProgram(data1);
                    updateDisplay();
                    discard = true;
                }
            }
        } // if listenchannel==channel
        if (! discard)
        {
            outputs[0].push_back(tomod);
        }
    } //for() inputs loop

    if (triggerbank)
    {
        if (! (trigger && program != 0))
        {
            triggerbank = false;
        }
        //create GUI triggered message
        if (bankmsb != 0)
        {
            VstMidiEvent msb;
            msb.midiData[0] = MIDI_CONTROLCHANGE | listenchannel;
            msb.midiData[1] = MIDI_BANK_CHANGE;
            msb.midiData[2] = bankmsb - 1;
            msb.deltaFrames = 0;
            outputs[0].push_back(msb);
        }
        if (banklsb != 0)
        {
            VstMidiEvent lsb;
            lsb.midiData[0] = MIDI_CONTROLCHANGE | listenchannel;
            lsb.midiData[1] = 0x20;
            lsb.midiData[2] = banklsb - 1;
            lsb.deltaFrames = 0;
            outputs[0].push_back(lsb);
        }
        setParameterAutomated(kBankTrigger, 0.4f);
    }
    if (trigger)
    {
        trigger = false;
        if (program != 0)
        {
            if (triggerbank)
            {
                //delay program change if bank was sent
                wait        = true;
                triggerbank = false;
                if (samples > delaytime)
                {
                    wait    = false;
                    counter = 0;
                    VstMidiEvent progch;
                    progch.midiData[0] = MIDI_PROGRAMCHANGE | listenchannel;
                    progch.midiData[1] = program - 1;
                    progch.midiData[2] = 0x0;
                    progch.deltaFrames = delaytime;
                    outputs[0].push_back(progch);
                    setParameterAutomated(kTrigger, 0.4f);
                }
            }
            else
            {
                //create GUI triggered message
                VstMidiEvent progch;
                progch.midiData[0] = MIDI_PROGRAMCHANGE | listenchannel;
                progch.midiData[1] = program - 1;
                progch.midiData[2] = 0x0;
                progch.deltaFrames = triggerdelta;
                outputs[0].push_back(progch);
                setParameterAutomated(kTrigger, 0.4f);
            }
        }
    }
    else if (inc)
    {
        //create GUI triggered message
        inc = false;
        ++program;
        if (program > 128)
        {
            program = 0;
        }
        if (program != 0)
        {
            VstMidiEvent progch;
            progch.midiData[0] = MIDI_PROGRAMCHANGE | listenchannel;
            progch.midiData[1] = program - 1;
            progch.midiData[2] = 0x0;
            progch.deltaFrames = 0;
            outputs[0].push_back(progch);
        }
        setParameterAutomated(kProgram, MIDI_TO_FLOAT2(program));
        setParameterAutomated(kInc, 0.4f);
    }
    else if (dec)
    {
        //create GUI triggered message
        dec = false;
        --program;
        if (program < 0)
        {
            program = 128;
        }
        if (program != 0)
        {
            VstMidiEvent progch;
            progch.midiData[0] = MIDI_PROGRAMCHANGE | listenchannel;
            progch.midiData[1] = program - 1;
            progch.midiData[2] = 0x0;
            progch.deltaFrames = 0;
            outputs[0].push_back(progch);
        }
        setParameterAutomated(kProgram, MIDI_TO_FLOAT2(program));
        setParameterAutomated(kDec, 0.4f);
    }
}
