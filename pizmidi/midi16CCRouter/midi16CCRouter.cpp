#include "midi16CCRouter.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new Midi16CCRouter (audioMaster);
}

Midi16CCRouterProgram::Midi16CCRouterProgram()
{
    // default program values
    for (int i = 0; i < numCCs; i++)
    {
        param[i * 2]     = MIDI_TO_FLOAT (i); //in
        param[i * 2 + 1] = 0.0;               //out; 0 means no change, otherwise use MIDI_TO_FLOAT2()
    }
    param[kChi] = CHANNEL_TO_FLOAT016 (0); // any channel
    param[kCho] = CHANNEL_TO_FLOAT016 (0); // same channel as input
    param[kPC]  = 1.0f;

    // default program name
    vst_strncpy (name, "program", kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------
Midi16CCRouter::Midi16CCRouter (audioMasterCallback audioMaster)
    : PizMidi (audioMaster, kNumPrograms, kNumParams)
{
    programs  = new Midi16CCRouterProgram[numPrograms];
    automated = false;

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
        }
        else
        {
            // built-in programs
            for (int i = 0; i < kNumPrograms; i++)
            {
                sprintf (programs[i].name, "Program %d", i + 1);
            }
        }
        setProgram (0);
    }

    init();
}

//-----------------------------------------------------------------------------------------
Midi16CCRouter::~Midi16CCRouter()
{
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void Midi16CCRouter::setProgram (VstInt32 program)
{
    Midi16CCRouterProgram* ap = &programs[program];

    curProgram = program;
    if (automated)
    {
        for (int i = 0; i < kNumParams; i++)
        {
            setParameterAutomated (i, ap->param[i]);
        }
        updateDisplay();
        automated = false;
    }
    else
    {
        for (int i = 0; i < kNumParams; i++)
        {
            setParameter (i, ap->param[i]);
        }
    }
}

//------------------------------------------------------------------------
void Midi16CCRouter::setProgramName (char* name)
{
    vst_strncpy (programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void Midi16CCRouter::getProgramName (char* name)
{
    vst_strncpy (name, programs[curProgram].name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
bool Midi16CCRouter::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
    if (index < numPrograms)
    {
        vst_strncpy (text, programs[index].name, kVstMaxProgNameLen);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void Midi16CCRouter::setParameter (VstInt32 index, float value)
{
    Midi16CCRouterProgram* ap = &programs[curProgram];
    param[index] = ap->param[index] = value;
    if (index == kPC)
    {
        for (int i = 0; i < kNumPrograms; i++)
            programs[i].param[kPC] = value;
    }
}

//-----------------------------------------------------------------------------------------
float Midi16CCRouter::getParameter (VstInt32 index)
{
    return param[index];
}

//-----------------------------------------------------------------------------------------
void Midi16CCRouter::getParameterName (VstInt32 index, char* label)
{
    if (index == kChi)
        vst_strncpy (label, "Ch. In", kVstMaxParamStrLen);
    else if (index == kCho)
        vst_strncpy (label, "Ch. Out", kVstMaxParamStrLen);
    else if (index == kPC)
        sprintf (label, "PC Channel");
    else if (index % 2 == 0)
        sprintf (label, "CC In %d", index / 2 + 1);
    else if (index % 2 == 1)
        sprintf (label, "CC Out %d", index / 2 + 1);
}

//-----------------------------------------------------------------------------------------
void Midi16CCRouter::getParameterDisplay (VstInt32 index, char* text)
{
    if (index == kChi)
    {
        if (FLOAT_TO_CHANNEL016 (param[index]) < 1)
            strcpy (text, "Any");
        else
            sprintf (text, "%d", FLOAT_TO_CHANNEL016 (param[index]));
    }
    else if (index == kCho)
    {
        if (FLOAT_TO_CHANNEL016 (param[index]) < 1)
            strcpy (text, "No Change");
        else
            sprintf (text, "%d", FLOAT_TO_CHANNEL016 (param[index]));
    }
    else if (index == kPC)
    {
        if (FLOAT_TO_CHANNEL016 (param[index]) < 1)
            strcpy (text, "Any");
        else if (param[index] < 1.0f)
            sprintf (text, "%d", FLOAT_TO_CHANNEL016 (param[index]));
        else
            strcpy (text, "None");
    }
    else if (index % 2 == 0)
        sprintf (text, "%d", FLOAT_TO_MIDI (param[index]));
    else if (index % 2 == 1)
    {
        if (FLOAT_TO_MIDI2 (param[index]) == 0)
            strcpy (text, "No Change");
        else
            sprintf (text, "%d", FLOAT_TO_MIDI2 (param[index]) - 1);
    }
}

//have fun modifying this one!
void Midi16CCRouter::processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        short status  = tomod.midiData[0] & 0xf0;       // scraping  channel
        short channel = (tomod.midiData[0] & 0x0f) + 1; // isolating channel (1-16)
        short data1   = tomod.midiData[1] & 0x7f;
        //short data2		 = tomod.midiData[2] & 0x7f;

        int cci[numCCs];
        int cco[numCCs];

        bool discard = false;

        for (int i = 0; i < numCCs; i++)
        {
            cci[i] = FLOAT_TO_MIDI (param[i * 2]);
            cco[i] = FLOAT_TO_MIDI2 (param[i * 2 + 1]);
        }
        int chi = FLOAT_TO_CHANNEL016 (param[kChi]);
        int cho = FLOAT_TO_CHANNEL016 (param[kCho]);
        if (cho == 0)
            cho = channel;
        int chipc = FLOAT_TO_CHANNEL016 (param[kPC]);
        if (param[kPC] == 1.0f)
            chipc = -1;

        //only look at the selected channel
        if (channel == chipc || chipc == 0)
        {
            if (status == MIDI_PROGRAMCHANGE)
            {
                automated = true;
                setProgram (data1);
            }
        }
        if (channel == chi || chi == 0)
        {
            if (status == MIDI_CONTROLCHANGE)
            {
                for (int i = 0; i < numCCs; i++)
                {
                    if (data1 == cci[i] /* && cco[i]>0*/)
                    {
                        if (cco[i] != 0)
                            tomod.midiData[1] = cco[i] - 1;
                        tomod.midiData[0] = status | (cho - 1);
                        outputs[0].push_back (tomod);
                        discard = true;
                    }
                }
            }
        }
        if (! discard)
            outputs[0].push_back (tomod);
    }
}
