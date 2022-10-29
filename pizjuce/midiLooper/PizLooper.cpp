#include "PizLooper.h"

#include "PizLooperEditor.h"

using juce::roundToInt;

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PizLooper();
}

//==============================================================================
JuceProgram::JuceProgram()
{
    //first set all to 0, in case i forget any later
    for (int i = 0; i < numGlobalParams + numParamsPerSlot; i++)
        param[i] = 0;

    //set default values
    param[kRecord]    = 0.0f;
    param[kPlay]      = 0.0f;
    param[kTranspose] = 0.5f;  // 0
    param[kOctave]    = 0.5f;  // 0
    param[kVelocity]  = 0.5f;  // 100%
    param[kLoopStart] = 0.5f;  // 0
    param[kLoopEnd]   = 0.5f;  // 0
    param[kShift]     = 0.5f;  // 0
    param[kStretch]   = 0.49f; // x1.0

    param[kTrigger]  = 0.0f;             //0.0 ==> loop after record
    param[kNoteTrig] = 0.0f;             //0.0 ==> off
    param[kRoot]     = midiToFloat(60);  //middle C
    param[kNLow]     = midiToFloat(0);   //    full
    param[kNHigh]    = midiToFloat(127); //  range
    param[kChannel]  = 0.0f;             //0.0 ==> channel 1

    param[kFullRelease] = 0.f;
    param[kPlayGroup]   = 0.f; //none
    param[kMuteGroup]   = 0.f; //none
    param[kNote0]       = 1.f; //C
    param[kNote1]       = 0.f; //C#
    param[kNote2]       = 1.f; //D
    param[kNote3]       = 0.f; //D#
    param[kNote4]       = 1.f; //E
    param[kNote5]       = 1.f; //F
    param[kNote6]       = 0.f; //F#
    param[kNote7]       = 1.f; //G
    param[kNote8]       = 0.f; //G#
    param[kNote9]       = 1.f; //A
    param[kNote10]      = 0.f; //A#
    param[kNote11]      = 1.f; //B
    param[kReverse]     = 0.f; //forward/reverse playback
    param[kNoteToggle]  = 0.f; //note triggers toggle or momentary
    param[kTranspose10] = 0.f;

    param[kMasterVelocity]  = 0.5f; // 100%
    param[kMasterTranspose] = 0.5f; // 0
    param[kPlayMain]        = 0.f;
    param[kRecordMain]      = 0.f;
    param[kSingleLoop]      = 0.f;
    param[kRecMode]         = 0.0f; // replace
    param[kRecStep]         = 0.0f; // 1 bar
    param[kQuantize]        = 0.0f; // off
    param[kSync]            = 0.3f; //0.3 ==> ppq (zeropoint == recstart)
    param[kThru]            = 0.0f; //0.0 ==> off
    param[kFile]            = 0.5f;
    param[kSave]            = 0.0f;
    param[kParamsToHost]    = 0.25f;
    param[kRecCC]           = (-1 + 2) / 129.f;
    param[kPlayCC]          = (-1 + 2) / 129.f;

    param[kMonitor] = 1.f;

    looplength      = 0;
    loopstart       = 0;
    measureFromHere = 0;

    numerator = denominator = 4;
    device                  = juce::MidiDeviceInfo();

    //program name
    name = "Default";
}

JuceProgram::~JuceProgram()
{
}

//==============================================================================
PizLooper::PizLooper()
    : programs(0), slotLimit(numSlots)
{
    DBG("PizLooper()");
    init             = true;
    newLoop          = false;
    curProgram       = 0;
    currentLength    = 0.0;
    currentNumEvents = 0;
    keySelectorState.reset();
    memset(barTriggerSample, 0, sizeof(barTriggerSample));
    memset(barStopSample, -1, sizeof(barStopSample));
    programs = new JuceProgram[numPrograms];
    for (int i = 0; i < numSlots; i++)
    {
        programs[i].PRSettings.set("slot", i);
        capturingScale[i]       = 0;
        capturingTranspose[i]   = 0;
        lowestScaleInputNote[i] = 999;
    }
    lastUIWidth  = 800;
    lastUIHeight = 487;
    devices      = juce::MidiOutput::getAvailableDevices();
    recCC = playCC = -1;
    midiOutput     = NULL;
    info           = new Info();
    loopDir        = ((juce::File::getSpecialLocation(juce::File::currentExecutableFile)).getParentDirectory()).getFullPathName()
            + juce::File::getSeparatorString() + juce::String("midiloops");

    //look for a default bank
    if (! loadDefaultFxb())
    {
        // create built-in programs
        for (int i = 0; i < getNumPrograms(); i++)
        {
            // name programs "Loop 1" - "Loop 128" and then look for midi files "Loop 1.mid" etc.
            programs[i].name = juce::String("Loop ") + juce::String(i + 1);
            for (int p = 0; p < numGlobalParams; p++)
                param[p] = programs[i].param[p];
            for (int p = 0; p < numParamsPerSlot; p++)
            {
                int index        = p + numGlobalParams;
                int realindex    = index + numParamsPerSlot * i;
                param[realindex] = programs[i].param[index];
            }
            //readMidiFile(i,programs[i].name);
        }
    }
    //start up with the first program
    resetCurrentProgram(0);

    //temp values
    memset(hanging, 0, sizeof(hanging));
    memset(playingNote, 0, sizeof(playingNote));
    for (int ch = 0; ch < 16; ch++)
    {
        for (int n = 0; n < 128; n++)
            inputNoteTransposition[ch][n] = -1;
    }
    for (int i = 0; i < numSlots; i++)
    {
        for (int ch = 0; ch < 16; ch++)
        {
            for (int n = 0; n < 128; n++)
                recNoteOn[i][ch][n] = -1;
        }
        loopPpq[i]       = 0.f;
        recordAndPlay[i] = false;
        tRules[i]        = 0;
        tRules[i]        = new TransposeRules(this, i);
        oldloop[i]       = PizMidiMessageSequence();
        temploop[i]      = PizMidiMessageSequence();
        for (int p = 0; p < polyphony; p++)
        {
            polytrigger[i][p]         = -1;
            measurePlayFromHere[i][p] = 0;
            //			playstarted[i][p]=0;
            lastPlayedIndex[i][p] = -1;
            lastLoopCount[i][p]   = -1;
            startLoopCount[i][p]  = -1;
        }
        programs[i].loop.setSemitones(roundToInt(getParameterForSlot(kTranspose, i) * 24.f) - 12);
        programs[i].loop.setOctaves(roundToInt(getParameterForSlot(kOctave, i) * 8.f) - 4);
        currentPoly[i]        = 0;
        newlooplength[i]      = 0;
        lastrec[i]            = programs[i].param[kRecord];
        lastplay[i]           = programs[i].param[kPlay];
        lastchannel[i]        = roundToInt(programs[i].param[kChannel] * 16.0f) - 1;
        lastshift[i]          = (double) roundToInt(programs[i].param[kShift] * 16.0f - 8.0f);
        lastend[i]            = (double) roundToInt(programs[i].param[kLoopEnd] * 16.0f - 8.0f);
        laststart[i]          = (double) roundToInt(programs[i].param[kLoopStart] * 16.0f - 8.0f);
        laststretch[i]        = 1.0;
        recstart[i]           = 0;
        recstop[i]            = 0;
        oldlength[i]          = getLoopLength(i);
        measureDubFromHere[i] = 0;
        kill_all_notes[i]     = false;
    }

    //keySelectorState.addListener(this);
    samples             = 0;
    samplesInLastBuffer = 0;
    FilenameIndex       = 0;
    info->s             = "Stopped";
}

PizLooper::~PizLooper()
{
    DBG("~PizLooper()");
    dispatchPendingMessages();
    info->dispatchPendingMessages();
    for (int i = 0; i < numSlots; i++)
        if (tRules[i])
            delete tRules[i];
    if (programs)
        delete[] programs;
    if (midiOutput)
    {
        midiOutput->stopBackgroundThread();
    }
}

//==============================================================================
const juce::String PizLooper::getName() const
{
    return JucePlugin_Name;
}

float PizLooper::getParameter(int index)
{
    if (index < numParams)
        return param[index];
    return 0.f;
}

void PizLooper::setParameter(int index, float newValue)
{
    if (index < numParams)
    {
        if (param[index] != newValue)
        {
            JuceProgram* ap = &programs[curProgram];
            int p           = index;
            int slot        = curProgram;
            if (index < numGlobalParams)
            {
                //global parameters
                if (index == kFile)
                {
                    kbstate.reset();
                    if (newValue == 0.0f)
                    {
                        this->getCallbackLock().enter();
                        ap->loop.clear();
                        recordAndPlay[curProgram] = false;
                        setLoopLength(curProgram, 0);
                        setLoopStart(curProgram, 0);
                        programs[curProgram].measureFromHere = 0;
                        param[kFile]                         = 0.5f;
                        this->getCallbackLock().exit();
                        newLoop = true;
                        updateLoopInfo();
                        sendChangeMessage();
                    }
                    else if (newValue == 1.0f)
                    {
                        this->getCallbackLock().enter();
                        ap->loop.clear();
                        this->getCallbackLock().exit();
                        readMidiFile(curProgram, getProgramName(curProgram));
                        param[kFile]                         = 0.5f;
                        programs[curProgram].measureFromHere = 0;
                    }
                }
                else if (index == kSave)
                {
                    if (newValue == 1.0f)
                    {
                        writeMidiFile(curProgram, juce::File(loopDir), true);
                        param[kSave] = 0.f;
                    }
                    else if (newValue == 0.9f)
                    {
                        writeMidiFile(curProgram, juce::File(loopDir), false);
                        param[kSave] = 0.f;
                    }
                }
                else
                {
                    if (index == kPlayMain)
                        param[kPlay + curProgram * numParamsPerSlot] = ap->param[kPlay + curProgram * numParamsPerSlot] = newValue;
                    else if (index == kRecordMain)
                        param[kRecord + curProgram * numParamsPerSlot] = ap->param[kRecord + curProgram * numParamsPerSlot] = newValue;
                    else if (index == kPlayCC)
                        playCC = roundToInt(newValue * 129.f) - 2;
                    else if (index == kRecCC)
                        recCC = roundToInt(newValue * 129.f) - 2;
                    else if (index == kMidiOutDevice)
                    {
                        if (newValue > 0.0)
                        {
                            setActiveDevice(activeDevice);
                        }
                        else
                        {
                            if (midiOutput)
                                midiOutput->stopBackgroundThread();
                            midiOutput = 0;
                        }
                    }
                    //copy value to all programs
                    for (int i = 0; i < getNumPrograms(); i++)
                        param[index] = ap->param[index] = newValue;
                    if (param[kParamsToHost] >= 0.5f)
                        setParameterNotifyingHost(index, newValue);
                }
            }
            else
            {
                slot = getParameterName(index).getTrailingIntValue() - 1;
                p    = index - slot * numParamsPerSlot;
                ap   = &programs[slot];
                switch (p)
                {
                    case kPlay:
                        if (slot == curProgram)
                            param[kPlayMain] = newValue;
                        else
                        {
                            //otherwise this is skipped below
                            //paramChange->set(p,newValue,slot);
                            sendChangeMessage();
                        }
                        break;
                    case kRecord:
                        if (slot == curProgram)
                            param[kRecordMain] = newValue;
                        break;
                    case kTranspose:
                        ap->loop.setSemitones(roundToInt(newValue * 24.f) - 12);
                        break;
                    case kOctave:
                        ap->loop.setOctaves(roundToInt(newValue * 8.f) - 4);
                        break;
                    case kNote0:
                    case kNote1:
                    case kNote2:
                    case kNote3:
                    case kNote4:
                    case kNote5:
                    case kNote6:
                    case kNote7:
                    case kNote8:
                    case kNote9:
                    case kNote10:
                    case kNote11:
                        if (slot == curProgram)
                        {
                            param[index] = newValue;
                            if (newValue >= 0.5f)
                                keySelectorState.noteOn(1, p - kNote0, 1.f);
                            else
                                keySelectorState.noteOff(1, p - kNote0, 1.f);
                        }
                        break;
                    case kNumLoops:
                        for (int v = 0; v < polyphony; v++)
                        {
                            if (roundToInt(newValue * 64.0f) < lastLoopCount[slot][v])
                                lastLoopCount[slot][v] = -1;
                        }
                        break;
                    default:
                        break;
                }
                param[index] = ap->param[p] = newValue;
                if (param[kParamsToHost] >= 0.5f)
                    setParameterNotifyingHost(index, newValue);
            }
            if (slot == curProgram)
            {
                sendChangeMessage();
            }
        }
    }
}

void PizLooper::setActiveSlot(int slot)
{
    if (slot < numSlots)
    {
        for (int i = 0; i < 12; i++)
        {
            param[kNote0 + i + curProgram * numParamsPerSlot] = keySelectorState.isNoteOn(1, i) ? 1.f : 0.f;
        }
        if (param[kSingleLoop] >= 0.5f)
        {
            bool isAnySlotPlaying = false;
            for (int s = 0; s < numSlots; s++)
            {
                if (getParameterForSlot(kPlay, s) >= 0.5f)
                {
                    isAnySlotPlaying = true;
                    stopSlot(s);
                }
            }
            curProgram = slot;
            if (isAnySlotPlaying)
                playSlot(slot);
        }
        curProgram       = slot;
        currentLength    = programs[slot].looplength;
        currentNumEvents = programs[slot].loop.getNumEvents();
        keySelectorState.reset();
        for (int i = 0; i < 12; i++)
        {
            if (programs[curProgram].param[kNote0 + i] >= 0.5f)
                keySelectorState.noteOn(1, i, 1.f);
        }
        kbstate.reset();
        info->s = (getParameterForSlot(kPlay, slot) >= 0.5f) ? "Playing (" + juce::String(currentPoly[slot]) + ")" : "Stopped";
        newLoop = true;
        sendChangeMessage();
    }
}

void PizLooper::setActiveDevice(juce::String name)
{
    setActiveDevice(getDeviceByName(name));
}

void PizLooper::setActiveDevice(juce::MidiDeviceInfo device)
{
    getCallbackLock().enter();
    if (midiOutput != nullptr)
    {
        midiOutput->stopBackgroundThread();
    }
    midiOutput = juce::MidiOutput::openDevice(device.identifier);
    if (midiOutput != nullptr)
    {
        midiOutput->startBackgroundThread();
    }
    else
    {
        device.name       = "--";
        device.identifier = "";
    }
    getCallbackLock().exit();

    activeDevice = device;
    for (int i = 0; i < numPrograms; i++)
        programs[i].device = device;
}

juce::MidiDeviceInfo PizLooper::getDeviceByName(juce::String name) const
{
    return devices.findIf([&](auto const& device)
                          { return name == device.name; });
}

const juce::String PizLooper::getParameterName(int index)
{
    if (index == kThru)
        return juce::String("MidiThru");
    if (index == kMonitor)
        return juce::String("Monitor");
    if (index == kFile)
        return juce::String("File");
    if (index == kSave)
        return juce::String("Save");
    if (index == kSync)
        return juce::String("SyncMode");
    if (index == kRecStep)
        return juce::String("LoopStepsize");
    if (index == kQuantize)
        return juce::String("InputQuantize");
    if (index == kFixedLength)
        return juce::String("FixedLengh");
    if (index == kRecMode)
        return juce::String("RecMode");
    if (index == kSingleLoop)
        return juce::String("SingleMode");
    if (index == kPlayMain)
        return juce::String("MasterPlay");
    if (index == kRecordMain)
        return juce::String("MasterRecord");
    if (index == kMasterVelocity)
        return juce::String("MasterVelocity");
    if (index == kMasterTranspose)
        return juce::String("MasterTranspose");
    if (index == kMidiOutDevice)
        return juce::String("MidiOutDevice");
    if (index == kParamsToHost)
        return juce::String("HostAutomation");
    if (index == kPlayCC)
        return juce::String("MasterPlayCC");
    if (index == kRecCC)
        return juce::String("MasterRecCC");
    for (int i = 0; i < numSlots; i++)
    {
        int p = index - i * numParamsPerSlot;
        switch (p)
        {
            case kRecord:
                return juce::String("Record") + juce::String(i + 1);
            case kPlay:
                return juce::String("Plajuce::y") + juce::String(i + 1);
            case kTranspose:
                return juce::String("Transpose") + juce::String(i + 1);
            case kOctave:
                return juce::String("OctaveShift") + juce::String(i + 1);
            case kVelocity:
                return juce::String("VelocityScale") + juce::String(i + 1);
            case kVeloSens:
                return juce::String("VelocitySens") + juce::String(i + 1);
            case kShift:
                return juce::String("BeatShift") + juce::String(i + 1);
            case kLoopStart:
                return juce::String("LoopStart") + juce::String(i + 1);
            case kLoopEnd:
                return juce::String("LoopEnd") + juce::String(i + 1);
            case kStretch:
                return juce::String("Stretch") + juce::String(i + 1);
            case kTrigger:
                return juce::String("LoopMode") + juce::String(i + 1);
            case kNoteTrig:
                return juce::String("NoteTrigger") + juce::String(i + 1);
            case kRoot:
                return juce::String("RootNote") + juce::String(i + 1);
            case kNLow:
                return juce::String("LowNote") + juce::String(i + 1);
            case kNHigh:
                return juce::String("HighNote") + juce::String(i + 1);
            case kChannel:
                return juce::String("Channel") + juce::String(i + 1);
            case kTrigChan:
                return juce::String("NoteTrigChannel") + juce::String(i + 1);
            case kFiltChan:
                return juce::String("ChannelMode") + juce::String(i + 1);
            case kFullRelease:
                return juce::String("FullRelease") + juce::String(i + 1);
            case kWaitForBar:
                return juce::String("WaitForBar") + juce::String(i + 1);
            case kNumLoops:
                return juce::String("NumLoops") + juce::String(i + 1);
            case kNextSlot:
                return juce::String("NextSlot") + juce::String(i + 1);
            case kPlayGroup:
                return juce::String("PlayGroup") + juce::String(i + 1);
            case kMuteGroup:
                return juce::String("MuteGroup") + juce::String(i + 1);
            case kForceToKey:
                return juce::String("ForceToScale") + juce::String(i + 1);
            case kForceToScaleMode:
                return juce::String("ForceMode") + juce::String(i + 1);
            case kScaleChannel:
                return juce::String("ScaleChannel") + juce::String(i + 1);
            case kTransposeChannel:
                return juce::String("TransposeChannel") + juce::String(i + 1);
            case kUseScaleChannel:
                return juce::String("UseScaleChan") + juce::String(i + 1);
            case kUseTrChannel:
                return juce::String("UseTransChan") + juce::String(i + 1);
            case kNote0:
                return juce::String("C") + juce::String(i + 1);
            case kNote1:
                return juce::String("Csharp") + juce::String(i + 1);
            case kNote2:
                return juce::String("D") + juce::String(i + 1);
            case kNote3:
                return juce::String("Dsharp") + juce::String(i + 1);
            case kNote4:
                return juce::String("E") + juce::String(i + 1);
            case kNote5:
                return juce::String("F") + juce::String(i + 1);
            case kNote6:
                return juce::String("Fsharp") + juce::String(i + 1);
            case kNote7:
                return juce::String("G") + juce::String(i + 1);
            case kNote8:
                return juce::String("Gsharp") + juce::String(i + 1);
            case kNote9:
                return juce::String("A") + juce::String(i + 1);
            case kNote10:
                return juce::String("Asharp") + juce::String(i + 1);
            case kNote11:
                return juce::String("B") + juce::String(i + 1);
            case kImmediateTranspose:
                return juce::String("SplitTrnsp") + juce::String(i + 1);
            case kTranspose10:
                return juce::String("Ch10Transpose") + juce::String(i + 1);
            case kReverse:
                return juce::String("Reverse") + juce::String(i + 1);
            case kNoteToggle:
                return juce::String("NoteToggle") + juce::String(i + 1);
            default:
                break;
        }
    }
    return juce::String();
}

const juce::String PizLooper::getParameterText(int index)
{
    if (index == kMasterVelocity)
        return juce::String(roundToInt(getParameter(index) * 200.f)) + juce::String("%");
    if (index == kThru)
    {
        if (getParameter(index) < 0.5f)
            return juce::String("Off");
        return juce::String("On");
    }
    if (index == kMonitor)
    {
        if (getParameter(index) < 0.5f)
            return juce::String("Off");
        return juce::String("On");
    }
    if (index == kSingleLoop)
    {
        if (getParameter(index) < 0.5f)
            return juce::String("Multi");
        return juce::String("Single");
    }
    if (index == kPlayMain)
    {
        if (getParameter(index) < 0.5f)
            return juce::String("Stopped");
        return juce::String("Playing");
    }
    if (index == kRecordMain)
    {
        if (getParameter(index) < 0.5f)
            return juce::String("Off");
        return juce::String("Recording");
    }
    if (index == kPlayCC)
    {
        if (playCC == -2)
            return juce::String("Learn...");
        if (playCC == -1)
            return juce::String("No CC");
        return "CC " + juce::String(playCC);
    }
    if (index == kRecCC)
    {
        if (recCC == -2)
            return juce::String("Learn...");
        if (recCC == -1)
            return juce::String("No CC");
        return "CC " + juce::String(recCC);
    }
    if (index == kFile)
    {
        if (getParameter(index) < 0.4f)
            return juce::String("<-Clear");
        if (getParameter(index) > 0.6f)
            return juce::String("Load->");
        if (getParameter(index) == 0.0f)
            return juce::String("Clear!");
        if (getParameter(index) == 1.0f)
            return juce::String("Load!");
        return juce::String("<-Clear/Load->");
    }
    if (index == kSave)
    {
        return juce::String("Save->");
    }
    if (index == kSync)
    {
        if (getParameter(index) < 0.25f)
            return juce::String("PPQ (Host 0)");
        if (getParameter(index) < 0.5f)
            return juce::String("PPQ (Recstart)");
        return juce::String("Sample");
    }
    if (index == kRecStep)
    {
        if (getParameter(index) < 0.1)
            return juce::String("1 Bar");
        if (getParameter(index) < 0.2)
            return juce::String("3 Beats");
        if (getParameter(index) < 0.3)
            return juce::String("2 Beats");
        if (getParameter(index) < 0.4)
            return juce::String("1 Beat");
        if (getParameter(index) < 0.5)
            return juce::String("8th Note");
        if (getParameter(index) < 0.6)
            return juce::String("16th Note");
        return juce::String("1 Tick");
    }
    if (index == kQuantize)
    {
        if (getParameter(index) == 0.0)
            return juce::String("Off");
        if (getParameter(index) < 0.3)
            return juce::String("8th");
        if (getParameter(index) < 0.6)
            return juce::String("16th");
        if (getParameter(index) < 0.9)
            return juce::String("32nd");
        return juce::String("64th");
    }
    if (index == kFixedLength)
    {
        const int l = roundToInt(getParameter(index) * 32);
        if (l == 0)
            return juce::String("Manual");
        if (getParameter(kRecStep) < 0.1)
            return juce::String(l) + juce::String(" Bar") + (l > 1 ? "s" : "");
        if (getParameter(kRecStep) < 0.2)
            return juce::String(l * 3) + juce::String(" Beats");
        if (getParameter(kRecStep) < 0.3)
            return juce::String(l * 2) + juce::String(" Beats");
        if (getParameter(kRecStep) < 0.4)
            return juce::String(l) + juce::String(" Beat") + (l > 1 ? "s" : "");
        if (getParameter(kRecStep) < 0.5)
            return juce::String(l) + juce::String(" 8th Note") + (l > 1 ? "s" : "");
        if (getParameter(kRecStep) < 0.6)
            return juce::String(l) + juce::String(" 16th Note") + (l > 1 ? "s" : "");
        else
            return juce::String(l) + juce::String(" Tick") + (l > 1 ? "s" : "");
    }
    if (index == kRecMode)
    {
        if (getParameter(index) < 0.5)
            return juce::String("Replace");
        if (getParameter(index) < 0.8)
            return juce::String("Overdub(Keep Length)");
        return juce::String("Overdub");
    }
    if (index == kMasterTranspose)
    {
        if (roundToInt(getParameter(index) * 24.0f) - 12 < 1)
            return juce::String(roundToInt(getParameter(index) * 24.0f) - 12);
        return juce::String("+") + juce::String(roundToInt(getParameter(index) * 24.0f) - 12);
    }
    if (index == kMidiOutDevice)
    {
        if (param[kMidiOutDevice] > 0.f)
            return juce::String("On");
        return juce::String("Off");
    }

    for (int i = 0; i < numSlots; i++)
    {
        int p = index - i * numParamsPerSlot;
        int value;
        switch (p)
        {
            case kRecord:
            case kPlay:
            //case kSnap:
            case kForceToKey:
            case kReverse:
            case kFullRelease:
            case kNoteToggle:
            case kUseScaleChannel:
            case kUseTrChannel:
            case kImmediateTranspose:
            case kTranspose10:
                if (getParameter(index) < 0.5)
                    return juce::String("Off");
                return juce::String("On");
            case kParamsToHost:
                if (! getParameter(index))
                    return juce::String("Off");
                if (getParameter(index) < 0.5f)
                    return juce::String("Only From GUI");
                return juce::String("Always");
            case kTranspose:
                value = roundToInt(getParameter(index) * 24.0f) - 12;
                if (value < 1)
                    return juce::String(value);
                return juce::String("+") + juce::String(value);
            case kOctave:
                value = roundToInt(getParameter(index) * 8.0f) - 4;
                if (value < 1)
                    return juce::String(value);
                return juce::String("+") + juce::String(value);
            case kVelocity:
            case kVeloSens:
                return juce::String(roundToInt(getParameter(index) * 200.f)) + juce::String("%");
            case kShift:
                value = roundToInt(getParameter(index) * 16.0f) - 8;
                if (value < 1)
                    return juce::String(value);
                return juce::String("+") + juce::String(value);
            case kLoopStart:
                value = roundToInt(getParameter(index) * 16.0f) - 8;
                if (value < 1)
                    return juce::String(value);
                return juce::String("+") + juce::String(value);
            case kLoopEnd:
                value = roundToInt(getParameter(index) * 16.0f) - 8;
                if (value < 1)
                    return juce::String(value);
                return juce::String("+") + juce::String(value);
            case kStretch:
                if (getParameter(index) == 0.00f)
                    return juce::String("x 1/16");
                if (getParameter(index) <= 0.05f)
                    return juce::String("x 1/12");
                if (getParameter(index) <= 0.10f)
                    return juce::String("x 1/8");
                if (getParameter(index) <= 0.15f)
                    return juce::String("x 1/6");
                if (getParameter(index) <= 0.20f)
                    return juce::String("x 1/5");
                if (getParameter(index) <= 0.25f)
                    return juce::String("x 1/4");
                if (getParameter(index) <= 0.30f)
                    return juce::String("x 1/3");
                if (getParameter(index) <= 0.35f)
                    return juce::String("x 1/2");
                if (getParameter(index) <= 0.40f)
                    return juce::String("x 2/3");
                if (getParameter(index) <= 0.45f)
                    return juce::String("x 3/4");
                if (getParameter(index) <= 0.50f)
                    return juce::String("x 1.0");
                if (getParameter(index) <= 0.55f)
                    return juce::String("x 1.33");
                if (getParameter(index) <= 0.60f)
                    return juce::String("x 1.5");
                if (getParameter(index) <= 0.65f)
                    return juce::String("x 2.0");
                if (getParameter(index) <= 0.70f)
                    return juce::String("x 3.0");
                if (getParameter(index) <= 0.75f)
                    return juce::String("x 4.0");
                if (getParameter(index) <= 0.80f)
                    return juce::String("x 5.0");
                if (getParameter(index) <= 0.85f)
                    return juce::String("x 6.0");
                if (getParameter(index) <= 0.90f)
                    return juce::String("x 8.0");
                if (getParameter(index) <= 0.95f)
                    return juce::String("x 12.0");
                return juce::String("x 16.0");
            case kTrigger:
                if (getParameter(index) == 0.0f)
                    return juce::String("Loop after rec");
                if (getParameter(index) < 0.1f)
                    return juce::String("Sync loop");
                if (getParameter(index) < 0.2f)
                    return juce::String("Unsync 1-shot");
                if (getParameter(index) < 0.3f)
                    return juce::String("Unsync loop");
                //if (getParameter(index)<0.5f) return String("sync 1-shot");
                return juce::String();
            case kNoteTrig:
                if (getParameter(index) == 0.0f)
                    return juce::String("Off");
                if (getParameter(index) < 0.1f)
                    return juce::String("Mono (Transpose)");
                if (getParameter(index) < 0.2f)
                    return juce::String("Poly (Transpose)");
                if (getParameter(index) < 0.3f)
                    return juce::String("Mono (Orig. Key)");
                //if (getParameter(index)<0.4f) return String("Poly (Orig. Key)");
                return juce::String();
            case kRoot:
            case kNLow:
            case kNHigh:
                value = floatToMidi(getParameter(index));
                if (value == -1)
                    return juce::String("Waiting...");
                return juce::String(value) + juce::String(" (") + getNoteName(value, bottomOctave) + juce::String(")");
            case kChannel:
                if (roundToInt(getParameter(index) * 16.0f) == 0)
                    return juce::String("All");
                return juce::String(roundToInt(getParameter(index) * 16.0f));
            case kNumLoops:
                if (roundToInt(getParameter(index) * 64.0f) == 0)
                    return juce::String("Forever");
                return "x" + juce::String(roundToInt(getParameter(index) * 64.0f));
            case kNextSlot:
                if (roundToInt(getParameter(index) * (float) numSlots) == 0)
                    return juce::String("Stop");
                return "Play Slot " + juce::String(roundToInt(getParameter(index) * (float) numSlots));
            case kMuteGroup:
            case kPlayGroup:
                if (roundToInt(getParameter(index) * 16.0f) == 0)
                    return juce::String("Off");
                return juce::String(roundToInt(getParameter(index) * 16.0f));
            case kTrigChan:
                return juce::String(roundToInt(getParameter(index) * 15.0f) + 1);
            case kFiltChan:
                if (getParameter(index) < 0.5f)
                    return juce::String("Transform");
                return juce::String("Filter");
            case kWaitForBar:
                if (getParameter(index) < 0.5f)
                    return juce::String("No");
                return juce::String("Yes");
            case kScaleChannel:
                //if (roundToInt(getParameter(index)*16.0f)==0) return String("Off");
                return juce::String(roundToInt(getParameter(index) * 15.0f) + 1);
            case kTransposeChannel:
                //if (roundToInt(getParameter(index)*16.0f)==0) return String("Off");
                return juce::String(roundToInt(getParameter(index) * 15.0f) + 1);
            case kForceToScaleMode:
                int mode;
                mode = roundToInt(getParameter(index) * (float) (numForceToKeyModes - 1));
                if (mode == nearest)
                    return juce::String("Nearest");
                else if (mode == alwaysup)
                    return juce::String("Up");
                else if (mode == alwaysdown)
                    return juce::String("Down");
                else if (mode == block)
                    return juce::String("Block");
                return juce::String();
            default:
                break;
        }
    }
    return juce::String();
}

const juce::String PizLooper::getInputChannelName(const int channelIndex) const
{
    return juce::String("Input") + juce::String(channelIndex + 1);
}

const juce::String PizLooper::getOutputChannelName(const int channelIndex) const
{
    return juce::String("Output") + juce::String(channelIndex + 1);
}

bool PizLooper::isInputChannelStereoPair(int index) const
{
    return true;
}

bool PizLooper::isOutputChannelStereoPair(int index) const
{
    return true;
}

//======================Programs================================================
void PizLooper::resetCurrentProgram(int index)
{
    if (index < getNumPrograms())
    {
        ////save non-parameter info to the old program, except the first time
        if (! init)
        {
            //nothing
        }
        init = false;

        //then set the new program
        JuceProgram* ap = &programs[index];
        curProgram      = index;

        setActiveDevice(ap->device);
        for (int i = 0; i < numGlobalParams + numParamsPerSlot; i++)
        {
            if (i < numGlobalParams)
                param[i] = ap->param[i];
            else
            {
                param[i + curProgram * numParamsPerSlot] = ap->param[i];
            }
        }
        keySelectorState.reset();
        for (int i = 0; i < 12; i++)
            if (getParameterForSlot(kNote0 + i, curProgram) >= 0.5f)
                keySelectorState.noteOn(1, i, 1.f);
        playCC = roundToInt(param[kPlayCC] * 129.f) - 2;
        recCC  = roundToInt(param[kRecCC] * 129.f) - 2;
        ap->loop.setSemitones(roundToInt(ap->param[kTranspose] * 24.f) - 12);
        ap->loop.setOctaves(roundToInt(ap->param[kOctave] * 8.f) - 4);
        kbstate.reset();
        currentLength    = ap->looplength;
        currentNumEvents = ap->loop.getNumEvents();
        killNotes(curProgram);

        sendChangeMessage();
    }
}

void PizLooper::changeProgramName(int index, const juce::String& newName)
{
    if (index < getNumPrograms())
    {
        programs[index].name = newName;
        sendChangeMessage();
        updateHostDisplay();
    }
}

const juce::String PizLooper::getProgramName(int index)
{
    if (index < getNumPrograms())
    {
        return programs[index].name;
    }
    return juce::String();
}

int PizLooper::getCurrentProgram()
{
    return curProgram;
}

//==============================================================================
juce::AudioProcessorEditor* PizLooper::createEditor()
{
#if JucePlugin_NoEditor
    return 0;
#else
    return new PizLooperEditor(this);
#endif
}

//==============================================================================
void PizLooper::getStateInformation(juce::MemoryBlock& destData)
{
    // make sure the non-parameter settings are copied to the current program
    for (int i = 0; i < 12; i++)
    {
        param[kNote0 + i + curProgram * numParamsPerSlot] = keySelectorState.isNoteOn(1, i) ? 1.f : 0.f;
    }

    //save patterns
    for (int i = 0; i < numPrograms; i++)
    {
        if (programs[i].loop.getNumEvents() > 0)
        {
            juce::MemoryBlock midiData(512, true);
            juce::MemoryOutputStream m(midiData, false);

            juce::MidiFile midifile;
            midifile.setTicksPerQuarterNote(960);

            juce::MidiMessageSequence midi(programs[i].loop.getAsJuceSequence());
            midifile.addTrack(midi);
            midifile.writeTo(m);
            int dataSize = (int) midiData.getSize();
            destData.append(&dataSize, sizeof(int));
            destData.append(midiData.getData(), dataSize);
        }
        else
        {
            int dataSize = 0;
            destData.append(&dataSize, sizeof(int));
        }
    }

    juce::MemoryBlock xmlData(512);

    juce::XmlElement xmlState("midiLooperSettings");
    xmlState.setAttribute("pluginVersion", 4);
    xmlState.setAttribute("program", getCurrentProgram());
    for (int i = 0; i < numGlobalParams; i++)
    {
        xmlState.setAttribute(getParameterName(i), getParameter(i));
    }
    xmlState.setAttribute("uiWidth", lastUIWidth);
    xmlState.setAttribute("uiHeight", lastUIHeight);
    for (int p = 0; p < getNumPrograms(); p++)
    {
        auto* xmlProgram = new juce::XmlElement("Slot" + juce::String(p));
        xmlProgram->setAttribute(juce::String("name"), programs[p].name);
        for (int i = 0; i < numParamsPerSlot; i++)
        {
            int index = i + curProgram * numParamsPerSlot + numGlobalParams;
            xmlProgram->setAttribute(getParameterName(index).removeCharacters("0123456789"),
                                     getParameterForSlot(i + numGlobalParams, p));
        }
        xmlProgram->setAttribute("looplength", programs[p].looplength);
        xmlProgram->setAttribute("loopstart1", programs[p].loopstart);
        xmlProgram->setAttribute("measureFromHere", programs[p].measureFromHere);
        xmlProgram->setAttribute("numerator", programs[p].numerator);
        xmlProgram->setAttribute("denominator", programs[p].denominator);
        xmlProgram->setAttribute("device", programs[p].device.name);

        xmlProgram->addChildElement(programs[p].PRSettings.createXml().release());

        xmlState.addChildElement(xmlProgram);
        //if (programs[p].loop.getNumEvents()>0) writeMidiFile(p);
    }
    copyXmlToBinary(xmlState, xmlData);
    destData.append(xmlData.getData(), xmlData.getSize());
#ifdef _DEBUG
    // TODO: macOS/Linux compatibility
    xmlState.writeTo(juce::File("C:\\loopergetState.xml"));
#endif
}

void PizLooper::setStateInformation(const void* data, int sizeInBytes)
{
    if (init)
    {
        for (int p = 0; p < getNumPrograms(); p++)
        {
            for (int i = 0; i < numParamsPerSlot + numGlobalParams; i++)
            {
                if (i < numGlobalParams)
                    param[i] = programs[p].param[i];
                else
                    param[i + numParamsPerSlot * p] = programs[p].param[i];
            }
        }
    }

    auto xmlState = getXmlFromBinary(data, sizeInBytes);
    if (xmlState == 0)
    {
        auto* datab       = (juce::uint8*) data;
        int totalMidiSize = 0;
        bool oldBank      = false;
        for (int i = 0; i < numPrograms; i++)
        {
            programs[i].loop.clear();
            if (i == 16)
            {
                xmlState = getXmlFromBinary(datab, sizeInBytes - totalMidiSize);
                if (xmlState != 0)
                    oldBank = true;
            }
            if (! oldBank)
            {
                int midiSize = *((int*) datab);
                datab += sizeof(midiSize);
                if (midiSize > 0)
                {
                    totalMidiSize += sizeof(midiSize);
                    juce::MemoryInputStream m(datab, midiSize, true);
                    juce::MidiFile midifile;
                    if (midifile.readFrom(m))
                    {
                        programs[i].loop.addSequence(*midifile.getTrack(midifile.getNumTracks() - 1), 0, 0, midifile.getLastTimestamp() + 1);
                        programs[i].loop.updateMatchedPairs();
                        juce::uint8 bogus[3] = { 0xff, 0x2f, 0x00 };
                        int lastIndex        = programs[i].loop.getNumEvents() - 1;
                        const juce::uint8* lastmsg; /* = programs[i].loop.getEventPointer(lastIndex)->message.getRawData();*/
                        bool done = false;
                        while (lastIndex >= 0 && ! done)
                        {
                            lastmsg = programs[i].loop.getEventPointer(lastIndex)->message.getRawData();
                            if (memcmp(bogus, lastmsg, sizeof(bogus)) == 0)
                                programs[i].loop.deleteEvent(lastIndex, false);
                            else
                                done = true;
                            lastIndex--;
                        }
                    }
                    datab += midiSize;
                    totalMidiSize += midiSize;
                }
            }
        }
        if (xmlState == 0)
            xmlState = getXmlFromBinary(datab, sizeInBytes - totalMidiSize);
    }

    if (xmlState != 0)
    {
        if (xmlState->hasTagName("midiLooperSettings"))
        {
            const int versionNumber = xmlState->getIntAttribute("pluginVersion", 0);
            for (int p = 0; p < getNumPrograms(); p++)
            {
                for (int i = 0; i < numGlobalParams; i++)
                {
                    param[i] = programs[p].param[i] = (float) xmlState->getDoubleAttribute(getParameterName(i), param[i]);
                }
                lastUIWidth  = xmlState->getIntAttribute("uiWidth", lastUIWidth);
                lastUIHeight = xmlState->getIntAttribute("uiHeight", lastUIHeight);
                juce::XmlElement* xmlProgram(xmlState->getChildByName("Slot" + juce::String(p)));
                if (xmlProgram)
                {
                    for (int i = numGlobalParams; i < numParamsPerSlot + numGlobalParams; i++)
                    {
                        param[i + numParamsPerSlot * p] = programs[p].param[i] = (float) xmlProgram->getDoubleAttribute(getParameterName(i).removeCharacters("0123456789"), param[i + numParamsPerSlot * p]);
                        if (versionNumber < 4 && kTransposeChannel == i)
                            param[i + numParamsPerSlot * p] = programs[p].param[i] = programs[p].param[kScaleChannel];
                    }
                    param[kRecord + numParamsPerSlot * p] = programs[p].param[kRecord] = 0.0f;
                    param[kFile] = programs[p].param[kFile] = 0.5f;
                    param[kSave] = programs[p].param[kSave] = 0.f;
                    setLoopLength(p, xmlProgram->getDoubleAttribute(juce::String("looplength"), getLoopLength(p)));
                    setLoopStart(p, xmlProgram->getDoubleAttribute(juce::String("loopstart1"), getLoopStart(p)));
                    programs[p].measureFromHere = xmlProgram->getDoubleAttribute(juce::String("measureFromHere"), programs[p].measureFromHere);
                    programs[p].denominator     = xmlProgram->getIntAttribute("denominator", programs[p].denominator);
                    programs[p].numerator       = xmlProgram->getIntAttribute("numerator", programs[p].numerator);
                    programs[p].name            = xmlProgram->getStringAttribute(juce::String("name"), programs[p].name);
                    programs[p].loop.setSemitones(roundToInt(param[kTranspose + numParamsPerSlot * p] * 24.f) - 12);
                    programs[p].loop.setOctaves(roundToInt(param[kOctave + numParamsPerSlot * p] * 8.f) - 4);
                    programs[p].device = getDeviceByName(xmlProgram->getStringAttribute("device", programs[p].device.name));

                    programs[p].PRSettings.loadXml(*xmlProgram);

                    if (programs[p].loop.getNumEvents() == 0)
                        readMidiFile(p, programs[p].name);
                }
            }
            init = true;
            resetCurrentProgram(xmlState->getIntAttribute(juce::String("program"), curProgram));
        }
        else if (xmlState->hasTagName("MYPLUGINSETTINGS"))
        {
            for (int p = 0; p < 16; p++)
            {
                juce::String prefix = juce::String("Program") + juce::String(p) + juce::String("_");
                for (int i = 0; i < numParamsPerSlot + numGlobalParams; i++)
                {
                    if (i < numGlobalParams)
                        param[i] = programs[p].param[i] = (float) xmlState->getDoubleAttribute(prefix + getParameterName(i), param[i]);
                    else
                        param[i + numParamsPerSlot * p] = programs[p].param[i] = (float) xmlState->getDoubleAttribute(prefix + getParameterName(i).removeCharacters("0123456789"), param[i + numParamsPerSlot * p]);
                }
                param[kRecord + numParamsPerSlot * p] = programs[p].param[kRecord] = 0.0f;
                param[kFile] = programs[p].param[kFile] = 0.5f;
                param[kSave] = programs[p].param[kSave] = 0.f;
                setLoopLength(p, xmlState->getDoubleAttribute(juce::String("looplength"), getLoopLength(p)));
                setLoopStart(p, xmlState->getDoubleAttribute(juce::String("loopstart1"), getLoopStart(p)));
                programs[p].measureFromHere = xmlState->getDoubleAttribute(prefix + juce::String("measureFromHere"), programs[p].measureFromHere);
                lastUIWidth                 = xmlState->getIntAttribute(prefix + "uiWidth", lastUIWidth);
                lastUIHeight                = xmlState->getIntAttribute(prefix + "uiHeight", lastUIHeight);
                programs[p].denominator     = xmlState->getIntAttribute(prefix + "denominator", programs[p].denominator);
                programs[p].numerator       = xmlState->getIntAttribute(prefix + "numerator", programs[p].numerator);
                programs[p].name            = xmlState->getStringAttribute(prefix + juce::String("progname"), programs[p].name);
                programs[p].loop.setSemitones(roundToInt(param[kTranspose + numParamsPerSlot * p] * 24.f) - 12);
                programs[p].loop.setOctaves(roundToInt(param[kOctave + numParamsPerSlot * p] * 8.f) - 4);
                programs[p].device = getDeviceByName(xmlState->getStringAttribute(prefix + "device", programs[p].device.name));

                readMidiFile(p, programs[p].name);
            }
            init = true;
            resetCurrentProgram(xmlState->getIntAttribute(juce::String("program"), curProgram));
        }
#ifdef _DEBUG
        xmlState->writeTo(juce::File("C:\\loopersetState.xml"));
#endif
    }
}

//==============================================================================
bool PizLooper::writeMidiFile(int index, juce::File file, bool IncrementFilename)
{
    juce::MidiFile midifile;
    midifile.setTicksPerQuarterNote(960);

    juce::uint8 dd = 0;
    int d          = getDenominator(index);
    while (d > 1)
    {
        d = d >> 1;
        dd++;
    }

    juce::MidiMessageSequence timesigtrack;
    juce::uint8 ts[] = { 0xFF, 0x58, 0x04, (juce::uint8) getNumerator(index), dd, 0x18, 0x08 };
    timesigtrack.addEvent(juce::MidiMessage(ts, 7, 0));
    midifile.addTrack(timesigtrack);

    juce::MidiMessageSequence metadata;
    juce::uint8 ID[] = { 0xFF, 0x03, 9, 'l', 'o', 'o', 'p', ' ', 'a', 'r', 'e', 'a' };
    metadata.addEvent(juce::MidiMessage(ID, 12, 0));
    int ls = roundToInt((programs[index].loopstart - programs[index].measureFromHere) * 960.0);
    metadata.addEvent(juce::MidiMessage(0x9f, 62, 1, 0), (double) ls);
    int ll = int((programs[index].looplength) * 960.0);
    metadata.addEvent(juce::MidiMessage(0x8f, 62, 1, 0), (double) (ls + ll));
    metadata.updateMatchedPairs();
    midifile.addTrack(metadata);

    juce::uint8 tn[] = { 0xFF, 0x03, 4, 'l', 'o', 'o', 'p' };
    programs[index].loop.addEvent(juce::MidiMessage(tn, 7, 0));
    midifile.addTrack(programs[index].loop.getAsJuceSequence());

    if (! juce::File(loopDir).exists())
        juce::File(loopDir).createDirectory();
    if (file.isDirectory())
    {
        juce::String filename = file.getFullPathName() + juce::File::getSeparatorString() + getProgramName(index);
        if (IncrementFilename)
            filename << "_" << FilenameIndex++;
        filename << ".mid";
        file = juce::File(filename);
    }
    if (! file.deleteFile())
        file.copyFileTo(juce::File(file.getParentDirectory().getFullPathName()
                                   + juce::File::getSeparatorString() + file.getFileNameWithoutExtension() + juce::String("_old.mid")));
    if (file.create())
    {
        juce::FileOutputStream f(file);
        midifile.writeTo(f);
    }
    return true;
}

void PizLooper::loadMidiFile(juce::File file)
{
    if (readMidiFile(curProgram, getProgramName(curProgram), file))
    {
        changeProgramName(curProgram, file.getFileNameWithoutExtension());
        updateHostDisplay();
        sendChangeMessage();
    }
}

bool PizLooper::readMidiFile(int index, juce::String progname, juce::File mid)
{
    double ppqPerBar = 4.0;
    double l         = ppqPerBar * (double) getPRSetting("bars");
    if (! mid.exists())
    {
        juce::String path = ((juce::File::getSpecialLocation(juce::File::currentExecutableFile)).getParentDirectory()).getFullPathName()
                          + juce::File::getSeparatorString() + juce::String("midiloops");
        mid = juce::File(path + juce::File::getSeparatorString() + getProgramName(index) + juce::String(".mid"));
    }
    if (mid.exists())
    {
        programs[index].loop.clear();
        programs[index].looplength = 0;
        programs[index].loopstart  = 0;
        juce::MidiFile midifile;
        {
            juce::FileInputStream f(mid);
            midifile.readFrom(f);
        }
        //look for time signature & tempo
        for (int i = 0; i < midifile.getTrack(0)->getNumEvents(); i++)
        {
            const juce::MidiMessage ID = midifile.getTrack(0)->getEventPointer(i)->message;
            if (ID.isTimeSignatureMetaEvent())
            {
                ID.getTimeSignatureInfo(programs[index].numerator, programs[index].denominator);
                ppqPerBar = (double) getNumerator(index) * 4.0 / (double) getDenominator(index);
                break;
            }
            else if (ID.isTempoMetaEvent())
            {
                double bpm = 60.0 / ID.getTempoSecondsPerQuarterNote();
            }
        }
        if (midifile.getNumTracks() == 1)
            programs[index].loop.addSequence(*midifile.getTrack(0), 0, 0, midifile.getLastTimestamp() + 1);
        else
        {
            for (int t = 0; t < midifile.getNumTracks(); t++)
            {
                bool loopAreaTrack                     = false;
                const juce::MidiMessageSequence* track = midifile.getTrack(t);
                for (int i = 0; i < track->getNumEvents(); i++)
                {
                    const juce::MidiMessage ID = track->getEventPointer(i)->message;
                    if (ID.isTrackNameEvent())
                    {
                        if (ID.getTextFromTextMetaEvent() == juce::String("loop area"))
                        {
                            //track->convertTimeBase(midifile.getTimeFormat());
                            double t1 = 0;
                            double t2 = 0;
                            for (int e = 0; e < track->getNumEvents(); e++)
                            {
                                juce::MidiMessage mm = track->getEventPointer(e)->message;
                                if (mm.isNoteOn())
                                {
                                    t1 = mm.getTimeStamp();
                                    setLoopStart(index, t1 / 960.0);
                                }
                                else if (mm.isNoteOff())
                                {
                                    t2 = mm.getTimeStamp();
                                    //round length to two decimal places
                                    setLoopLength(index, (double) (roundToInt((t2 - t1) / 9.6)) * 0.01);
                                    programs[index].measureFromHere = 0;
                                    loopAreaTrack                   = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                if (! loopAreaTrack)
                    programs[index].loop.addSequence(*track, 0, 0, midifile.getLastTimestamp() + 1);
            }
        }
        programs[index].loop.updateMatchedPairs();
        programs[index].loop.convertTimeBase(midifile.getTimeFormat());

        l = ppqPerBar * ceil(programs[index].loop.getEndTime() / (960.0 * ppqPerBar));
        //guess looplength if it's not indicated
        if (programs[index].looplength == 0.0)
            setLoopLength(index, (double) roundToInt(l));
    }
    programs[index].loop.setLength(l);
    currentNumEvents = programs[curProgram].loop.getNumEvents();
    if (index == curProgram)
        newLoop = true;

    setPRSetting("bars", (double) roundToInt(l) / ppqPerBar, true);
    return true;
}

Loop* PizLooper::getActiveLoop()
{
    return &(programs[curProgram].loop);
}

void PizLooper::updateLoopInfo()
{
    currentNumEvents = programs[curProgram].loop.getNumEvents();
    currentLength    = programs[curProgram].looplength;
    //killNotes(curProgram);
}

double PizLooper::getStretchMultiplier(int slot)
{
    const float ss = getParameterForSlot(kStretch, slot);
    if (ss == 0.00f)
        return 0.0625;
    else if (ss <= 0.05f)
        return 0.0833333333333333333333333333333;
    else if (ss <= 0.10f)
        return 0.125;
    else if (ss <= 0.15f)
        return 0.1666666666666666666666666666667;
    else if (ss <= 0.20f)
        return 0.2;
    else if (ss <= 0.25f)
        return 0.25;
    else if (ss <= 0.30f)
        return 0.33333333333333333333333333333;
    else if (ss <= 0.35f)
        return 0.5;
    else if (ss <= 0.40f)
        return 0.66666666666666666666666666667;
    else if (ss <= 0.45f)
        return 0.75;
    else if (ss <= 0.50f)
        return 1.0;
    else if (ss <= 0.55f)
        return 1.33333333333333333333333333333;
    else if (ss <= 0.60f)
        return 1.5;
    else if (ss <= 0.65f)
        return 2.0;
    else if (ss <= 0.70f)
        return 3.0;
    else if (ss <= 0.75f)
        return 4.0;
    else if (ss <= 0.80f)
        return 5.0;
    else if (ss <= 0.85f)
        return 6.0;
    else if (ss <= 0.90f)
        return 8.0;
    else if (ss <= 0.95f)
        return 12.0;
    else
        return 16.0;
}
