#include "midiCCReset.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new MidiCCReset (audioMaster);
}

MidiCCResetProgram::MidiCCResetProgram()
{
    // default program values
    this->param[kTrigger] = 0.f;
    this->param[kUsePlay] = 1.f;
    this->param[kCapture] = 1.f;
    this->param[kClear] = 0.f;
    //this->param[kTimeout]  = 0;
    this->param[kChannel] = CHANNEL_TO_FLOAT016 (0); // any channel
    this->param[kProgChSend] = 1.f;
    this->param[kUsePC] = 0.f;
    this->param[kThru] = 1.f;
    this->param[kBlockDup] = 0.f;
    //param[kUseMIDIReset]  = 0;

    // default program name
    vst_strncpy (this->name, "default", kVstMaxProgNameLen);

    for (int ch = 0; ch < 16; ch++)
    {
        for (int n = 0; n < 128; n++)
            lastCC[ch][n] = 129;
    }
}

//-----------------------------------------------------------------------------
MidiCCReset::MidiCCReset (audioMasterCallback audioMaster)
    : PizMidi (audioMaster, kNumPrograms, kNumParams)
{
    programsAreChunks();

    firstTime = true;

    init();
    programs = new MidiCCResetProgram[numPrograms];
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
}

//-----------------------------------------------------------------------------------------
MidiCCReset::~MidiCCReset()
{
    if (programs)
        delete[] programs;
}

VstInt32 MidiCCReset::getChunk (void** data, bool isPreset)
{
    dbg ("getChunk " << isPreset);
    for (int ch = 0; ch < 16; ch++)
    {
        for (int n = 0; n < 128; n++)
            programs[curProgram].lastCC[ch][n] = lastCC[ch][n];
    }
    if (isPreset)
    {
        MidiCCResetProgram* ap = &programs[curProgram];
        *data = (void*) ap;
        return sizeof (programs[curProgram]);
    }
    else
    {
        *data = (void*) programs;
        return sizeof (programs[0]) * kNumPrograms;
    }
}

VstInt32 MidiCCReset::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{
    dbg ("setChunk " << isPreset);
    if (byteSize > 0 && data != 0)
    {
        if (isPreset)
        {
            MidiCCResetProgram* ap = reinterpret_cast<MidiCCResetProgram*> (data);
            setProgramName (ap->name);
            for (int i = 0; i < kNumParams; i++)
            {
                param[i] = ap->param[i];
            }
            for (int ch = 0; ch < 16; ch++)
            {
                for (int n = 0; n < 128; n++)
                    lastCC[ch][n] = ap->lastCC[ch][n];
            }
        }
        else
        {
            MidiCCResetProgram* ap = reinterpret_cast<MidiCCResetProgram*> (data);
            for (int p = 0; p < kNumPrograms; p++)
            {
                strcpy (programs[p].name, ap->name);
                for (int i = 0; i < kNumParams; i++)
                {
                    programs[p].param[i] = ap->param[i];
                }
                for (int ch = 0; ch < 16; ch++)
                {
                    for (int n = 0; n < 128; n++)
                        programs[p].lastCC[ch][n] = ap->lastCC[ch][n];
                }
                ap++;
            }
            firstTime = true;
            setProgram (0);
        }
    }

    return 0;
}

//------------------------------------------------------------------------
void MidiCCReset::setProgram (VstInt32 program)
{
    dbg ("setProgram(" << program << ")");
    if (! firstTime)
    {
        //save non-parameter info to the old program, except the first time
        for (int ch = 0; ch < 16; ch++)
        {
            for (int n = 0; n < 128; n++)
            {
                programs[curProgram].lastCC[ch][n] = lastCC[ch][n];
            }
        }
    }

    MidiCCResetProgram* ap = &programs[program];

    curProgram = program;
    for (int i = 0; i < kNumParams; i++)
    {
        setParameter (i, ap->param[i]);
    }

    //non-parameter values go here
    for (int ch = 0; ch < 16; ch++)
    {
        for (int n = 0; n < 128; n++)
        {
            lastCC[ch][n] = ap->lastCC[ch][n];
        }
    }

    if (! firstTime)
    {
        //send default CC values for new program
        if (param[kProgChSend] >= 0.5f)
        {
            trigger = true;
            triggeredFromPreset = true;
        }
    }
    firstTime = false;
}

//------------------------------------------------------------------------
void MidiCCReset::setProgramName (char* name)
{
    vst_strncpy (programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiCCReset::getProgramName (char* name)
{
    vst_strncpy (name, programs[curProgram].name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
bool MidiCCReset::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
    if (index < numPrograms)
    {
        vst_strncpy (text, programs[index].name, kVstMaxProgNameLen);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiCCReset::setParameter (VstInt32 index, float value)
{
    //MidiCCResetProgram* ap = &programs[curProgram];
    if (index == kTrigger)
    {
        if (param[kTrigger] < 0.5f && value >= 0.5f)
            trigger = true;
        else if (param[kTrigger] >= 0.5f && value < 0.5f)
            trigger = false;
        param[index] = value;
    }
    else if (index == kClear)
    {
        if (param[kClear] < 0.5f && value >= 0.5f)
        {
            for (int ch = 0; ch < 16; ch++)
            {
                for (int i = 0; i < 128; i++)
                    lastCC[ch][i] = 129;
            }
        }
        param[index] = value;
    }
    else
    {
        param[index] = value;
        for (int p = 0; p < kNumPrograms; p++)
        {
            programs[p].param[index] = value;
        }
    }
}

//-----------------------------------------------------------------------------------------
float MidiCCReset::getParameter (VstInt32 index)
{
    return param[index];
}

//-----------------------------------------------------------------------------------------
void MidiCCReset::getParameterName (VstInt32 index, char* label)
{
    switch (index)
    {
        case kTrigger:
            strcpy (label, "ManualTrig");
            break;
        case kUsePlay:
            strcpy (label, "RecordTrig");
            break;
        case kCapture:
            strcpy (label, "Capture");
            break;
        case kClear:
            strcpy (label, "Clear");
            break;
            //	  case kTimeout:   strcpy(label, "Timeout"); break;
        case kChannel:
            strcpy (label, "Channel");
            break;
        case kProgChSend:
            strcpy (label, "ProgramTrig");
            break;
        case kUsePC:
            strcpy (label, "PC Listen");
            break;
        case kThru:
            strcpy (label, "Thru");
            break;
        case kBlockDup:
            strcpy (label, "Duplicates");
            break;
            //	  case kUseMIDIReset:   strcpy(label, "Use MIDI Reset"); break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiCCReset::getParameterDisplay (VstInt32 index, char* text)
{
    switch (index)
    {
        case kTrigger:
            if (param[index] < 0.5f)
                strcpy (text, "Trigger-->");
            else
                strcpy (text, "Triggered");
            break;
        case kUsePlay:
            if (param[index] < 0.5f)
                strcpy (text, "Off");
            else
                strcpy (text, "On");
            break;
        case kCapture:
            if (param[index] < 0.5f)
                strcpy (text, "Off");
            else
                strcpy (text, "On");
            break;
        case kClear:
            if (param[index] < 0.5f)
                strcpy (text, "Clear-->");
            else
                strcpy (text, "Cleared");
            break;
        case kChannel:
            if (FLOAT_TO_CHANNEL016 (param[index]) < 1)
                vst_strncpy (text, "All", kVstMaxParamStrLen);
            else
                sprintf (text, "%d", FLOAT_TO_CHANNEL016 (param[index]));
            break;
        case kUsePC:
            if (param[index] < 0.5f)
                strcpy (text, "Off");
            else
                strcpy (text, "On");
            break;
        case kProgChSend:
            if (param[index] < 0.5f)
                strcpy (text, "Off");
            else
                strcpy (text, "On");
            break;
        case kThru:
            if (param[index] < 0.5f)
                strcpy (text, "Off");
            else
                strcpy (text, "On");
            break;
        case kBlockDup:
            if (param[index] < 0.5f)
                strcpy (text, "Allow");
            else
                strcpy (text, "Block");
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
bool MidiCCReset::init()
{
    trigger = false;
    triggeredFromPreset = false;

    return PizMidi::init();
}

void MidiCCReset::preProcess (void)
{
    // preparing Proccess
    VstTimeInfo* timeInfo = NULL;
    timeInfo = getTimeInfo (0xffff); //ALL

    if (timeInfo)
    {
        //if (kVstTempoValid & timeInfo->flags)
        //    _bpm = (float)timeInfo->tempo;
        // you can get all other pertinent info from VstTimeInfo
        if (param[kUsePlay] >= 0.5f)
        {
            if (kVstTransportChanged & timeInfo->flags)
            {
                if (kVstTransportPlaying & timeInfo->flags)
                    trigger = true;
                else if (kVstTransportRecording & timeInfo->flags)
                    trigger = true;
            }
        }
    }
    //else
    //{
    //    _bpm = 120.0f ;
    //}

    _cleanMidiOutBuffers();
}

void MidiCCReset::processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    short listenchannel = FLOAT_TO_CHANNEL016 (param[kChannel]);

    if (trigger)
    {
        if (listenchannel > 0)
        { //specified channel
            for (int i = 0; i < 128; i++)
            {
                if (lastCC[listenchannel - 1][i] < 128)
                {
                    VstMidiEvent cc;
                    cc.deltaFrames = 0;
                    cc.midiData[0] = 0xb0 | listenchannel - 1;
                    cc.midiData[1] = i;
                    cc.midiData[2] = (char) lastCC[listenchannel - 1][i];
                    if (param[kBlockDup] >= 0.5f && triggeredFromPreset)
                    {
                        if (lastSentCC[listenchannel - 1][i] != cc.midiData[2])
                        {
                            outputs[0].push_back (cc);
                            lastSentCC[listenchannel - 1][i] = cc.midiData[2];
                        }
                    }
                    else
                    {
                        outputs[0].push_back (cc);
                        lastSentCC[listenchannel - 1][i] = cc.midiData[2];
                    }
                }
            }
        }
        else
        { //all channels
            for (int ch = 0; ch < 16; ch++)
            {
                for (int i = 0; i < 128; i++)
                {
                    if (lastCC[ch][i] < 128)
                    {
                        VstMidiEvent cc;
                        cc.deltaFrames = 0;
                        cc.midiData[0] = 0xb0 | ch;
                        cc.midiData[1] = i;
                        cc.midiData[2] = (char) lastCC[ch][i];
                        if (param[kBlockDup] >= 0.5f && triggeredFromPreset)
                        {
                            if (lastSentCC[ch][i] != cc.midiData[2])
                            {
                                outputs[0].push_back (cc);
                                lastSentCC[ch][i] = cc.midiData[2];
                            }
                        }
                        else
                        {
                            outputs[0].push_back (cc);
                            lastSentCC[ch][i] = cc.midiData[2];
                        }
                    }
                }
            }
        }
        trigger = false;
        triggeredFromPreset = false;
    }

    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        short status = tomod.midiData[0] & 0xf0; // scraping  channel
        short channel = (tomod.midiData[0] & 0x0f) + 1; // isolating channel (1-16)
        short data1 = tomod.midiData[1] & 0x7f;
        short data2 = tomod.midiData[2] & 0x7f;

        bool discard = param[kThru] < 0.5f;

        //only look at the selected channel
        if (channel == listenchannel || listenchannel == 0)
        {
            if (status == MIDI_CONTROLCHANGE && param[kCapture] >= 0.5f)
            {
                lastCC[channel - 1][data1] = data2;
                //if (data2==lastSentCC[channel-1][data1] && param[kBlockDup]>=0.5f) {
                //    discard=true;
                //}
                //else lastSentCC[channel-1][data1]=data2;
                if (! discard)
                    lastSentCC[channel - 1][data1] = data2;
            }
            else if (status == MIDI_PROGRAMCHANGE && param[kUsePC] >= 0.5f)
            {
                setProgram (data1);
                updateDisplay();
            }
        }
        if (! discard)
            outputs[0].push_back (tomod);
    }
}
