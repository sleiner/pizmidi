#include "PizMidi.h"

//-----------------------------------------------------------------------------
PizMidi::PizMidi (audioMasterCallback audioMaster, VstInt32 numPrograms, VstInt32 numParams)
    : AudioEffectX (audioMaster, numPrograms, numParams),
      _midiEventsIn (0),
      _midiEventsOut (0),
      _midiSysexEventsIn (0),
      _midiSysexEventsOut (0),
      _vstEventsToHost (0),
      _vstMidiEventsToHost (0),
      _vstSysexEventsToHost (0)
{
    numinputs = numoutputs = 0;
    bottomOctave           = -2;
    char* host;
    host      = new char[kVstMaxVendorStrLen + 1];
    bool inst = false;
    bool ignoreDefault;
    if (getHostVendorString (host))
    {
        getHostStuff (host, inst, numoutputs, ignoreDefault);
    }
    if (! inst && numoutputs)
        numinputs = numoutputs;
    if (! getHostProductString (host))
        strcpy (host, "unknown");
    readIniFile (host, inst, numinputs, numoutputs, bottomOctave, ignoreDefault);
    delete[] host;

#if ! PLUG_FORCE_EFFECT
    if (inst)
        isSynth();
#elif PLUG_FORCE_INST
    isSynth();
#endif

#if PLUG_AUDIO_INPUTS
    numinputs = PLUG_AUDIO_INPUTS;
#endif
#if PLUG_AUDIO_OUTPUTS
    numoutputs = PLUG_AUDIO_OUTPUTS;
#endif
    setNumInputs (numinputs);
    setNumOutputs (numoutputs);

    setUniqueID (PLUG_IDENT);
    canProcessReplacing();
    canDoubleReplacing();
}

//-----------------------------------------------------------------------------------------
PizMidi::~PizMidi()
{
    _cleanMidiInBuffers();
    _cleanMidiOutBuffers();

    if (_midiEventsIn)
        delete[] _midiEventsIn;
    if (_midiEventsOut)
        delete[] _midiEventsOut;
    if (_midiSysexEventsIn)
        delete[] _midiSysexEventsIn;
    if (_midiSysexEventsOut)
        delete[] _midiSysexEventsOut;
    if (_vstEventsToHost)
        delete _vstEventsToHost;
    if (_vstMidiEventsToHost)
        delete[] _vstMidiEventsToHost;
    if (_vstSysexEventsToHost)
        delete[] _vstSysexEventsToHost;
}

//-----------------------------------------------------------------------------------------
void PizMidi::getParameterLabel (VstInt32 index, char* label)
{
    if (index < numParams)
        vst_strncpy (label, " ", kVstMaxParamStrLen);
}

void PizMidi::setParameterAutomated (VstInt32 index, float value)
{
    setParameter (index, value);
    if (audioMaster)
        audioMaster (&cEffect, audioMasterAutomate, index, 0, 0, value); // value is in opt
}

//-----------------------------------------------------------------------------------------
bool PizMidi::getVendorString (char* text)
{
    sprintf (text, PLUG_VENDOR);
    return true;
}

//-----------------------------------------------------------------------------------------
bool PizMidi::getProductString (char* text)
{
#ifndef _DEBUG
    strcpy (text, PLUG_NAME);
#else
    char temp[kVstMaxProductStrLen];
    strcpy (temp, PLUG_NAME);
    strcat (temp, " debug");
    strcpy (text, temp);
#endif
    return true;
}

//-----------------------------------------------------------------------------------------
bool PizMidi::getEffectName (char* name)
{
#ifndef _DEBUG
    strcpy (name, PLUG_NAME);
#else
    char temp[kVstMaxProductStrLen];
    strcpy (temp, PLUG_NAME);
    strcat (temp, " debug");
    strcpy (name, temp);
#endif
    return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 PizMidi::getVendorVersion()
{
    return PLUG_VERSION;
}

//-----------------------------------------------------------------------------------------
bool PizMidi::init()
{
    try
    {
        _vstEventsToHost            = new MyVstEvents;
        _vstEventsToHost->numEvents = 0;
        _vstMidiEventsToHost        = new VstMidiEvent[MAX_EVENTS_PER_TIMESLICE];
        _vstSysexEventsToHost       = new VstMidiSysexEvent[MAX_EVENTS_PER_TIMESLICE];

        _midiEventsIn      = new VstMidiEventVec[PLUG_MIDI_INPUTS];
        _midiSysexEventsIn = new VstSysexEventVec[PLUG_MIDI_INPUTS];
        _cleanMidiInBuffers();

        _midiEventsOut      = new VstMidiEventVec[PLUG_MIDI_OUTPUTS];
        _midiSysexEventsOut = new VstSysexEventVec[PLUG_MIDI_OUTPUTS];
        _cleanMidiOutBuffers();
    }
    catch (...)
    {
        return false;
    }

    return true;
}

void PizMidi::_cleanMidiInBuffers()
{
    for (int i = 0; i < PLUG_MIDI_INPUTS; i++)
    {
        _midiEventsIn[i].clear();
        _midiSysexEventsIn[i].clear();
    }
}

void PizMidi::_cleanMidiOutBuffers()
{
    for (int i = 0; i < PLUG_MIDI_OUTPUTS; i++)
    {
        _midiEventsOut[i].clear();
        _midiSysexEventsOut[i].clear();
    }
}

void PizMidi::copySysex()
{
    VstSysexEventVec::iterator it;
    for (it = _midiSysexEventsIn->begin(); it < _midiSysexEventsIn->end(); it++)
    {
        _midiSysexEventsOut->push_back (*it);
    }
}

//-----------------------------------------------------------------------------------------
void PizMidi::setSampleRate (float sampleRateIn)
{
    AudioEffectX::setSampleRate (sampleRateIn);
}

//-----------------------------------------------------------------------------------------
void PizMidi::setBlockSize (VstInt32 blockSize)
{
    AudioEffectX::setBlockSize (blockSize);
}

//-----------------------------------------------------------------------------------------
void PizMidi::resume()
{
    AudioEffectX::resume();
}

//-----------------------------------------------------------------------------------------
VstInt32 PizMidi::canDo (char* text)
{
    int result = -1;

    if (PLUG_MIDI_OUTPUTS)
    {
        if (! strcmp (text, "sendVstMidiEvent"))
            result = 1;
        else if (! strcmp (text, "sendVstEvents"))
            result = 1;
    }

    if (PLUG_MIDI_INPUTS)
    {
        if (! strcmp (text, "receiveVstEvents"))
            result = 1;
        else if (! strcmp (text, "receiveVstMidiEvent"))
            result = 1;
    }

    // VstTimeInfo
    if (! strcmp (text, "receiveVstTimeInfo"))
        result = 1;

    dbg ("canDo(" << text << "), => " << result);
    return result; // 0 => don't know
}

//-----------------------------------------------------------------------------------------
bool PizMidi::getInputProperties (VstInt32 index, VstPinProperties* properties)
{
    dbg ("getInputProperties(" << index << ")");
    if (index < numinputs)
    {
        strcpy (properties->label, PLUG_NAME);
        properties->flags |= kVstPinIsActive;
        if (index % 2 == 0)
            properties->flags |= kVstPinIsStereo;
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
bool PizMidi::getOutputProperties (VstInt32 index, VstPinProperties* properties)
{
    dbg ("getOutputProperties(" << index << ")");
    if (index < numoutputs)
    {
        strcpy (properties->label, PLUG_NAME);
        properties->flags |= kVstPinIsActive;
        if (index % 2 == 0)
            properties->flags |= kVstPinIsStereo;
        return true;
    }
    return false;
}

void PizMidi::preProcess (void)
{
    /*
	// preparing Proccess
	VstTimeInfo * timeInfo = NULL;
	timeInfo = getTimeInfo(0xffff); //ALL

	if (timeInfo)
	{
		if (kVstTempoValid & timeInfo->flags)
			_bpm = (float)timeInfo->tempo;
        // you can get all other pertinent info from VstTimeInfo
	}
    else
	{
		_bpm = 120.0f ;
	}
	*/
    _cleanMidiOutBuffers();
}

void PizMidi::postProcess (void)
{
    if (PLUG_MIDI_OUTPUTS)
    {
        VstInt32 left  = (VstInt32) _midiEventsOut[0].size();
        VstInt32 count = 0;
        while (left > 0)
        {
            if (left < MAX_EVENTS_PER_TIMESLICE)
                _vstEventsToHost->numEvents = left;
            else
                _vstEventsToHost->numEvents = MAX_EVENTS_PER_TIMESLICE;
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
            if (_vstEventsToHost->numEvents > 0)
                sendVstEventsToHost ((VstEvents*) _vstEventsToHost);
            left -= _vstEventsToHost->numEvents;
            count += _vstEventsToHost->numEvents;
        }

        left  = (VstInt32) _midiSysexEventsOut[0].size();
        count = 0;
        while (left > 0)
        {
            if (left < MAX_EVENTS_PER_TIMESLICE)
                _vstEventsToHost->numEvents = left;
            else
                _vstEventsToHost->numEvents = MAX_EVENTS_PER_TIMESLICE;
            for (int i = 0; i < _vstEventsToHost->numEvents; i++)
            {
                VstInt32 j                           = i + count;
                _vstSysexEventsToHost[i].type        = kVstSysExType;
                _vstSysexEventsToHost[i].byteSize    = _midiSysexEventsOut[0][j].byteSize;
                _vstSysexEventsToHost[i].deltaFrames = _midiSysexEventsOut[0][j].deltaFrames;
                _vstSysexEventsToHost[i].flags       = 0;
                _vstSysexEventsToHost[i].dumpBytes   = _midiSysexEventsOut[0][j].dumpBytes;
                _vstSysexEventsToHost[i].resvd1      = 0;
                _vstSysexEventsToHost[i].sysexDump   = _midiSysexEventsOut[0][j].sysexDump;
                _vstSysexEventsToHost[i].resvd2      = 0;

                _vstEventsToHost->events[i] = (VstEvent*) &_vstSysexEventsToHost[i];
            }
            _vstEventsToHost->reserved = 0;
            if (_midiSysexEventsOut[0].size() > 0)
                sendVstEventsToHost ((VstEvents*) _vstEventsToHost);
            left -= _vstEventsToHost->numEvents;
            count += _vstEventsToHost->numEvents;
        }
        //flushing Midi Input Buffers before they are filled
    }
    _cleanMidiInBuffers();
}

VstInt32 PizMidi::processEvents (VstEvents* ev)
{
    if (PLUG_MIDI_INPUTS)
    {
        VstEvents* evts = (VstEvents*) ev;

        for (int i = 0; i < evts->numEvents; i++)
        {
            if ((evts->events[i])->type == kVstMidiType)
            {
                VstMidiEvent* e = (VstMidiEvent*) evts->events[i];
                _midiEventsIn[0].push_back (*e);
            }
            else if ((evts->events[i])->type == kVstSysExType)
            {
                VstMidiSysexEvent* e = (VstMidiSysexEvent*) evts->events[i];
                _midiSysexEventsIn[0].push_back (*e);
            }
        }

        //if the host doesnt sort the incoming MIDI events (dumb)
        //sortMidiEvents(_midiEventsIn[0]);
        //sortMidiEvents(_midiSysexEventsIn[0]);
    }
    return 1;
}

//-----------------------------------------------------------------------------------------
void PizMidi::process (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    //takes care of VstTimeInfo and such
    preProcess();

    //host should have called processEvents before process
    processMidiEvents (_midiEventsIn, _midiEventsOut, sampleFrames);
    copySysex();

    if (numinputs && numoutputs)
    {
        float* in1  = inputs[0];
        float* in2  = inputs[1];
        float* out1 = outputs[0];
        float* out2 = outputs[1];

        for (VstInt32 i = 0; i < sampleFrames; i++)
        {
            (*out1++) += (*in1++);
            (*out2++) += (*in2++);
        }
    }
    else if (numoutputs)
    {
        float* out1 = outputs[0];
        float* out2 = outputs[1];

        for (VstInt32 i = 0; i < sampleFrames; i++)
        {
            (*out1++) = 0.f;
            (*out2++) = 0.f;
        }
    }

    //sending out MIDI events to Host to conclude wrapper
    postProcess();
}

//Only modify this if you want to do paralel Audio/Midi
//-----------------------------------------------------------------------------------------
void PizMidi::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    //takes care of VstTimeInfo and such
    preProcess();

    //host should have called processEvents before process
    processMidiEvents (_midiEventsIn, _midiEventsOut, sampleFrames);
    copySysex();

    if (numinputs && numoutputs)
    {
        float* in1  = inputs[0];
        float* in2  = inputs[1];
        float* out1 = outputs[0];
        float* out2 = outputs[1];

        for (VstInt32 i = 0; i < sampleFrames; i++)
        {
            (*out1++) = (*in1++);
            (*out2++) = (*in2++);
        }
    }
    else if (numoutputs)
    {
        float* out1 = outputs[0];
        float* out2 = outputs[1];

        for (VstInt32 i = 0; i < sampleFrames; i++)
        {
            (*out1++) = 0.f;
            (*out2++) = 0.f;
        }
    }

    //sending out MIDI events to Host to conclude wrapper
    postProcess();
}

void PizMidi::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{
    //takes care of VstTimeInfo and such
    preProcess();

    //host should have called processEvents before process
    processMidiEvents (_midiEventsIn, _midiEventsOut, sampleFrames);
    copySysex();

    if (numinputs && numoutputs)
    {
        double* in1  = inputs[0];
        double* in2  = inputs[1];
        double* out1 = outputs[0];
        double* out2 = outputs[1];

        for (VstInt32 i = 0; i < sampleFrames; i++)
        {
            (*out1++) = (*in1++);
            (*out2++) = (*in2++);
        }
    }
    else if (numoutputs)
    {
        double* out1 = outputs[0];
        double* out2 = outputs[1];

        for (VstInt32 i = 0; i < sampleFrames; i++)
        {
            (*out1++) = 0.0;
            (*out2++) = 0.0;
        }
    }

    //sending out MIDI events to Host to conclude wrapper
    postProcess();
}
