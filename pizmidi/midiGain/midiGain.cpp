/*-----------------------------------------------------------------------------
midiGain example 1.0
by Reuben Vinal

originally based on:
MIDI Gain 1.02
David Viens
davidv@plogue.com
-----------------------------------------------------------------------------*/

#include "midiGain.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new MidiGain (audioMaster);
}

MidiGainProgram::MidiGainProgram()
{
    // default Program Values
    param[kGain] = 0.5f;

    vst_strncpy (name, "MIDI Gain", kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------
MidiGain::MidiGain (audioMasterCallback audioMaster)
    : AudioEffectX (audioMaster, kNumPrograms, kNumParams),
      programs (0)
{
    setNumInputs (2);
    setNumOutputs (2);
    isSynth();

    setUniqueID (PLUG_IDENT);
    canProcessReplacing();

    programs = new MidiGainProgram[numPrograms];

    if (programs)
        setProgram (0);

    init();
}

//-----------------------------------------------------------------------------------------
MidiGain::~MidiGain()
{
    _cleanMidiInBuffers();
    _cleanMidiOutBuffers();

    delete[] _midiEventsIn;
    delete[] _midiEventsOut;

    if (_vstEventsToHost)
        delete _vstEventsToHost;
    if (_vstMidiEventsToHost)
        delete[] _vstMidiEventsToHost;
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void MidiGain::setProgram (VstInt32 program)
{
    if (program < numPrograms)
    {
        MidiGainProgram* ap = &programs[program];
        curProgram          = program;
        for (int i = 0; i < numParams; i++)
            setParameter (i, ap->param[i]);
    }
}

//------------------------------------------------------------------------
void MidiGain::setProgramName (char* name)
{
    vst_strncpy (programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiGain::getProgramName (char* name)
{
    vst_strncpy (name, programs[curProgram].name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
bool MidiGain::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
    if (index < numPrograms)
    {
        vst_strncpy (text, programs[index].name, kVstMaxProgNameLen);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
bool MidiGain::getVendorString (char* text)
{
    sprintf (text, PLUG_VENDOR);
    return true;
}

//-----------------------------------------------------------------------------------------
bool MidiGain::getProductString (char* text)
{
    sprintf (text, PLUG_NAME);
    return true;
}

//-----------------------------------------------------------------------------------------
bool MidiGain::getEffectName (char* name)
{
    sprintf (name, PLUG_NAME);
    return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 MidiGain::getVendorVersion()
{
    return PLUG_VERSION;
}

//-----------------------------------------------------------------------------------------
void MidiGain::setSampleRate (float sampleRateIn)
{
    AudioEffectX::setSampleRate (sampleRateIn);
}

//-----------------------------------------------------------------------------------------
void MidiGain::setBlockSize (VstInt32 blockSize)
{
    AudioEffectX::setBlockSize (blockSize);
}

//-----------------------------------------------------------------------------------------
void MidiGain::resume()
{
    AudioEffectX::resume();
}

//-----------------------------------------------------------------------------------------
void MidiGain::setParameter (VstInt32 index, float value)
{
    if (index < numParams)
        param[index] = programs[curProgram].param[index] = value;
}

//-----------------------------------------------------------------------------------------
float MidiGain::getParameter (VstInt32 index)
{
    if (index < numParams)
        return param[index];
    return 0.f;
}

//-----------------------------------------------------------------------------------------
void MidiGain::getParameterName (VstInt32 index, char* label)
{
    switch (index)
    {
        case kGain:
            vst_strncpy (label, "Gain", kVstMaxParamStrLen);
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiGain::getParameterDisplay (VstInt32 index, char* text)
{
    switch (index)
    {
        case kGain:
            sprintf (text, "%d", roundToInt (param[kGain] * 200.f));
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiGain::getParameterLabel (VstInt32 index, char* label)
{
    switch (index)
    {
        case kGain:
            vst_strncpy (label, "%", kVstMaxParamStrLen);
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
VstInt32 MidiGain::canDo (char* text)
{
    if (PLUG_MIDI_OUTPUTS)
    {
        if (! strcmp (text, "sendVstMidiEvent"))
            return 1;
        if (! strcmp (text, "sendVstEvents"))
            return 1;
    }
    if (PLUG_MIDI_INPUTS)
    {
        if (! strcmp (text, "receiveVstEvents"))
            return 1;
        if (! strcmp (text, "receiveVstMidiEvent"))
            return 1;
    }

    // VstTimeInfo
    if (! strcmp (text, "receiveVstTimeInfo"))
        return 1;

    return -1; // -1 => explicitly can't do; 0 => don't know
}

//-----------------------------------------------------------------------------------------
bool MidiGain::getInputProperties (VstInt32 index, VstPinProperties* properties)
{
    return false;
}

//-----------------------------------------------------------------------------------------
bool MidiGain::getOutputProperties (VstInt32 index, VstPinProperties* properties)
{
    return false;
}

//-----------------------------------------------------------------------------------------
bool MidiGain::init()
{
    try
    {
        _vstEventsToHost            = new MyVstEvents;
        _vstEventsToHost->numEvents = 0;
        _vstMidiEventsToHost        = new VstMidiEvent[MAX_EVENTS_PER_TIMESLICE]; //might be not enough...!!

        _midiEventsIn = new VstMidiEventVec[PLUG_MIDI_INPUTS];
        _cleanMidiInBuffers();

        _midiEventsOut = new VstMidiEventVec[PLUG_MIDI_OUTPUTS];
        _cleanMidiOutBuffers();
    }
    catch (...)
    {
        return false;
    }
    return true;
}

//DONT MODIFY THIS METHOD EITHER!!!!
void MidiGain::_cleanMidiInBuffers()
{
    for (int i = 0; i < PLUG_MIDI_INPUTS; i++)
        _midiEventsIn[i].clear();
}

//DONT MODIFY THIS METHOD EITHER!!!!
void MidiGain::_cleanMidiOutBuffers()
{
    for (int i = 0; i < PLUG_MIDI_OUTPUTS; i++)
        _midiEventsOut[i].clear();
}

void MidiGain::preProcess()
{
    // preparing Proccess
    VstTimeInfo* timeInfo = NULL;
    timeInfo              = getTimeInfo (0xffff); //ALL

    if (timeInfo)
    {
        //
    }

    _cleanMidiOutBuffers();
}

//DONT MODIFY THIS METHOD!!!!
//it sends all midi events to the host from the internal vector
void MidiGain::postProcess()
{
    if (_midiEventsOut[0].size() < MAX_EVENTS_PER_TIMESLICE)
    {
        _vstEventsToHost->numEvents = (VstInt32) (_midiEventsOut[0].size());
        for (unsigned int i = 0; i < _midiEventsOut[0].size(); i++)
        {
            _vstMidiEventsToHost[i].type        = kVstMidiType;
            _vstMidiEventsToHost[i].byteSize    = 24;
            _vstMidiEventsToHost[i].deltaFrames = _midiEventsOut[0][i].deltaFrames;
            _vstMidiEventsToHost[i].flags       = 0;
            _vstMidiEventsToHost[i].noteLength  = 0;
            _vstMidiEventsToHost[i].noteOffset  = 0;
            _vstMidiEventsToHost[i].midiData[0] = _midiEventsOut[0][i].midiData[0];
            _vstMidiEventsToHost[i].midiData[1] = _midiEventsOut[0][i].midiData[1];
            _vstMidiEventsToHost[i].midiData[2] = _midiEventsOut[0][i].midiData[2];
            _vstMidiEventsToHost[i].midiData[3] = 0;
            _vstMidiEventsToHost[i].detune      = _midiEventsOut[0][i].detune;

            _vstEventsToHost->events[i] = (VstEvent*) &_vstMidiEventsToHost[i];
        }
        _vstEventsToHost->reserved = 0;
        if (_midiEventsOut[0].size() > 0)
            sendVstEventsToHost ((VstEvents*) _vstEventsToHost);
    }
    else
    {
        VstInt32 left  = (VstInt32) _midiEventsOut[0].size();
        VstInt32 count = 0;
        while (left > 0)
        {
            if (left > MAX_EVENTS_PER_TIMESLICE)
                _vstEventsToHost->numEvents = MAX_EVENTS_PER_TIMESLICE;
            else
                _vstEventsToHost->numEvents = left;
            for (int i = 0; i < _vstEventsToHost->numEvents; i++)
            {
                VstInt32 j                          = i + count;
                _vstMidiEventsToHost[i].type        = kVstMidiType;
                _vstMidiEventsToHost[i].byteSize    = 24;
                _vstMidiEventsToHost[i].deltaFrames = _midiEventsOut[0][j].deltaFrames;
                _vstMidiEventsToHost[i].flags       = 0;
                _vstMidiEventsToHost[i].noteLength  = 0;
                _vstMidiEventsToHost[i].noteOffset  = 0;
                _vstMidiEventsToHost[i].midiData[0] = _midiEventsOut[0][j].midiData[0];
                _vstMidiEventsToHost[i].midiData[1] = _midiEventsOut[0][j].midiData[1];
                _vstMidiEventsToHost[i].midiData[2] = _midiEventsOut[0][j].midiData[2];
                _vstMidiEventsToHost[i].midiData[3] = 0;
                _vstMidiEventsToHost[i].detune      = _midiEventsOut[0][j].detune;

                _vstEventsToHost->events[i] = (VstEvent*) &_vstMidiEventsToHost[i];
            }
            _vstEventsToHost->reserved = 0;
            sendVstEventsToHost ((VstEvents*) _vstEventsToHost);
            left -= _vstEventsToHost->numEvents;
            count += _vstEventsToHost->numEvents;
        }
    }

    //flushing Midi Input Buffers before they are filled
    _cleanMidiInBuffers();
}

//DONT MODIFY THIS METHOD EITHER!!!!
//it copies all VST events comming from the host to internal vector

VstInt32 MidiGain::processEvents (VstEvents* ev)
{
    VstEvents* evts = (VstEvents*) ev;

    for (int i = 0; i < evts->numEvents; i++)
    {
        if ((evts->events[i])->type != kVstMidiType)
            continue;
        VstMidiEvent* event = (VstMidiEvent*) evts->events[i];
        _midiEventsIn[0].push_back (*event);
    }

    return true;
}

//-----------------------------------------------------------------------------------------
void MidiGain::process (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    //takes care of VstTimeInfo and such
    preProcess();

    //host should have called processEvents before process
    processMidiEvents (_midiEventsIn, _midiEventsOut);

    //dummy audio processing, copy inputs to outputs
    float* in1  = inputs[0];
    float* in2  = inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

    while (--sampleFrames)
    {
        (*out1++) += (*in1++);
        (*out2++) += (*in2++);
    }

    //sending out MIDI events to Host to conclude wrapper
    postProcess();
}

//Only modify this if you want to do parallel Audio/Midi
//-----------------------------------------------------------------------------------------
void MidiGain::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    //takes care of VstTimeInfo and such
    preProcess();

    //host should have called processEvents before process
    processMidiEvents (_midiEventsIn, _midiEventsOut);

    //dummy audio processing, copy inputs to outputs
    float* in1  = inputs[0];
    float* in2  = inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

    while (--sampleFrames)
    {
        (*out1++) = (*in1++);
        (*out2++) = (*in2++);
    }

    //sending out MIDI events to Host to conclude wrapper
    postProcess();
}

//have fun modifying this one!
void MidiGain::processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs)
{
    // process incoming events
    VstMidiEventVec::iterator it;
    for (it = inputs[0].begin(); it < inputs[0].end(); it++)
    {
        const int status  = it->midiData[0] & 0xf0; // scraping channel
        const int channel = it->midiData[0] & 0x0f; // isolating channel (0-15)
        const int data1   = it->midiData[1] & 0x7f; // note/cc number, etc
        const int data2   = it->midiData[2] & 0x7f; // velocity/cc value, etc

        if (isNoteOn (*it))
        {
            int newVelocity = roundToInt (2.f * param[kGain] * (float) data2);
            it->midiData[2] = midiLimit (newVelocity);
        }
        outputs[0].push_back (*it);
    }
}
