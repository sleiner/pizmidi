#include "midiAudioToCC.hpp"
#include <cstdlib>
#include <math.h>
#include <stdio.h>

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new MidiAudioToCC (audioMaster);
}

MidiAudioToCCProgram::MidiAudioToCCProgram()
{
    // default Program Values
    fGain    = 0.1f;
    fCC      = MIDI_TO_FLOAT (1.1);
    fCC2     = MIDI_TO_FLOAT (2.1);
    fChannel = 0.0f;
    fMode    = 0.0f;
    fOffset  = 0.1f; //"inertia"
    fRate    = 0.50f;
    // default program name
    strncpy (name, "Default", kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------
MidiAudioToCC::MidiAudioToCC (audioMasterCallback audioMaster)
    : PizMidi (audioMaster, kNumPrograms, kNumParams)
{
    programs = new MidiAudioToCCProgram[numPrograms];
    if (programs)
    {
        CFxBank* defaultBank = new CFxBank (kNumPrograms, kNumParams);
        if (readDefaultBank (PLUG_NAME, defaultBank))
        {
            if ((VstInt32) defaultBank->GetFxID() == PLUG_IDENT)
            {
                for (int i = 0; i < kNumPrograms; i++)
                {
                    programs[i].fGain    = defaultBank->GetProgParm (i, 0);
                    programs[i].fCC      = defaultBank->GetProgParm (i, 1);
                    programs[i].fCC2     = defaultBank->GetProgParm (i, 2);
                    programs[i].fChannel = defaultBank->GetProgParm (i, 3);
                    programs[i].fMode    = defaultBank->GetProgParm (i, 4);
                    programs[i].fOffset  = defaultBank->GetProgParm (i, 5);
                    programs[i].fRate    = defaultBank->GetProgParm (i, 6);
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

    numChannels = 2;
    oldenv[0]   = 0;
    olderenv[0] = 0;
    oldenv[1]   = 0;
    olderenv[1] = 0;
    fEnvelopeL  = 0.f;
    fEnvelopeR  = 0.f;

    samp[0]            = 0;
    continualRMS[0]    = 0.0;
    guiContinualRMS[0] = 0.0;
    samp[1]            = 0;
    continualRMS[1]    = 0.0;
    guiContinualRMS[1] = 0.0;

    init();
}

//-----------------------------------------------------------------------------------------
MidiAudioToCC::~MidiAudioToCC()
{
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void MidiAudioToCC::setProgram (VstInt32 program)
{
    MidiAudioToCCProgram* ap = &programs[program];

    curProgram = program;
    setParameter (kGain, ap->fGain);
    setParameter (kCC, ap->fCC);
    setParameter (kCC2, ap->fCC2);
    setParameter (kChannel, ap->fChannel);
    setParameter (kMode, ap->fMode);
    setParameter (kOffset, ap->fOffset);
    setParameter (kRate, ap->fRate);
}

//------------------------------------------------------------------------
void MidiAudioToCC::setProgramName (char* name)
{
    vst_strncpy (programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiAudioToCC::getProgramName (char* name)
{
    if (! strcmp (programs[curProgram].name, "Init"))
        sprintf (name, "%s %d", programs[curProgram].name, curProgram + 1);
    else
        strcpy (name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool MidiAudioToCC::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy (text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiAudioToCC::setParameter (VstInt32 index, float value)
{
    MidiAudioToCCProgram* ap = &programs[curProgram];

    switch (index)
    {
        case kGain:
            fGain = ap->fGain = value;
            break;
        case kCC:
            fCC = ap->fCC = value;
            break;
        case kCC2:
            fCC2 = ap->fCC2 = value;
            break;
        case kChannel:
            fChannel = ap->fChannel = value;
            break;
        case kMode:
            fMode = ap->fMode = value;
            break;
        case kOffset:
            fOffset = ap->fOffset = value;
            break;
        case kRate:
            fRate = ap->fRate = value;
            break;
        case kEnvelopeL:
            fEnvelopeL = value;
            break;
        case kEnvelopeR:
            fEnvelopeR = value;
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
float MidiAudioToCC::getParameter (VstInt32 index)
{
    switch (index)
    {
        case kGain:
            return fGain;
        case kCC:
            return fCC;
        case kCC2:
            return fCC2;
        case kChannel:
            return fChannel;
        case kMode:
            return fMode;
        case kOffset:
            return fOffset;
        case kRate:
            return fRate;
        case kEnvelopeL:
            return fEnvelopeL;
        case kEnvelopeR:
            return fEnvelopeR;
        default:
            return 0;
    }
}

//-----------------------------------------------------------------------------------------
void MidiAudioToCC::getParameterName (VstInt32 index, char* label)
{
    switch (index)
    {
        case kGain:
            strcpy (label, "Input Gain");
            break;
        case kCC:
            strcpy (label, "CC Out L");
            break;
        case kCC2:
            strcpy (label, "CC Out R");
            break;
        case kChannel:
            strcpy (label, "Channel Out");
            break;
        case kMode:
            strcpy (label, "Mode");
            break;
        case kOffset:
            strcpy (label, "Inertia");
            break;
        case kRate:
            strcpy (label, "Rate");
            break;
        case kEnvelopeL:
            strcpy (label, "Envelope L");
            break;
        case kEnvelopeR:
            strcpy (label, "Envelope R");
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiAudioToCC::getParameterDisplay (VstInt32 index, char* text)
{
    switch (index)
    {
        case kGain:
            dB2string (2 * fGain, text, 4);
            break;
        case kCC:
            sprintf (text, "%d", FLOAT_TO_MIDI (fCC));
            break;
        case kCC2:
            sprintf (text, "%d", FLOAT_TO_MIDI (fCC2));
            break;
        case kChannel:
            sprintf (text, "%d", (int) (fChannel * 15.1f) + 1);
            break;
        case kMode:
            if (fMode >= 0.5f)
                strcpy (text, "Stereo");
            else
                strcpy (text, "Mono (L+R)");
            break;
        case kOffset:
            sprintf (text, "%f", fOffset);
            break;
        case kRate:
            sprintf (text, "%f", fRate);
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiAudioToCC::getParameterLabel (VstInt32 index, char* label)
{
    switch (index)
    {
        case kGain:
            strcpy (label, " dB");
            break;
        default:
            strcpy (label, " ");
            break;
    }
}

void MidiAudioToCC::preProcess (void)
{
    // preparing Proccess
    VstTimeInfo* timeInfo = NULL;
    timeInfo              = getTimeInfo (kVstTempoValid);

    if (timeInfo)
    {
        if (kVstTempoValid & timeInfo->flags)
            _bpm = (float) timeInfo->tempo;
    }
    else
    {
        _bpm = 120.0f;
    }

    _cleanMidiOutBuffers();
}

int MidiAudioToCC::smooth (int data, int old, int older, float inertia)
{
    float change = (float) (data - old) * (1.0f - inertia * 0.95f);

    //make sure change isn't smaller than 1
    if (change < 1.0f && change > 0.0f)
        change = 1.0f;
    else if (change < 0.0f && change > -1.0f)
        change = -1.0f;

    if (change < 0.f)
        data = old - roundToInt (-change);
    else
        data = old + roundToInt (change);
    if (data < 1)
        data = 0;
    if (data > 127)
        data = 127;
    return data;
}

//-----------------------------------------------------------------------------------------
void MidiAudioToCC::process (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    //takes care of VstTimeInfo and such
    preProcess();

    int channel = FLOAT_TO_CHANNEL015 (fChannel); //outgoing midi channel
    int data1   = FLOAT_TO_MIDI (fCC);
    int data1b  = FLOAT_TO_MIDI (fCC2);

    float updateRate                   = (1.0f - fRate) * 0.0025f + 0.001f;
    const unsigned int sampleThreshold = roundToInt (getSampleRate() * updateRate);

    float gain = 12.f * fGain;

    float* in1  = inputs[0];
    float* in2  = inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

    // analyze every sample for this channel
    for (VstInt32 i = 0; i < sampleFrames; i++)
    {
        out1[i] += in1[i];
        out2[i] += in2[i];
        float in = in1[i] * gain;
        //if mono mode, sum the left and right channels:
        if (fMode < 0.5f)
        {
            in = (in2[i] + in1[i]) * 0.5f * gain;
        }
        // RMS is the sum of squared input values, then averaged and square-rooted, so here we square and sum
        continualRMS[0] += in * in;

        samp[0] += 1;
        if (samp[0] >= sampleThreshold)
        {
            guiContinualRMS[0] += continualRMS[0];
            double invGUItotal = 1.0 / (double) samp[0];
            int data2          = (int) (127.0 * (sqrt (guiContinualRMS[0] * invGUItotal)));
            data2              = smooth (data2, oldenv[0], olderenv[0], fOffset);
            if (data2 != oldenv[0])
            {
                VstMidiEvent cc;
                cc.deltaFrames = i;
                cc.midiData[0] = MIDI_CONTROLCHANGE + channel;
                cc.midiData[1] = data1;
                cc.midiData[2] = data2;
                _midiEventsOut[0].push_back (cc);
                olderenv[0] = oldenv[0];
                oldenv[0]   = data2;
                setParameterAutomated (kEnvelopeL, MIDI_TO_FLOAT (data2));
            }
            samp[0]            = 0;
            continualRMS[0]    = 0.0;
            guiContinualRMS[0] = 0.0;
        }
        if (fMode >= 0.5f)
        {
            //right channel
            in = in2[i] * gain;
            // RMS is the sum of squared input values, then averaged and square-rooted, so here we square and sum
            continualRMS[1] += in * in;

            samp[1] += 1;
            if (samp[1] >= sampleThreshold)
            {
                guiContinualRMS[1] += continualRMS[1];
                double invGUItotal = 1.0 / (double) samp[1];
                int data2b         = (int) (127 * (sqrt (guiContinualRMS[1] * invGUItotal)));
                data2b             = smooth (data2b, oldenv[1], olderenv[1], fOffset);
                if (data2b != oldenv[1])
                {
                    VstMidiEvent cc;
                    cc.deltaFrames = i;
                    cc.midiData[0] = MIDI_CONTROLCHANGE + channel;
                    cc.midiData[1] = data1b;
                    cc.midiData[2] = data2b;
                    _midiEventsOut[0].push_back (cc);
                    olderenv[1] = oldenv[1];
                    oldenv[1]   = data2b;
                    setParameterAutomated (kEnvelopeR, MIDI_TO_FLOAT (data2b));
                }
                samp[1]            = 0;
                continualRMS[1]    = 0.0;
                guiContinualRMS[1] = 0.0;
            }
        }
    }

    //sending out MIDI events to Host to conclude wrapper
    postProcess();
}

//-----------------------------------------------------------------------------------------
void MidiAudioToCC::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    //takes care of VstTimeInfo and such
    preProcess();

    int channel = FLOAT_TO_CHANNEL015 (fChannel); //outgoing midi channel
    int data1   = FLOAT_TO_MIDI (fCC);
    int data1b  = FLOAT_TO_MIDI (fCC2);

    float updateRate                   = (1.0f - fRate) * 0.0025f + 0.001f;
    const unsigned int sampleThreshold = roundToInt (getSampleRate() * updateRate);

    float gain = 12.f * fGain;

    float* in1  = inputs[0];
    float* in2  = inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

    // analyze every sample for this channel
    for (VstInt32 i = 0; i < sampleFrames; i++)
    {
        out1[i]  = in1[i];
        out2[i]  = in2[i];
        float in = in1[i] * gain;
        //if mono mode, sum the left and right channels:
        if (fMode < 0.5f)
        {
            in = (in2[i] + in1[i]) * 0.5f * gain;
        }
        // RMS is the sum of squared input values, then averaged and square-rooted, so here we square and sum
        continualRMS[0] += in * in;

        samp[0] += 1;
        if (samp[0] >= sampleThreshold)
        {
            guiContinualRMS[0] += continualRMS[0];
            double invGUItotal = 1.0 / (double) samp[0];
            int data2          = (int) (127.0 * (sqrt (guiContinualRMS[0] * invGUItotal)));
            data2              = smooth (data2, oldenv[0], olderenv[0], fOffset);
            if (data2 != oldenv[0])
            {
                VstMidiEvent cc;
                cc.deltaFrames = i;
                cc.midiData[0] = MIDI_CONTROLCHANGE + channel;
                cc.midiData[1] = data1;
                cc.midiData[2] = data2;
                _midiEventsOut[0].push_back (cc);
                olderenv[0] = oldenv[0];
                oldenv[0]   = data2;
                setParameterAutomated (kEnvelopeL, MIDI_TO_FLOAT (data2));
            }
            samp[0]            = 0;
            continualRMS[0]    = 0.0;
            guiContinualRMS[0] = 0.0;
        }
    }

    if (fMode >= 0.5f)
    {
        //right channel
        for (VstInt32 i = 0; i < sampleFrames; i++)
        {
            float in = in2[i] * gain;
            // RMS is the sum of squared input values, then averaged and square-rooted, so here we square and sum
            continualRMS[1] += in * in;

            samp[1] += 1;
            if (samp[1] >= sampleThreshold)
            {
                guiContinualRMS[1] += continualRMS[1];
                double invGUItotal = 1.0 / (double) samp[1];
                int data2b         = (int) (127 * (sqrt (guiContinualRMS[1] * invGUItotal)));
                data2b             = smooth (data2b, oldenv[1], olderenv[1], fOffset);
                if (data2b != oldenv[1])
                {
                    VstMidiEvent cc;
                    cc.deltaFrames = i;
                    cc.midiData[0] = MIDI_CONTROLCHANGE + channel;
                    cc.midiData[1] = data1b;
                    cc.midiData[2] = data2b;
                    _midiEventsOut[0].push_back (cc);
                    olderenv[1] = oldenv[1];
                    oldenv[1]   = data2b;
                    setParameterAutomated (kEnvelopeR, MIDI_TO_FLOAT (data2b));
                }
                samp[1]            = 0;
                continualRMS[1]    = 0.0;
                guiContinualRMS[1] = 0.0;
            }
        }
    }

    //sending out MIDI events to Host to conclude wrapper
    postProcess();
}

//-----------------------------------------------------------------------------------------
void MidiAudioToCC::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{
    //takes care of VstTimeInfo and such
    preProcess();

    int channel = FLOAT_TO_CHANNEL015 (fChannel); //outgoing midi channel
    int data1   = FLOAT_TO_MIDI (fCC);
    int data1b  = FLOAT_TO_MIDI (fCC2);

    float updateRate                   = (1.0f - fRate) * 0.0025f + 0.001f;
    const unsigned int sampleThreshold = roundToInt (getSampleRate() * updateRate);

    float gain = 12.f * fGain;

    double* in1  = inputs[0];
    double* in2  = inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

    // analyze every sample for this channel
    for (VstInt32 i = 0; i < sampleFrames; i++)
    {
        out1[i]  = in1[i];
        out2[i]  = in2[i];
        float in = (float) in1[i] * gain;
        //if mono mode, sum the left and right channels:
        if (fMode < 0.5f)
        {
            in = (float) (in2[i] + in1[i]) * 0.5f * gain;
        }
        // RMS is the sum of squared input values, then averaged and square-rooted, so here we square and sum
        continualRMS[0] += in * in;

        samp[0] += 1;
        if (samp[0] >= sampleThreshold)
        {
            guiContinualRMS[0] += continualRMS[0];
            double invGUItotal = 1.0 / (double) samp[0];
            int data2          = (int) (127.0 * (sqrt (guiContinualRMS[0] * invGUItotal)));
            data2              = smooth (data2, oldenv[0], olderenv[0], fOffset);
            if (data2 != oldenv[0])
            {
                VstMidiEvent cc;
                cc.deltaFrames = i;
                cc.midiData[0] = MIDI_CONTROLCHANGE + channel;
                cc.midiData[1] = data1;
                cc.midiData[2] = data2;
                _midiEventsOut[0].push_back (cc);
                olderenv[0] = oldenv[0];
                oldenv[0]   = data2;
                setParameterAutomated (kEnvelopeL, MIDI_TO_FLOAT (data2));
            }
            samp[0]            = 0;
            continualRMS[0]    = 0.0;
            guiContinualRMS[0] = 0.0;
        }
    }

    if (fMode >= 0.5f)
    {
        //right channel
        for (VstInt32 i = 0; i < sampleFrames; i++)
        {
            float in = (float) in2[i] * gain;
            // RMS is the sum of squared input values, then averaged and square-rooted, so here we square and sum
            continualRMS[1] += in * in;

            samp[1] += 1;
            if (samp[1] >= sampleThreshold)
            {
                guiContinualRMS[1] += continualRMS[1];
                double invGUItotal = 1.0 / (double) samp[1];
                int data2b         = (int) (127 * (sqrt (guiContinualRMS[1] * invGUItotal)));
                data2b             = smooth (data2b, oldenv[1], olderenv[1], fOffset);
                if (data2b != oldenv[1])
                {
                    VstMidiEvent cc;
                    cc.deltaFrames = i;
                    cc.midiData[0] = MIDI_CONTROLCHANGE + channel;
                    cc.midiData[1] = data1b;
                    cc.midiData[2] = data2b;
                    _midiEventsOut[0].push_back (cc);
                    olderenv[1] = oldenv[1];
                    oldenv[1]   = data2b;
                    setParameterAutomated (kEnvelopeR, MIDI_TO_FLOAT (data2b));
                }
                samp[1]            = 0;
                continualRMS[1]    = 0.0;
                guiContinualRMS[1] = 0.0;
            }
        }
    }

    //sending out MIDI events to Host to conclude wrapper
    postProcess();
}
