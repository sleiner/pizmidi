#include "midiNRPNConverter.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiNRPNConverter(audioMaster);
}

MidiNRPNConverterProgram::MidiNRPNConverterProgram()
{
    // default Program Values
    param[kIn]      = 0.35f;                  //NRPN in
    param[kOut]     = 0.25f;                  //CC out
    param[kChin]    = CHANNEL_TO_FLOAT016(0); //Any Channel in
    param[kChout]   = CHANNEL_TO_FLOAT016(0); //same channel out
    param[kDiscard] = 0.0f;                   //don't pass on original message
    for (int i = 0; i < kNumSlots; i++)
    {
        param[i * 4 + 4] = 0.0f;               //coarse #
        param[i * 4 + 5] = MIDI_TO_FLOAT_X(0); //fine #
        param[i * 4 + 6] = 0.0f;               //coarse data
        param[i * 4 + 7] = 0.0f;               //fine data
    }
    // default program name
    strcpy(name, "Default");
}

//-----------------------------------------------------------------------------
MidiNRPNConverter::MidiNRPNConverter(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams), programs(0)
{
    programs = new MidiNRPNConverterProgram[numPrograms];

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

    for (int ch = 0; ch < 16; ch++)
    {
        nrpn[ch]       = -1;
        nrpncoarse[ch] = -1;
        rpn[ch]        = -1;
        rpncoarse[ch]  = -1;
        datafine[ch]   = 0;
        datacoarse[ch] = 0;
        data[ch]       = 0;
    }

    init();
}

//-----------------------------------------------------------------------------------------
MidiNRPNConverter::~MidiNRPNConverter()
{
    if (programs)
    {
        delete[] programs;
    }
}

//------------------------------------------------------------------------
void MidiNRPNConverter::setProgram(VstInt32 program)
{
    MidiNRPNConverterProgram* ap = &programs[program];

    curProgram = program;
    for (int i = 0; i < kNumParams; i++)
    {
        setParameter(i, ap->param[i]);
    }
}

//------------------------------------------------------------------------
void MidiNRPNConverter::setProgramName(char* name)
{
    vst_strncpy(programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiNRPNConverter::getProgramName(char* name)
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
bool MidiNRPNConverter::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy(text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiNRPNConverter::setParameter(VstInt32 index, float value)
{
    MidiNRPNConverterProgram* ap = &programs[curProgram];
    param[index] = ap->param[index] = value;
    if (index == kIn || index == kOut)
    {
        updateDisplay();
    }
}

//-----------------------------------------------------------------------------------------
float MidiNRPNConverter::getParameter(VstInt32 index)
{
    return param[index];
}

//-----------------------------------------------------------------------------------------
void MidiNRPNConverter::getParameterName(VstInt32 index, char* label)
{
    if (index == kIn)
    {
        strcpy(label, "Input");
    }
    else if (index == kOut)
    {
        strcpy(label, "Output");
    }
    else if (index == kChin)
    {
        strcpy(label, "Channel in");
    }
    else if (index == kChout)
    {
        strcpy(label, "Channel out");
    }
    else if (index == kDiscard)
    {
        strcpy(label, "Thru");
    }

    else
    {
        for (int i = 0; i < kNumSlots; i++)
        {
            if (index == i * 4 + 4)
            {
                sprintf(label, "%d:Coarse in", i + 1);
            }
            else if (index == i * 4 + 5)
            {
                sprintf(label, "%d:(N)RPN in", i + 1);
            }
            else if (index == i * 4 + 6)
            {
                sprintf(label, "%d:Coarse out", i + 1);
            }
            else if (index == i * 4 + 7)
            {
                sprintf(label, "%d:(N)RPN out", i + 1);
            }
        }
    }
}

//-----------------------------------------------------------------------------------------
void MidiNRPNConverter::getParameterDisplay(VstInt32 index, char* text)
{
    if (index == kIn)
    {
        if (param[index] < 0.2f)
        {
            strcpy(text, "CC");
        }
        else if (param[index] < 0.4f)
        {
            strcpy(text, "NRPN");
        }
        else if (param[index] < 0.6f)
        {
            strcpy(text, "NRPN (LSB)");
        }
        else if (param[index] < 0.8f)
        {
            strcpy(text, "RPN");
        }
        else
        {
            strcpy(text, "RPN (LSB)");
        }
    }
    else if (index == kOut)
    {
        if (param[index] < 0.16667f)
        {
            strcpy(text, "Discard");
        }
        else if (param[index] < 0.33333f)
        {
            strcpy(text, "CC");
        }
        else if (param[index] < 0.50000f)
        {
            strcpy(text, "NRPN");
        }
        else if (param[index] < 0.66667f)
        {
            strcpy(text, "NRPN (LSB)");
        }
        else if (param[index] < 0.83333f)
        {
            strcpy(text, "RPN");
        }
        else
        {
            strcpy(text, "RPN (LSB)");
        }
    }
    else if (index == kChin)
    {
        if (FLOAT_TO_CHANNEL016(param[index]) < 1)
        {
            strcpy(text, "Any");
        }
        else
        {
            sprintf(text, "Channel %d", FLOAT_TO_CHANNEL016(param[index]));
        }
    }
    else if (index == kChout)
    {
        if (FLOAT_TO_CHANNEL016(param[index]) < 1)
        {
            strcpy(text, "No Change");
        }
        else
        {
            sprintf(text, "Channel %d", FLOAT_TO_CHANNEL016(param[index]));
        }
    }
    else if (index == kDiscard)
    {
        if (param[index] < 0.3f)
        {
            strcpy(text, "Block Converted");
        }
        else if (param[index] < 0.7f)
        {
            strcpy(text, "Block All");
        }
        else
        {
            strcpy(text, "Thru All");
        }
    }

    else
    {
        for (int i = 0; i < kNumSlots; i++)
        {
            if (index == i * 4 + 4)
            { //coarse in
                if (FLOAT_TO_MIDI_X(param[i * 4 + 5]) == -1)
                {
                    strcpy(text, "Learn next...");
                }
                else if (FLOAT_TO_MIDI2(param[index]) == 0)
                {
                    strcpy(text, "None");
                }
                else if (param[kIn] < 0.2f)
                {
                    sprintf(text, "CC %d", FLOAT_TO_MIDI2(param[index]) - 1);
                }
                else if (param[kIn] >= 0.2f)
                {
                    if (FLOAT_TO_MIDI2(param[index]) - 1 == 0)
                    {
                        strcpy(text, "0x0000");
                    }
                    else
                    {
                        sprintf(text, "%#.4x", (FLOAT_TO_MIDI2(param[index]) - 1) << 7);
                    }
                }
            }
            else if (index == i * 4 + 5)
            { //nrpn in
                if (FLOAT_TO_MIDI_X(param[index]) == -1)
                {
                    strcpy(text, "Learn next...");
                }
                else if (FLOAT_TO_MIDI2(param[i * 4 + 4]) == 0)
                {
                    strcpy(text, "(set coarse)"); //no input
                }
                else if (param[kIn] >= 0.2f && param[kIn] < 0.6f)
                {
                    if ((FLOAT_TO_MIDI_X(param[index]) | ((FLOAT_TO_MIDI2(param[i * 4 + 4]) - 1) << 7)) == 0)
                    {
                        strcpy(text, "NRPN 0x0000");
                    }
                    else
                    {
                        sprintf(text, "NRPN %#.4x", FLOAT_TO_MIDI_X(param[index]) | ((FLOAT_TO_MIDI2(param[i * 4 + 4]) - 1) << 7));
                    }
                }
                else if (param[kIn] >= 0.6f && param[kIn] < 0.8f)
                {
                    if ((FLOAT_TO_MIDI_X(param[index]) | ((FLOAT_TO_MIDI2(param[i * 4 + 4]) - 1) << 7)) == 0)
                    {
                        strcpy(text, "RPN 0x0000");
                    }
                    else
                    {
                        sprintf(text, "RPN %#.4x", FLOAT_TO_MIDI_X(param[index]) | ((FLOAT_TO_MIDI2(param[i * 4 + 4]) - 1) << 7));
                    }
                }
                else
                {
                    strcpy(text, "(not used)"); //no input
                }
            }
            else if (index == i * 4 + 6)
            {
                if (FLOAT_TO_MIDI2(param[index]) == 0)
                {
                    strcpy(text, "None"); //no output
                }
                else if (param[kOut] >= 0.16667f && param[kOut] < 0.33333f)
                {
                    sprintf(text, "CC %d", FLOAT_TO_MIDI2(param[index]) - 1);
                }
                else if (param[kOut] >= 0.33333f)
                {
                    if (FLOAT_TO_MIDI2(param[index]) - 1 == 0)
                    {
                        strcpy(text, "0x0000");
                    }
                    else
                    {
                        sprintf(text, "%#.4x", (FLOAT_TO_MIDI2(param[index]) - 1) << 7);
                    }
                }
                //if (FLOAT_TO_MIDI2(param[i*4+4])==0) strcat(text, " (off)"); //no input
            }
            else if (index == i * 4 + 7)
            {
                //if      (FLOAT_TO_MIDI2(param[i*4+4])==0) strcpy(text, " "); //no input
                if (FLOAT_TO_MIDI2(param[i * 4 + 6]) == 0)
                {
                    strcpy(text, "(set coarse)");
                }
                else if (param[kOut] >= 0.33333f && param[kOut] < 0.66667f)
                {
                    if ((FLOAT_TO_MIDI(param[index]) | ((FLOAT_TO_MIDI2(param[i * 4 + 6]) - 1) << 7)) == 0)
                    {
                        strcpy(text, "NRPN 0x0000");
                    }
                    else
                    {
                        sprintf(text, "NRPN %#.4x", FLOAT_TO_MIDI(param[index]) | ((FLOAT_TO_MIDI2(param[i * 4 + 6]) - 1) << 7));
                    }
                }
                else if (param[kOut] >= 0.66667f)
                {
                    if ((FLOAT_TO_MIDI(param[index]) | ((FLOAT_TO_MIDI2(param[i * 4 + 6]) - 1) << 7)) == 0)
                    {
                        strcpy(text, "RPN 0x0000");
                    }
                    else
                    {
                        sprintf(text, "RPN %#.4x", FLOAT_TO_MIDI(param[index]) | ((FLOAT_TO_MIDI2(param[i * 4 + 6]) - 1) << 7));
                    }
                }
                else
                {
                    strcpy(text, "(not used)"); //no input
                }
                //if (FLOAT_TO_MIDI2(param[i*4+4])==0) strcat(text, " (off)"); //no input
            }
        }
    }
}

//have fun modifying this one!
void MidiNRPNConverter::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    int incc[kNumSlots];
    int inputnrpn[kNumSlots];
    int outcc[kNumSlots];
    int outputnrpn[kNumSlots];
    bool learnIn[kNumSlots];
    //int gotboth=0;

    for (int j = 0; j < kNumSlots; j++)
    {
        incc[j]       = FLOAT_TO_MIDI2(param[j * 4 + 4]) - 1;
        learnIn[j]    = FLOAT_TO_MIDI_X(param[j * 4 + 5]) == -1;
        inputnrpn[j]  = FLOAT_TO_MIDI_X(param[j * 4 + 5]) | (incc[j] << 7);
        outcc[j]      = FLOAT_TO_MIDI2(param[j * 4 + 6]); //1-128 (subtract 1 to use)
        outputnrpn[j] = FLOAT_TO_MIDI(param[j * 4 + 7]) | ((outcc[j] - 1) << 7);
    }

    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];
        short status       = tomod.midiData[0] & 0xf0;       // scraping  channel
        short channel      = (tomod.midiData[0] & 0x0f) + 1; // isolating channel (1-16)
        short data1        = tomod.midiData[1] & 0x7f;
        short data2        = tomod.midiData[2] & 0x7f;

        bool discard = param[kDiscard] >= 0.3f && param[kDiscard] < 0.7f;
        bool thru    = ! discard;

        int chin = FLOAT_TO_CHANNEL016(param[kChin]);  //1-16
        chout    = FLOAT_TO_CHANNEL016(param[kChout]); //1-16
        if (chout == 0)
        {
            chout = channel;
        }

        //------------------------------------------------------------------------------
        if (param[kIn] < 0.2)
        { //CC input
            if (status == MIDI_CONTROLCHANGE)
            {
                for (int n = 0; n < kNumSlots; n++)
                {
                    if ((channel == chin || chin == 0) && data1 == incc[n])
                    {
                        // send old message if "thru" is on
                        if (thru)
                        {
                            if (param[kDiscard] < 0.3f)
                            {
                                discard = true;
                            }
                            if (channel == chout && data1 == (outcc[n] - 1) && param[kOut] < 0.33333f)
                            {
                                discard = true;
                            }
                        }
                        // create new message
                        int newdata = data2;
                        if (param[kOut] < 0.16667f)
                        {
                            //no output
                        }
                        else if (param[kOut] < 0.33333f)
                        { //CC
                            if (outcc[n] != 0)
                            {
                                VstMidiEvent cc = inputs[0][i];
                                cc.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                cc.midiData[1]  = outcc[n] - 1;
                                cc.midiData[2]  = newdata;
                                outputs[0].push_back(cc);
                            }
                        }
                        else if (param[kOut] < 0.5f)
                        { //NRPN output
                            if (outcc[n] != 0)
                            {
                                VstMidiEvent ncoarse = inputs[0][i];
                                ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                ncoarse.midiData[1]  = 99;
                                ncoarse.midiData[2]  = outcc[n] - 1;
                                outputs[0].push_back(ncoarse);

                                VstMidiEvent nfine = inputs[0][i];
                                nfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                nfine.midiData[1]  = 98;
                                nfine.midiData[2]  = (outputnrpn[n] & 0x007f);
                                outputs[0].push_back(nfine);

                                VstMidiEvent dcoarse = inputs[0][i];
                                dcoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                dcoarse.midiData[1]  = 6;
                                dcoarse.midiData[2]  = newdata;
                                outputs[0].push_back(dcoarse);

                                VstMidiEvent dfine = inputs[0][i];
                                dfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                dfine.midiData[1]  = 38;
                                if (newdata > 64)
                                {
                                    dfine.midiData[2] = newdata;
                                }
                                else
                                {
                                    dfine.midiData[2] = 0;
                                }
                                outputs[0].push_back(dfine);
                            }
                        }
                        else if (param[kOut] < 0.66667f)
                        { //NRPN (LSB) output
                            if (outcc[n] != 0)
                            {
                                VstMidiEvent ncoarse = inputs[0][i];
                                ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                ncoarse.midiData[1]  = 99;
                                ncoarse.midiData[2]  = outcc[n] - 1;
                                outputs[0].push_back(ncoarse);

                                VstMidiEvent nfine = inputs[0][i];
                                nfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                nfine.midiData[1]  = 98;
                                nfine.midiData[2]  = outputnrpn[n] & 0x007f;
                                outputs[0].push_back(nfine);

                                VstMidiEvent dfine = inputs[0][i];
                                dfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                dfine.midiData[1]  = 38;
                                dfine.midiData[2]  = newdata;
                                outputs[0].push_back(dfine);
                            }
                        }
                        else if (param[kOut] < 0.83333f)
                        { //RPN output
                            if (outcc[n] != 0)
                            {
                                VstMidiEvent ncoarse = inputs[0][i];
                                ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                ncoarse.midiData[1]  = 101;
                                ncoarse.midiData[2]  = outcc[n] - 1;
                                outputs[0].push_back(ncoarse);

                                VstMidiEvent nfine = inputs[0][i];
                                nfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                nfine.midiData[1]  = 100;
                                nfine.midiData[2]  = outputnrpn[n] & 0x007f;
                                outputs[0].push_back(nfine);

                                VstMidiEvent dcoarse = inputs[0][i];
                                dcoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                dcoarse.midiData[1]  = 6;
                                dcoarse.midiData[2]  = newdata;
                                outputs[0].push_back(dcoarse);

                                VstMidiEvent dfine = inputs[0][i];
                                dfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                dfine.midiData[1]  = 38;
                                if (newdata > 64)
                                {
                                    dfine.midiData[2] = newdata;
                                }
                                else
                                {
                                    dfine.midiData[2] = 0;
                                }
                                outputs[0].push_back(dfine);
                            }
                        }
                        else if (param[kOut] <= 1.f)
                        { //RPN (LSB) output
                            if (outcc[n] != 0)
                            {
                                VstMidiEvent ncoarse = inputs[0][i];
                                ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                ncoarse.midiData[1]  = 101;
                                ncoarse.midiData[2]  = outcc[n] - 1;
                                outputs[0].push_back(ncoarse);

                                VstMidiEvent nfine = inputs[0][i];
                                nfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                nfine.midiData[1]  = 100;
                                nfine.midiData[2]  = outputnrpn[n] & 0x007f;
                                outputs[0].push_back(nfine);

                                VstMidiEvent dfine = inputs[0][i];
                                dfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                dfine.midiData[1]  = 38;
                                dfine.midiData[2]  = newdata;
                                outputs[0].push_back(dfine);
                            }
                        }
                    }
                }
            }
        }
        //------------------------------------------------------------------------------
        else if (param[kIn] < 0.4)
        { //NRPN input
            if (status == MIDI_CONTROLCHANGE && (channel == chin || chin == 0))
            {
                if (param[kDiscard] < 0.3f && (data1 == 98 || data1 == 99 || data1 == 6 || data1 == 38 || data1 == 96 || data1 == 97))
                {
                    discard = true;
                }
                if (data1 == 99)
                {
                    nrpncoarse[channel - 1] = data2;
                }
                else if (data1 == 98 && nrpncoarse[channel - 1] >= 0)
                {
                    nrpn[channel - 1] = data2 | (nrpncoarse[channel - 1] << 7);
                }
                int newdata = data2;
                for (int n = 0; n < kNumSlots; n++)
                {
                    if (learnIn[n])
                    {
                        inputnrpn[n] = nrpn[channel - 1];
                        setParameterAutomated(n * 4 + 4, MIDI_TO_FLOAT2(((inputnrpn[n] & 0x3f80) >> 7) + 1));
                        setParameterAutomated(n * 4 + 5, MIDI_TO_FLOAT_X((inputnrpn[n] & 0x007f)));
                    }
                    if (nrpn[channel - 1] == inputnrpn[n])
                    {
                        int send = 0; //1: coarse;  2: fine
                        if (data1 == 6)
                        { //data entry slider (coarse)
                            datacoarse[channel - 1] = data2;
                            data[channel - 1]       = datafine[channel - 1] | (datacoarse[channel - 1] << 7);
                            send                    = 1;
                        }
                        else if (data1 == 38)
                        { //data entry slider (fine)
                            datafine[channel - 1] = data2;
                            data[channel - 1]     = datafine[channel - 1] | (datacoarse[channel - 1] << 7);
                            send                  = 2;
                        }
                        else if (data1 == 96)
                        { //data increment button
                            if (param[kOut] < 0.1f)
                            {
                                datacoarse[channel - 1] += 1;
                                if (datacoarse[channel - 1] > 127)
                                {
                                    datacoarse[channel - 1] = 127;
                                }
                                data[channel - 1] = datafine[channel - 1] | (datacoarse[channel - 1] << 7);
                                send              = 1;
                            }
                            else
                            {
                                data[channel - 1] += 1;
                                if (data[channel - 1] > 127)
                                {
                                    data[channel - 1] = 127;
                                }
                                datacoarse[channel - 1] = (data[channel - 1] & 0x3f80) >> 7;
                                datafine[channel - 1]   = data[channel - 1] & 0x007f;
                                send                    = 2;
                            }
                        }
                        else if (data1 == 97)
                        { //data decrement button
                            if (param[kOut] < 0.1f)
                            {
                                datacoarse[channel - 1] -= 1;
                                if (datacoarse[channel - 1] < 0)
                                {
                                    datacoarse[channel - 1] = 0;
                                }
                                data[channel - 1] = datafine[channel - 1] | (datacoarse[channel - 1] << 7);
                                send              = 1;
                            }
                            else
                            {
                                data[channel - 1] -= 1;
                                if (data[channel - 1] < 0)
                                {
                                    data[channel - 1] = 0;
                                }
                                datacoarse[channel - 1] = (data[channel - 1] & 0x3f80) >> 7;
                                datafine[channel - 1]   = data[channel - 1] & 0x007f;
                                send                    = 2;
                            }
                        }
                        newdata = datacoarse[channel - 1];

                        // create new message
                        if (param[kOut] < 0.16667f)
                        {
                            //discard
                        }
                        else if (param[kOut] < 0.33333f)
                        { //CC output
                            if (outcc[n] != 0 && send == 1)
                            {
                                VstMidiEvent cc = inputs[0][i];
                                cc.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                cc.midiData[1]  = outcc[n] - 1;
                                cc.midiData[2]  = newdata;
                                outputs[0].push_back(cc);
                            }
                        }
                        else if (param[kOut] < 0.5f)
                        { //NRPN output
                            if (outcc[n] != 0 && send > 0)
                            {
                                VstMidiEvent ncoarse = inputs[0][i];
                                ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                ncoarse.midiData[1]  = 99;
                                ncoarse.midiData[2]  = outcc[n] - 1;
                                outputs[0].push_back(ncoarse);

                                VstMidiEvent nfine = inputs[0][i];
                                nfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                nfine.midiData[1]  = 98;
                                nfine.midiData[2]  = outputnrpn[n] & 0x007f;
                                outputs[0].push_back(nfine);

                                if (send == 1)
                                {
                                    VstMidiEvent dcoarse = inputs[0][i];
                                    dcoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                    dcoarse.midiData[1]  = 6;
                                    dcoarse.midiData[2]  = newdata;
                                    outputs[0].push_back(dcoarse);
                                }
                                else if (send == 2)
                                {
                                    VstMidiEvent dfine = inputs[0][i];
                                    dfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                    dfine.midiData[1]  = 38;
                                    dfine.midiData[2]  = datafine[channel - 1];
                                    outputs[0].push_back(dfine);
                                }
                            }
                        }
                        else if (param[kOut] < 0.66667f)
                        { //NRPN (LSB) output
                            if (outcc[n] != 0 && send > 0)
                            {
                                VstMidiEvent ncoarse = inputs[0][i];
                                ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                ncoarse.midiData[1]  = 99;
                                ncoarse.midiData[2]  = outcc[n] - 1;
                                outputs[0].push_back(ncoarse);

                                VstMidiEvent nfine = inputs[0][i];
                                nfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                nfine.midiData[1]  = 98;
                                nfine.midiData[2]  = outputnrpn[n] & 0x007f;
                                outputs[0].push_back(nfine);

                                VstMidiEvent dfine = inputs[0][i];
                                dfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                dfine.midiData[1]  = 38;
                                dfine.midiData[2]  = newdata;
                                outputs[0].push_back(dfine);
                            }
                        }
                        else if (param[kOut] < 0.83333f)
                        { //RPN output
                            if (outcc[n] != 0 && send > 0)
                            {
                                VstMidiEvent ncoarse = inputs[0][i];
                                ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                ncoarse.midiData[1]  = 101;
                                ncoarse.midiData[2]  = outcc[n] - 1;
                                outputs[0].push_back(ncoarse);

                                VstMidiEvent nfine = inputs[0][i];
                                nfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                nfine.midiData[1]  = 100;
                                nfine.midiData[2]  = outputnrpn[n] & 0x007f;
                                outputs[0].push_back(nfine);

                                if (send == 1)
                                {
                                    VstMidiEvent dcoarse = inputs[0][i];
                                    dcoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                    dcoarse.midiData[1]  = 6;
                                    dcoarse.midiData[2]  = newdata;
                                    outputs[0].push_back(dcoarse);
                                }
                                else if (send == 2)
                                {
                                    VstMidiEvent dfine = inputs[0][i];
                                    dfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                    dfine.midiData[1]  = 38;
                                    dfine.midiData[2]  = datafine[channel - 1];
                                    outputs[0].push_back(dfine);
                                }
                            }
                        }
                        else if (param[kOut] <= 1.f)
                        { //RPN (LSB) output
                            if (outcc[n] != 0 && send > 0)
                            {
                                VstMidiEvent ncoarse = inputs[0][i];
                                ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                ncoarse.midiData[1]  = 101;
                                ncoarse.midiData[2]  = outcc[n] - 1;
                                outputs[0].push_back(ncoarse);

                                VstMidiEvent nfine = inputs[0][i];
                                nfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                nfine.midiData[1]  = 100;
                                nfine.midiData[2]  = outputnrpn[n] & 0x007f;
                                outputs[0].push_back(nfine);

                                VstMidiEvent dfine = inputs[0][i];
                                dfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                dfine.midiData[1]  = 38;
                                dfine.midiData[2]  = newdata;
                                outputs[0].push_back(dfine);
                            }
                        }
                    }
                }
            }
        }
        //------------------------------------------------------------------------------
        else if (param[kIn] < 0.6)
        { //NRPN (LSB) input
            if (status == MIDI_CONTROLCHANGE && (channel == chin || chin == 0))
            {
                // only send nprn stuff through if "thru" is on
                if (param[kDiscard] < 0.3f && (data1 == 98 || data1 == 99 || data1 == 6 || data1 == 38 || data1 == 96 || data1 == 97))
                {
                    discard = true;
                }
                if (data1 == 99)
                {
                    nrpncoarse[channel - 1] = data2;
                }
                else if (data1 == 98 && nrpncoarse[channel - 1] >= 0)
                {
                    nrpn[channel - 1] = data2 | (nrpncoarse[channel - 1] << 7);
                }
                int newdata = data2;
                for (int n = 0; n < kNumSlots; n++)
                {
                    if (nrpn[channel - 1] == inputnrpn[n])
                    {
                        int send = 0; //1: coarse;  2: fine
                        if (data1 == 38)
                        { //data entry slider (fine)
                            datafine[channel - 1] = data2;
                            data[channel - 1]     = datafine[channel - 1] | (datacoarse[channel - 1] << 7);
                            send                  = 2;
                        }
                        else if (data1 == 96)
                        { //data increment button
                            datafine[channel - 1] += 1;
                            if (datafine[channel - 1] > 127)
                            {
                                datafine[channel - 1] = 127;
                            }
                            data[channel - 1] = datafine[channel - 1] | (datacoarse[channel - 1] << 7);
                            send              = 2;
                        }
                        else if (data1 == 97)
                        { //data decrement button
                            datafine[channel - 1] -= 1;
                            if (datafine[channel - 1] < 0)
                            {
                                datafine[channel - 1] = 0;
                            }
                            data[channel - 1] = datafine[channel - 1] | (datacoarse[channel - 1] << 7);
                            send              = 2;
                        }
                        newdata = datafine[channel - 1];

                        // create new message
                        if (param[kOut] <= 0.16667f)
                        {
                            //no output
                        }
                        else if (param[kOut] < 0.33333f)
                        { //CC output
                            if (outcc[n] != 0 && send == 2)
                            {
                                VstMidiEvent cc = inputs[0][i];
                                cc.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                cc.midiData[1]  = outcc[n] - 1;
                                cc.midiData[2]  = newdata;
                                outputs[0].push_back(cc);
                            }
                        }
                        else if (param[kOut] < 0.5f)
                        { //NRPN output
                            if (outcc[n] != 0 && send > 0)
                            {
                                VstMidiEvent ncoarse = inputs[0][i];
                                ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                ncoarse.midiData[1]  = 99;
                                ncoarse.midiData[2]  = outcc[n] - 1;
                                outputs[0].push_back(ncoarse);

                                VstMidiEvent nfine = inputs[0][i];
                                nfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                nfine.midiData[1]  = 98;
                                nfine.midiData[2]  = outputnrpn[n] & 0x007f;
                                outputs[0].push_back(nfine);

                                VstMidiEvent dcoarse = inputs[0][i];
                                dcoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                dcoarse.midiData[1]  = 6;
                                dcoarse.midiData[2]  = newdata;
                                outputs[0].push_back(dcoarse);
                            }
                        }
                        else if (param[kOut] < 0.66667f)
                        { //NRPN (LSB) output
                            if (outcc[n] != 0 && send > 0)
                            {
                                VstMidiEvent ncoarse = inputs[0][i];
                                ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                ncoarse.midiData[1]  = 99;
                                ncoarse.midiData[2]  = outcc[n] - 1;
                                outputs[0].push_back(ncoarse);

                                VstMidiEvent nfine = inputs[0][i];
                                nfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                nfine.midiData[1]  = 98;
                                nfine.midiData[2]  = outputnrpn[n] & 0x007f;
                                outputs[0].push_back(nfine);

                                VstMidiEvent dfine = inputs[0][i];
                                dfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                dfine.midiData[1]  = 38;
                                dfine.midiData[2]  = newdata;
                                outputs[0].push_back(dfine);
                            }
                        }
                        else if (param[kOut] < 0.83333f)
                        { //RPN output
                            if (outcc[n] != 0 && send > 0)
                            {
                                VstMidiEvent ncoarse = inputs[0][i];
                                ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                ncoarse.midiData[1]  = 101;
                                ncoarse.midiData[2]  = outcc[n] - 1;
                                outputs[0].push_back(ncoarse);

                                VstMidiEvent nfine = inputs[0][i];
                                nfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                nfine.midiData[1]  = 100;
                                nfine.midiData[2]  = outputnrpn[n] & 0x007f;
                                outputs[0].push_back(nfine);

                                VstMidiEvent dcoarse = inputs[0][i];
                                dcoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                dcoarse.midiData[1]  = 6;
                                dcoarse.midiData[2]  = newdata;
                                outputs[0].push_back(dcoarse);
                            }
                        }
                        else if (param[kOut] <= 1.f)
                        { //RPN (LSB) output
                            if (outcc[n] != 0 && send > 0)
                            {
                                VstMidiEvent ncoarse = inputs[0][i];
                                ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                ncoarse.midiData[1]  = 101;
                                ncoarse.midiData[2]  = outcc[n] - 1;
                                outputs[0].push_back(ncoarse);

                                VstMidiEvent nfine = inputs[0][i];
                                nfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                nfine.midiData[1]  = 100;
                                nfine.midiData[2]  = outputnrpn[n] & 0x007f;
                                outputs[0].push_back(nfine);

                                VstMidiEvent dfine = inputs[0][i];
                                dfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                dfine.midiData[1]  = 38;
                                dfine.midiData[2]  = newdata;
                                outputs[0].push_back(dfine);
                            }
                        }
                    }
                }
            }
        }
        //------------------------------------------------------------------------------
        else if (param[kIn] < 0.8)
        { //RPN input
            if (status == MIDI_CONTROLCHANGE && (channel == chin || chin == 0))
            {
                // only send nprn stuff through if "thru" is on
                if (param[kDiscard] < 0.3f && (data1 == 98 || data1 == 99 || data1 == 6 || data1 == 38 || data1 == 96 || data1 == 97))
                {
                    discard = true;
                }
                if (data1 == 101)
                {
                    rpncoarse[channel - 1] = data2;
                }
                else if (data1 == 100 && rpncoarse[channel - 1] >= 0)
                {
                    rpn[channel - 1] = data2 | (rpncoarse[channel - 1] << 7);
                }
                int newdata = data2;
                for (int n = 0; n < kNumSlots; n++)
                {
                    if (rpn[channel - 1] == inputnrpn[n])
                    {
                        int send = 0; //1: coarse;  2: fine
                        if (data1 == 6)
                        { //data entry slider (coarse)
                            datacoarse[channel - 1] = data2;
                            data[channel - 1]       = datafine[channel - 1] | (datacoarse[channel - 1] << 7);
                            send                    = 1;
                        }
                        else if (data1 == 38)
                        { //data entry slider (fine)
                            datafine[channel - 1] = data2;
                            data[channel - 1]     = datafine[channel - 1] | (datacoarse[channel - 1] << 7);
                            send                  = 2;
                        }
                        else if (data1 == 96)
                        { //data increment button
                            if (param[kOut] < 0.1f)
                            {
                                datacoarse[channel - 1] += 1;
                                if (datacoarse[channel - 1] > 127)
                                {
                                    datacoarse[channel - 1] = 127;
                                }
                                data[channel - 1] = datafine[channel - 1] | (datacoarse[channel - 1] << 7);
                                send              = 1;
                            }
                            else
                            {
                                data[channel - 1] += 1;
                                if (data[channel - 1] > 127)
                                {
                                    data[channel - 1] = 127;
                                }
                                datacoarse[channel - 1] = (data[channel - 1] & 0x3f80) >> 7;
                                datafine[channel - 1]   = data[channel - 1] & 0x007f;
                                send                    = 2;
                            }
                        }
                        else if (data1 == 97)
                        { //data decrement button
                            if (param[kOut] < 0.1f)
                            {
                                datacoarse[channel - 1] -= 1;
                                if (datacoarse[channel - 1] < 0)
                                {
                                    datacoarse[channel - 1] = 0;
                                }
                                data[channel - 1] = datafine[channel - 1] | (datacoarse[channel - 1] << 7);
                                send              = 1;
                            }
                            else
                            {
                                data[channel - 1] -= 1;
                                if (data[channel - 1] < 0)
                                {
                                    data[channel - 1] = 0;
                                }
                                datacoarse[channel - 1] = (data[channel - 1] & 0x3f80) >> 7;
                                datafine[channel - 1]   = data[channel - 1] & 0x007f;
                                send                    = 2;
                            }
                        }
                        newdata = datacoarse[channel - 1];

                        // create new message
                        if (param[kOut] <= 0.16667f)
                        {
                            //discard=true;
                        }
                        else if (param[kOut] < 0.33333f)
                        { //CC output
                            if (outcc[n] != 0 && send == 1)
                            {
                                VstMidiEvent cc = inputs[0][i];
                                cc.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                cc.midiData[1]  = outcc[n] - 1;
                                cc.midiData[2]  = newdata;
                                outputs[0].push_back(cc);
                            }
                        }
                        else if (param[kOut] < 0.5f)
                        { //NRPN output
                            if (outcc[n] != 0 && send > 0)
                            {
                                VstMidiEvent ncoarse = inputs[0][i];
                                ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                ncoarse.midiData[1]  = 99;
                                ncoarse.midiData[2]  = outcc[n] - 1;
                                outputs[0].push_back(ncoarse);

                                VstMidiEvent nfine = inputs[0][i];
                                nfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                nfine.midiData[1]  = 98;
                                nfine.midiData[2]  = outputnrpn[n] & 0x007f;
                                outputs[0].push_back(nfine);

                                if (send == 1)
                                {
                                    VstMidiEvent dcoarse = inputs[0][i];
                                    dcoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                    dcoarse.midiData[1]  = 6;
                                    dcoarse.midiData[2]  = newdata;
                                    outputs[0].push_back(dcoarse);
                                }
                                else if (send == 2)
                                {
                                    VstMidiEvent dfine = inputs[0][i];
                                    dfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                    dfine.midiData[1]  = 38;
                                    dfine.midiData[2]  = datafine[channel - 1];
                                    outputs[0].push_back(dfine);
                                }
                            }
                        }
                        else if (param[kOut] < 0.66667f)
                        { //NRPN (LSB) output
                            if (outcc[n] != 0 && send > 0)
                            {
                                VstMidiEvent ncoarse = inputs[0][i];
                                ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                ncoarse.midiData[1]  = 99;
                                ncoarse.midiData[2]  = outcc[n] - 1;
                                outputs[0].push_back(ncoarse);

                                VstMidiEvent nfine = inputs[0][i];
                                nfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                nfine.midiData[1]  = 98;
                                nfine.midiData[2]  = outputnrpn[n] & 0x007f;
                                outputs[0].push_back(nfine);

                                VstMidiEvent dfine = inputs[0][i];
                                dfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                dfine.midiData[1]  = 38;
                                dfine.midiData[2]  = newdata;
                                outputs[0].push_back(dfine);
                            }
                        }
                        else if (param[kOut] < 0.83333f)
                        { //RPN output
                            if (outcc[n] != 0 && send > 0)
                            {
                                VstMidiEvent ncoarse = inputs[0][i];
                                ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                ncoarse.midiData[1]  = 101;
                                ncoarse.midiData[2]  = outcc[n] - 1;
                                outputs[0].push_back(ncoarse);

                                VstMidiEvent nfine = inputs[0][i];
                                nfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                nfine.midiData[1]  = 100;
                                nfine.midiData[2]  = outputnrpn[n] & 0x007f;
                                outputs[0].push_back(nfine);

                                if (send == 1)
                                {
                                    VstMidiEvent dcoarse = inputs[0][i];
                                    dcoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                    dcoarse.midiData[1]  = 6;
                                    dcoarse.midiData[2]  = newdata;
                                    outputs[0].push_back(dcoarse);
                                }
                                else if (send == 2)
                                {
                                    VstMidiEvent dfine = inputs[0][i];
                                    dfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                    dfine.midiData[1]  = 38;
                                    dfine.midiData[2]  = datafine[channel - 1];
                                    outputs[0].push_back(dfine);
                                }
                            }
                        }
                        else if (param[kOut] <= 1.f)
                        { //RPN (LSB) output
                            if (outcc[n] != 0 && send > 0)
                            {
                                VstMidiEvent ncoarse = inputs[0][i];
                                ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                ncoarse.midiData[1]  = 101;
                                ncoarse.midiData[2]  = outcc[n] - 1;
                                outputs[0].push_back(ncoarse);

                                VstMidiEvent nfine = inputs[0][i];
                                nfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                nfine.midiData[1]  = 100;
                                nfine.midiData[2]  = outputnrpn[n] & 0x007f;
                                outputs[0].push_back(nfine);

                                VstMidiEvent dfine = inputs[0][i];
                                dfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                dfine.midiData[1]  = 38;
                                dfine.midiData[2]  = newdata;
                                outputs[0].push_back(dfine);
                            }
                        }
                    }
                }
            }
        }
        //------------------------------------------------------------------------------
        else if (param[kIn] <= 1.0)
        { //RPN (LSB) input
            if (status == MIDI_CONTROLCHANGE && (channel == chin || chin == 0))
            {
                // only send nprn stuff through if "thru" is on
                if (param[kDiscard] < 0.3f && (data1 == 98 || data1 == 99 || data1 == 6 || data1 == 38 || data1 == 96 || data1 == 97))
                {
                    discard = true;
                }
                if (data1 == 101)
                {
                    rpncoarse[channel - 1] = data2;
                }
                else if (data1 == 100 && rpncoarse[channel - 1] >= 0)
                {
                    rpn[channel - 1] = data2 | (rpncoarse[channel - 1] << 7);
                }
                int newdata = data2;
                for (int n = 0; n < kNumSlots; n++)
                {
                    if (rpn[channel - 1] == inputnrpn[n])
                    {
                        int send = 0; //1: coarse;  2: fine
                        if (data1 == 38)
                        { //data entry slider (fine)
                            datafine[channel - 1] = data2;
                            data[channel - 1]     = datafine[channel - 1] | (datacoarse[channel - 1] << 7);
                            send                  = 2;
                        }
                        else if (data1 == 96)
                        { //data increment button
                            datafine[channel - 1] += 1;
                            if (datafine[channel - 1] > 127)
                            {
                                datafine[channel - 1] = 127;
                            }
                            data[channel - 1] = datafine[channel - 1] | (datacoarse[channel - 1] << 7);
                            send              = 2;
                        }
                        else if (data1 == 97)
                        { //data decrement button
                            datafine[channel - 1] -= 1;
                            if (datafine[channel - 1] < 0)
                            {
                                datafine[channel - 1] = 0;
                            }
                            data[channel - 1] = datafine[channel - 1] | (datacoarse[channel - 1] << 7);
                            send              = 2;
                        }
                        newdata = datafine[channel - 1];

                        // create new message
                        if (param[kOut] <= 0.16667f)
                        {
                            //
                        }
                        else if (param[kOut] < 0.33333f)
                        { //CC output
                            if (outcc[n] != 0 && send == 2)
                            {
                                VstMidiEvent cc = inputs[0][i];
                                cc.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                cc.midiData[1]  = outcc[n] - 1;
                                cc.midiData[2]  = newdata;
                                outputs[0].push_back(cc);
                            }
                        }
                        else if (param[kOut] < 0.5f)
                        { //NRPN output
                            if (outcc[n] != 0 && send > 0)
                            {
                                VstMidiEvent ncoarse = inputs[0][i];
                                ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                ncoarse.midiData[1]  = 99;
                                ncoarse.midiData[2]  = outcc[n] - 1;
                                outputs[0].push_back(ncoarse);

                                VstMidiEvent nfine = inputs[0][i];
                                nfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                nfine.midiData[1]  = 98;
                                nfine.midiData[2]  = outputnrpn[n] & 0x007f;
                                outputs[0].push_back(nfine);

                                VstMidiEvent dcoarse = inputs[0][i];
                                dcoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                dcoarse.midiData[1]  = 6;
                                dcoarse.midiData[2]  = newdata;
                                outputs[0].push_back(dcoarse);
                            }
                        }
                        else if (param[kOut] < 0.66667f)
                        { //NRPN (LSB) output
                            if (outcc[n] != 0 && send > 0)
                            {
                                VstMidiEvent ncoarse = inputs[0][i];
                                ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                ncoarse.midiData[1]  = 99;
                                ncoarse.midiData[2]  = outcc[n] - 1;
                                outputs[0].push_back(ncoarse);

                                VstMidiEvent nfine = inputs[0][i];
                                nfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                nfine.midiData[1]  = 98;
                                nfine.midiData[2]  = outputnrpn[n] & 0x007f;
                                outputs[0].push_back(nfine);

                                VstMidiEvent dfine = inputs[0][i];
                                dfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                dfine.midiData[1]  = 38;
                                dfine.midiData[2]  = newdata;
                                outputs[0].push_back(dfine);
                            }
                        }
                        else if (param[kOut] < 0.83333f)
                        { //RPN output
                            if (outcc[n] != 0 && send > 0)
                            {
                                VstMidiEvent ncoarse = inputs[0][i];
                                ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                ncoarse.midiData[1]  = 101;
                                ncoarse.midiData[2]  = outcc[n] - 1;
                                outputs[0].push_back(ncoarse);

                                VstMidiEvent nfine = inputs[0][i];
                                nfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                nfine.midiData[1]  = 100;
                                nfine.midiData[2]  = outputnrpn[n] & 0x007f;
                                outputs[0].push_back(nfine);

                                VstMidiEvent dcoarse = inputs[0][i];
                                dcoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                dcoarse.midiData[1]  = 6;
                                dcoarse.midiData[2]  = newdata;
                                outputs[0].push_back(dcoarse);
                            }
                        }
                        else if (param[kOut] <= 1.f)
                        { //RPN (LSB) output
                            if (outcc[n] != 0 && send > 0)
                            {
                                VstMidiEvent ncoarse = inputs[0][i];
                                ncoarse.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                ncoarse.midiData[1]  = 101;
                                ncoarse.midiData[2]  = outcc[n] - 1;
                                outputs[0].push_back(ncoarse);

                                VstMidiEvent nfine = inputs[0][i];
                                nfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                nfine.midiData[1]  = 100;
                                nfine.midiData[2]  = outputnrpn[n] & 0x007f;
                                outputs[0].push_back(nfine);

                                VstMidiEvent dfine = inputs[0][i];
                                dfine.midiData[0]  = MIDI_CONTROLCHANGE | (chout - 1);
                                dfine.midiData[1]  = 38;
                                dfine.midiData[2]  = newdata;
                                outputs[0].push_back(dfine);
                            }
                        }
                    }
                }
            }
        }
        //------------------------------------------------------------------------------
        //send the original message
        if (! discard)
        {
            outputs[0].push_back(tomod);
        }
    }
}
