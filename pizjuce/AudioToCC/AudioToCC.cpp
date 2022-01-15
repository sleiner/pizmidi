#include "AudioToCC.h"
#include "../_common/midistuff.h"
#include "AudioToCCEditor.h"

EnvelopeFollower::EnvelopeFollower()
{
    envelope = 0;
    a = pow (0.01, 1.0 / (10 * 44100 * 0.001));
    r = pow (0.01, 1.0 / (500 * 44100 * 0.001));
}

void EnvelopeFollower::setup (double attackMs, double releaseMs, double sampleRate)
{
    a = pow (0.01, 1.0 / (attackMs * sampleRate * 0.001));
    r = pow (0.01, 1.0 / (releaseMs * sampleRate * 0.001));
}

float EnvelopeFollower::process (float v)
{
    //double v=fabs(L+R);
    if (v > envelope)
        envelope = a * (envelope - v) + v;
    else
        envelope = r * (envelope - v) + v;
    if (envelope <= 0.00001f)
    {
        envelope = 0;
        return 0.f;
    }
    return (float) envelope;
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioToCC();
}

JuceProgram::JuceProgram()
{
    //default values
    param[kGain] = 2.f / maxGain;
    param[kPeakGain] = 1.f / maxGain;
    param[kCCL] = midiToFloat (1);
    param[kCCR] = midiToFloat (2);
    param[kStereo] = 1.0;
    param[kRate] = 0.5f;
    param[kDevice] = 0.0;
    param[kSmooth] = 0.5f;
    param[kChannel] = 0.0;
    param[kLOut] = 0.0;
    param[kROut] = 0.0;
    param[kLClip] = 0.0;
    param[kRClip] = 0.0;
    param[kAutomateHost] = 0.0;
    param[kMidiToHost] = 1.0;
    param[kMode] = 0.f;
    param[kAttack] = 0.02f;
    param[kRelease] = 0.12f;
    param[kGateThreshold] = 1.f;
    param[kGateCCL] = midiToFloat (3);
    param[kGateCCR] = midiToFloat (4);
    param[kGateOnValueCCL] = midiToFloat (127);
    param[kGateOnValueCCR] = midiToFloat (127);
    param[kGateOffValueCCL] = midiToFloat (0);
    param[kGateOffValueCCR] = midiToFloat (0);

    //default program name
    name = "Default";

    device.name = "--";
}

//==============================================================================
AudioToCC::AudioToCC() : envL (0),
                         envR (0),
                         peakenvL (0),
                         peakenvR (0),
                         maxAttack (16000),
                         maxRelease (16000)
{
    programs = new JuceProgram[getNumPrograms()];

    devices = MidiOutput::getAvailableDevices();
    midiOutput = NULL;
    lastCCL = 0;
    lastCCR = 0;
    oldenv[0] = 0;
    olderenv[0] = 0;
    oldenv[1] = 0;
    olderenv[1] = 0;

    samp[0] = 0;
    continualRMS[0] = 0.0;
    guiContinualRMS[0] = 0.0;
    samp[1] = 0;
    continualRMS[1] = 0.0;
    guiContinualRMS[1] = 0.0;
    rateCounter = 0;

    envL = new EnvelopeFollower();
    envR = new EnvelopeFollower();
    peakenvL = new EnvelopeFollower();
    peakenvR = new EnvelopeFollower();

    lastInL = 0.f;
    lastInR = 0.f;
    lastGateCCL = false;
    lastGateCCR = false;
    resetGateL = false;
    resetGateR = false;

    curProgram = 0;
    if (! loadDefaultFxb())
    {
        for (int i = 0; i < getNumPrograms(); i++)
        {
            programs[i].name = String ("Program ") + String (i + 1);
        }
    }
    setCurrentProgram (0);
}

AudioToCC::~AudioToCC()
{
    if (midiOutput)
    {
        midiOutput->stopBackgroundThread();
        midiOutput.reset();
    }
    if (envL)
        delete envL;
    if (envR)
        delete envR;
    if (peakenvL)
        delete peakenvL;
    if (peakenvR)
        delete peakenvR;
}

//==============================================================================
int AudioToCC::getNumParameters()
{
    return numParams;
}

float AudioToCC::getParameter (int index)
{
    return param[index];
}

void AudioToCC::setParameter (int index, float newValue)
{
    JuceProgram* ap = &programs[curProgram];

    if (index == kGateResetL)
    {
        if (newValue == 1.f)
        {
            resetGateL = true;
        }
        return;
    }
    else if (index == kGateResetR)
    {
        if (newValue == 1.f)
        {
            resetGateR = true;
        }
        return;
    }
    param[index] = ap->param[index] = newValue;
    if (index == kDevice && newValue != param[kDevice])
    {
        if (newValue > 0.f)
        {
            setActiveDevice (devices[roundToInt (newValue * (devices.size() - 1))]);
        }
        else
        {
            if (midiOutput)
                midiOutput->stopBackgroundThread();
            midiOutput = NULL;
        }
    }
    else if (index == kAttack || index == kRelease || index == kRate)
    {
        envL->setup (param[kAttack] * maxAttack, param[kRelease] * maxRelease, getSampleRate() * ((1.0f - param[kRate]) * 0.0025f + 0.001f));
        envR->setup (param[kAttack] * maxAttack, param[kRelease] * maxRelease, getSampleRate() * ((1.0f - param[kRate]) * 0.0025f + 0.001f));
    }
    sendChangeMessage();
}

void AudioToCC::setActiveDevice (String name)
{
    setActiveDevice (getDeviceByName (name));
}

void AudioToCC::setActiveDevice (MidiDeviceInfo const& device)
{
    activeDevice = programs[curProgram].device = device;
    int index = devices.indexOf (device);

    if (index == -1)
    {
        param[kDevice] = programs[curProgram].param[kDevice] = 0.f;
        if (midiOutput)
            midiOutput->stopBackgroundThread();
        midiOutput = NULL;
    }
    else
    {
        param[kDevice] = programs[curProgram].param[kDevice] = (float) index / (float) (devices.size() - 1);
        if (midiOutput)
            midiOutput->stopBackgroundThread();
        midiOutput = MidiOutput::openDevice (device.identifier);
        midiOutput->startBackgroundThread();
    }
}

MidiDeviceInfo AudioToCC::getDeviceByName (String name) const
{
    return devices.findIf ([&] (auto const& device)
                           { return name == device.name; });
}

const String AudioToCC::getParameterName (int index)
{
    if (index == kGain)
        return "RMSGain";
    else if (index == kPeakGain)
        return "PeakGain";
    else if (index == kDevice)
        return "Device";
    else if (index == kCCL)
        return "L CC";
    else if (index == kCCR)
        return "R CC";
    else if (index == kStereo)
        return "Mono/Stereo";
    else if (index == kRate)
        return "Rate";
    else if (index == kChannel)
        return "Channe";
    else if (index == kSmooth)
        return "Inertia";
    else if (index == kAutomateHost)
        return "AutomateHost";
    else if (index == kMidiToHost)
        return "MidiToHost";
    else if (index == kLOut)
        return "Envelope";
    else if (index == kROut)
        return "EnvelopeR";
    else if (index == kLClip)
        return "GateEnv";
    else if (index == kRClip)
        return "GateEnvR";
    else if (index == kMode)
        return "EnvScale";
    else if (index == kAttack)
        return "Attack";
    else if (index == kRelease)
        return "Release";
    else if (index == kGateThreshold)
        return "Gate Thresh";
    else if (index == kGateCCL)
        return "L Gate CC";
    else if (index == kGateCCR)
        return "R Gate CC";
    else if (index == kGateOnValueCCL)
        return "L On Value";
    else if (index == kGateOnValueCCR)
        return "R On Value";
    else if (index == kGateOffValueCCL)
        return "L Off Value";
    else if (index == kGateOffValueCCR)
        return "R Off Value";
    else if (index == kGateResetL)
        return "L Gate Reset";
    else if (index == kGateResetR)
        return "R Gate Reset";
    return String();
}

const String AudioToCC::getParameterText (int index)
{
    if (index == kGain || index == kPeakGain)
        return param[index] == 0.f ? String ("-inf") : String (20.f * log10 (param[index] * maxGain), 1) + " dB";
    else if (index == kAttack)
        return String (roundToInt (param[kAttack] * 100.f));
    else if (index == kRelease)
        return String (roundToInt (param[kRelease] * 100.f));
    else if (index == kDevice)
    {
        if (param[kDevice] > 0.0)
            return devices[roundToInt (param[kDevice] * (devices.size() - 1))].name;
        else
            return String ("--");
    }
    else if (index == kCCL || index == kCCR)
    {
        const int v = floatToMidi (param[index]);
        return v < 0 ? "Off" : String (floatToMidi (param[index]));
    }
    else if (index == kStereo)
    {
        if (param[kStereo] < 0.5f)
            return String ("Mono (L+R)");
        else
            return String ("Stereo");
    }
    else if (index == kChannel)
        return String (roundToInt (param[index] * 15.0) + 1);
    else if (index == kAutomateHost)
    {
        if (param[kAutomateHost] < 0.5f)
            return String ("Off");
        else
            return String ("On");
    }
    else if (index == kMode)
    {
        if (param[kMode] >= 0.5f)
            return String ("Logarithmic");
        else
            return String ("Linear");
    }
    else if (index == kMidiToHost)
    {
        if (param[kMidiToHost] < 0.5f)
            return String ("Off");
        else
            return String ("On");
    }
    else if (index == kGateThreshold)
    {
        if (param[index] == 0.f)
            return "Off";
        return String (20.f * log10 (param[index]), 1) + " dB";
    }
    else if (index == kGateCCL || index == kGateCCR)
    {
        const int v = floatToMidi (param[index]);
        return v < 0 ? "Off" : String (floatToMidi (param[index]));
    }
    else if (index == kGateOnValueCCL || index == kGateOnValueCCR)
    {
        const int v = floatToMidi (param[index]);
        return v < 0 ? "Off" : String (floatToMidi (param[index]));
    }
    else if (index == kGateOffValueCCL || index == kGateOffValueCCR)
    {
        const int v = floatToMidi (param[index]);
        return v < 0 ? "Off" : String (floatToMidi (param[index]));
    }
    else if (index == kRate)
    {
        return String ((param[index] == 1.f ? 1.0 / getSampleRate() : (1.0f - param[index]) * 0.0025f + 0.001f) * 1000, 1) + " ms";
    }
    else
        return String (roundToInt (param[index] * 100.f));
}

const String AudioToCC::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String AudioToCC::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool AudioToCC::isInputChannelStereoPair (int index) const
{
    if (getNumInputChannels() == 2)
        return true;
    else
        return false;
}

bool AudioToCC::isOutputChannelStereoPair (int index) const
{
    if (getNumOutputChannels() == 2)
        return true;
    else
        return false;
}

void AudioToCC::setCurrentProgram (int index)
{
    JuceProgram* ap = &programs[index];
    curProgram = index;
    setActiveDevice (ap->device);
    memcpy (param, ap->param, sizeof (param));
    sendChangeMessage();
}

void AudioToCC::changeProgramName (int index, const String& newName)
{
    programs[curProgram].name = newName;
}

const String AudioToCC::getProgramName (int index)
{
    return programs[index].name;
}

int AudioToCC::getCurrentProgram()
{
    return curProgram;
}

//==============================================================================
void AudioToCC::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // do your pre-playback setup stuff here..
    envL->setup (param[kAttack] * maxAttack, param[kRelease] * maxRelease, sampleRate * ((1.0f - param[kRate]) * 0.0025f + 0.001f));
    envR->setup (param[kAttack] * maxAttack, param[kRelease] * maxRelease, sampleRate * ((1.0f - param[kRate]) * 0.0025f + 0.001f));
    peakenvL->setup (0.0, 100.0, sampleRate);
    peakenvR->setup (0.0, 100.0, sampleRate);
}

void AudioToCC::releaseResources()
{
    // when playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

// wimpy "interpolation" from last 3 data points
int AudioToCC::smooth (int data, int old, int older, float inertia)
{
    //data=roundToInt((float)(data+old+older)/3.0f);
    //if (data>old) data=old+1;
    //else if (data<old) data=old-1;

    //if increasing
    //if (data>old) data=data-roundToInt((float)(data-old)*inertia*0.9f);
    float change = (float) (data - old) * (1.0f - inertia * 0.95f);
    if (change < 1.0f && change > 0.0f)
        change = 1.0f;
    else if (change < 0.0f && change > -1.0f)
        change = -1.0f;
    data = old + roundToInt (change);
    if (data < 1)
        data = 0;
    if (data > 127)
        data = 127;
    return data;
}

void AudioToCC::processBlock (AudioSampleBuffer& buffer,
                              MidiBuffer& midiMessages)
{
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
    midiMessages.clear();

    const int ccL = floatToMidi (param[kCCL]);
    const int ccR = floatToMidi (param[kCCR]);
    const int gateccL = floatToMidi (param[kGateCCL]);
    const int gateccR = floatToMidi (param[kGateCCR]);
    const int gateOnValueL = floatToMidi (param[kGateOnValueCCL]);
    const int gateOnValueR = floatToMidi (param[kGateOnValueCCR]);
    const int gateOffValueL = floatToMidi (param[kGateOffValueCCL]);
    const int gateOffValueR = floatToMidi (param[kGateOffValueCCR]);
    const float thresh = param[kGateThreshold];
    const int Ch = roundToInt (param[kChannel] * 15.0);
    const float updateRate = (1.0f - param[kRate]) * 0.0025f + 0.001f; //in seconds
    const bool stereo = param[kStereo] >= 0.5f;
    const unsigned int sampleThreshold = param[kRate] == 1.f ? 1 : roundToInt (getSampleRate() * updateRate);
    const unsigned int RMSThreshold = roundToInt (getSampleRate() * 0.001f);
    const bool oldMethod = false;
    const bool logScale = param[kMode] >= 0.5f;

    const float* in1 = buffer.getReadPointer (0);
    const float* in2 = buffer.getReadPointer (1);

    float gain = maxGain * param[kGain];
    float peakgain = maxGain * param[kPeakGain];

    if (resetGateL)
    {
        if (gateccL >= 0 && gateOffValueL >= 0)
        {
            MidiMessage cc = MidiMessage::controllerEvent (Ch + 1, gateccL, gateOffValueL);
            if (midiOutput)
                midiOutput->sendMessageNow (cc);
            if (param[kMidiToHost] >= 0.5f)
                midiMessages.addEvent (cc, 0);
            if (param[kAutomateHost] >= 0.5f)
                setParameterNotifyingHost (kLClip, 0.f);
        }
        lastGateCCL = false;
        resetGateL = false;
        sendChangeMessage();
    }
    if (resetGateR)
    {
        if (gateccR >= 0 && gateOffValueR >= 0)
        {
            MidiMessage cc = MidiMessage::controllerEvent (Ch + 1, gateccR, gateOffValueR);
            if (midiOutput)
                midiOutput->sendMessageNow (cc);
            if (param[kMidiToHost] >= 0.5f)
                midiMessages.addEvent (cc, 0);
            if (param[kAutomateHost] >= 0.5f)
                setParameterNotifyingHost (kRClip, 0.f);
        }
        lastGateCCR = false;
        resetGateR = false;
        sendChangeMessage();
    }

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        float sample = in1[i] * gain;
        lastInL = peakenvL->process (in1[i] * peakgain);
        //if mono mode, sum the left and right channels:
        if (! stereo)
        {
            sample = (sample + (in2[i] * gain)) * 0.5f;
            lastInR = peakenvL->process ((in1[i] + in2[i]) * 0.5f * peakgain);
        }

        if (gateccL >= 0)
        {
            if (lastInL >= thresh && ! lastGateCCL)
            {
                if (gateOnValueL >= 0)
                {
                    MidiMessage cc = MidiMessage::controllerEvent (Ch + 1, gateccL, gateOnValueL);
                    if (midiOutput)
                        midiOutput->sendMessageNow (cc);
                    if (param[kMidiToHost] >= 0.5f)
                        midiMessages.addEvent (cc, i);
                }
                if (param[kAutomateHost] >= 0.5f)
                    setParameterNotifyingHost (kLClip, 1.f);
                lastGateCCL = true;
            }
            else if (lastInL < thresh && lastGateCCL)
            {
                if (gateOffValueL >= 0)
                {
                    MidiMessage cc = MidiMessage::controllerEvent (Ch + 1, gateccL, gateOffValueL);
                    if (midiOutput)
                        midiOutput->sendMessageNow (cc);
                    if (param[kMidiToHost] >= 0.5f)
                        midiMessages.addEvent (cc, i);
                }
                if (param[kAutomateHost] >= 0.5f)
                    setParameterNotifyingHost (kLClip, 0.f);
                lastGateCCL = false;
            }
        }

        // RMS is the sum of squared input values, then averaged and square-rooted,
        // so here we square and sum
        continualRMS[0] += sample * sample;

        samp[0] += 1;
        rateCounter++;

        int data2 = oldenv[0];
        if (samp[0] >= RMSThreshold)
        {
            guiContinualRMS[0] += continualRMS[0];
            float rms = (float) sqrt (guiContinualRMS[0] / (double) samp[0]);
            if (! oldMethod)
            {
                if (logScale)
                {
                    float logrms = 127.f + 100.f * log10 (rms + 0.000000001f);
                    data2 = roundToInt (jlimit (0.f, 127.f, 2 * param[kGain] * envL->process (logrms)));
                }
                else
                {
                    rms = jlimit (0.f, 1.f, envL->process (rms));
                    data2 = roundToInt (rms * 127.f);
                }
                data2 = smooth (data2, oldenv[0], olderenv[0], param[kSmooth]);
            }
            else
            {
                data2 = (int) (127 * rms);
                data2 = smooth (data2, oldenv[0], olderenv[0], param[kSmooth]);
            }
            samp[0] = 0;
            continualRMS[0] = 0.0;
            guiContinualRMS[0] = 0.0;
        }
        if (rateCounter >= sampleThreshold)
        {
            if (data2 != oldenv[0])
            {
                if (ccL >= 0)
                {
                    MidiMessage cc (0xB0 | Ch, ccL, data2);
                    if (midiOutput)
                        midiOutput->sendMessageNow (cc);
                    if (param[kMidiToHost] >= 0.5f)
                        midiMessages.addEvent (cc, i);
                    if (param[kAutomateHost] >= 0.5f)
                        setParameterNotifyingHost (kLOut, midiToFloat (data2, false));
                }
                olderenv[0] = oldenv[0];
                oldenv[0] = data2;
                lastCCL = data2;
            }
        }
    }

    if (stereo)
    {
        //right channel
        for (int i = 0; i < buffer.getNumSamples(); i++)
        {
            float sample = in2[i] * gain;
            lastInR = peakenvR->process ((in2[i] * peakgain));

            if (gateccR >= 0)
            {
                if (lastInR >= thresh && ! lastGateCCR)
                {
                    if (gateOnValueR >= 0)
                    {
                        MidiMessage cc = MidiMessage::controllerEvent (Ch + 1, gateccR, gateOnValueR);
                        if (midiOutput)
                            midiOutput->sendMessageNow (cc);
                        if (param[kMidiToHost] >= 0.5f)
                            midiMessages.addEvent (cc, i);
                    }
                    if (param[kAutomateHost] >= 0.5f && ! lastGateCCR)
                        setParameterNotifyingHost (kRClip, 1.f);
                    lastGateCCR = true;
                }
                else if (lastInR < thresh && lastGateCCR)
                {
                    if (gateOffValueR >= 0)
                    {
                        MidiMessage cc = MidiMessage::controllerEvent (Ch + 1, gateccR, gateOffValueR);
                        if (midiOutput)
                            midiOutput->sendMessageNow (cc);
                        if (param[kMidiToHost] >= 0.5f)
                            midiMessages.addEvent (cc, i);
                    }
                    if (param[kAutomateHost] >= 0.5f && lastGateCCR)
                        setParameterNotifyingHost (kRClip, 0.f);
                    lastGateCCR = false;
                }
            }

            continualRMS[1] += sample * sample;

            samp[1] += 1;
            int data2 = oldenv[1];
            if (samp[1] >= RMSThreshold)
            {
                guiContinualRMS[1] += continualRMS[1];
                float rms = (float) sqrt (guiContinualRMS[1] / (double) samp[1]);
                if (! oldMethod)
                {
                    if (logScale)
                    {
                        float logrms = 127.f + 100.f * log10 (rms + 0.000000001f);
                        data2 = roundToInt (jlimit (0.f, 127.f, 2 * param[kGain] * envR->process (logrms)));
                    }
                    else
                    {
                        rms = jlimit (0.f, 1.f, envR->process (rms));
                        data2 = roundToInt (rms * 127.f);
                    }
                    data2 = smooth (data2, oldenv[1], olderenv[1], param[kSmooth]);
                }
                else
                {
                    data2 = (int) (127.f * rms);
                    data2 = smooth (data2, oldenv[1], olderenv[1], param[kSmooth]);
                }
                samp[1] = 0;
                continualRMS[1] = 0.0;
                guiContinualRMS[1] = 0.0;
            }
            if (rateCounter >= sampleThreshold)
            {
                if (data2 != oldenv[1])
                {
                    if (ccR >= 0)
                    {
                        MidiMessage cc (0xB0 | Ch, ccR, data2);
                        if (midiOutput)
                            midiOutput->sendMessageNow (cc);
                        if (param[kMidiToHost] >= 0.5f)
                            midiMessages.addEvent (cc, i);
                        if (param[kAutomateHost] >= 0.5f)
                            setParameterNotifyingHost (kROut, midiToFloat (data2, false));
                    }
                    olderenv[1] = oldenv[1];
                    oldenv[1] = data2;
                    lastCCR = data2;
                }
            }
        }
        if (rateCounter >= sampleThreshold)
            rateCounter = 0;
    }
}

//==============================================================================
AudioProcessorEditor* AudioToCC::createEditor()
{
    return new AudioToCCEditor (this);
}

//==============================================================================
void AudioToCC::getStateInformation (MemoryBlock& destData)
{
    XmlElement xmlState ("PizAudioToCCSettings");
    xmlState.setAttribute ("pluginVersion", 3);
    xmlState.setAttribute ("program", getCurrentProgram());
    for (int p = 0; p < getNumPrograms(); p++)
    {
        XmlElement* prog = new XmlElement ("Program");
        prog->setAttribute ("index", p);
        JuceProgram* program = &programs[p];
        for (int i = 0; i < (numParams - 2); i++)
        {
            prog->setAttribute (getParameterName (i).toLowerCase().retainCharacters (goodXmlChars), program->param[i]);
        }
        prog->setAttribute ("progname", program->name);
        prog->setAttribute ("device", program->device.name);
        xmlState.addChildElement (prog);
    }
    copyXmlToBinary (xmlState, destData);
}

void AudioToCC::setStateInformation (const void* data, int sizeInBytes)
{
    auto const xmlState = getXmlFromBinary (data, sizeInBytes);

    if (xmlState != 0)
    {
        if (xmlState->hasTagName ("PizAudioToCCSettings"))
        {
            for (auto* e : xmlState->getChildIterator())
            {
                if (e->hasTagName ("Program"))
                {
                    const int p = e->getIntAttribute ("index");
                    JuceProgram* program = &programs[p];
                    for (int i = 0; i < (numParams - 2); i++)
                    {
                        program->param[i] = (float) e->getDoubleAttribute (getParameterName (i).toLowerCase().retainCharacters (goodXmlChars), program->param[i]);
                    }
                    if (xmlState->getIntAttribute ("pluginVersion") < 3)
                    {
                        program->param[kGain] *= 0.25f;
                        program->param[kPeakGain] *= 0.25f;
                    }
                    program->name = e->getStringAttribute ("progname", program->name);
                    program->device = getDeviceByName (e->getStringAttribute ("device", programs->device.name));
                }
            }
            setCurrentProgram (xmlState->getIntAttribute ("program", 0));
        }
    }
}
