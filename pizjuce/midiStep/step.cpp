#include "step.h"
#include "stepgui.h"

//==============================================================================
/*
    This function must be implemented to create the actual plugin object that
    you want to use.
*/
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiStep();
}

//==============================================================================
JuceProgram::JuceProgram()
{
    //default program name
    name = "midiStep";

    //default values
    for (int i = 0; i < kNumParams; i++)
    {
        param[i] = 0.f;
    }
    for (int i = 0; i < numLoops; i++)
    {
        param[kTranspose + i] = 0.5f;
        param[kTriggerKey + i] = midiToFloat (0, true);
    }
    param[kRecArm] = 1.f;
    param[kRecActive] = 1.f;

    //default GUI size
    lastUIWidth = 610;
    lastUIHeight = 360;
}

//==============================================================================
MidiStep::MidiStep()
    : programs (0),
      lastUIWidth (610),
      lastUIHeight (360),
      curProgram (0),
      init (true),
      samples (0),
      wasRecording (false),
      activeLoop (0)
{
    for (int i = 0; i < numLoops; i++)
        loop.add (new Loop());

    loopDir = ((File::getSpecialLocation (File::currentExecutableFile)).getParentDirectory()).getFullPathName()
              + File::getSeparatorString() + "midiStep";
    String defaultBank = loopDir + File::getSeparatorString() + "default.fxb";
    programs = new JuceProgram[getNumPrograms()];
    if (File (defaultBank).exists())
    {
        MemoryBlock bank = MemoryBlock (0, true);
        File (defaultBank).loadFileAsData (bank);
        bank.removeSection (0, 0xA0);
        setStateInformation (bank.getData(), (int) bank.getSize());
    }
    else
        (loadDefaultFxb());

    if (File (loopDir).findChildFiles (midiFiles, File::findFiles, true, "*.mid"))
    {
        //for (int i=0;i<midiFiles.size();i++)
        //{

        //}
    }
    //start up with the first program
    setCurrentProgram (0);
}

MidiStep::~MidiStep()
{
    if (programs)
        delete[] programs;
}

//==============================================================================
float MidiStep::getParameter (int index)
{
    if (index < getNumParameters())
        return param[index];
    return 0.f;
}

void MidiStep::setParameter (int index, float newValue)
{
    if (index < getNumParameters())
    {
        JuceProgram* ap = &programs[curProgram];

        //if (param[index] != newValue) {
        switch (index)
        {
            case kActiveLoop:
                activeLoop = roundToInt (newValue * (float) (numLoops - 1));
                break;
            default:
                break;
        }
        param[index] = ap->param[index] = newValue;
        sendChangeMessage();
        //}
    }
}

const String MidiStep::getParameterName (int index)
{
    if (index == kRecord)
        return "Record";
    if (index == kRecActive)
        return "RecActive";
    if (index == kActiveLoop)
        return "ActiveLoop";
    if (index == kThru)
        return "Thru";
    else
    {
        String loopnum = String ((index - kNumGlobalParams) % 16 + 1);
        if (index < kChannel)
            return "RecArm" + loopnum;
        if (index < kTriggerKey)
            return "InChannel" + loopnum;
        if (index < kTranspose)
            return "TriggerKey" + loopnum;
        if (index < kOutChannel)
            return "Transpose" + loopnum;
        if (index < kNumParams)
            return "OutChannel" + loopnum;
    }
    return String();
}

const String MidiStep::getParameterText (int index)
{
    if (index == kRecord)
    {
        return param[kRecord] < 0.5f ? String ("Off") : String ("On");
    }
    if (index == kRecActive)
    {
        return param[kRecActive] < 0.5f ? String ("Off") : String ("On");
    }
    if (index == kActiveLoop)
    {
        if (roundToInt (param[kActiveLoop] * 16.0f) == 0)
            return String ("Any");
        else
            return String (roundToInt (param[kActiveLoop] * 16.0f));
    }
    if (index == kThru)
    {
        return param[kThru] < 0.5f ? String ("Off") : String ("On");
    }
    else
    {
        if (index < kChannel)
        { //rec arm
            return param[index] < 0.5f ? String ("Off") : String ("On");
        }
        if (index < kTriggerKey)
        { //in channel
            if (roundToInt (param[index] * 16.0f) == 0)
                return String ("Any");
            else
                return String (roundToInt (param[index] * 16.0f));
        }
        if (index < kTranspose)
        { //trigger key
            if (floatToMidi (param[index], true) < 0)
                return String ("Learn...");
            return String (floatToMidi (param[index], true));
        }
        if (index < kOutChannel)
        { //transpose
            return String (roundToInt (param[index] * 96.f) - 48);
        }
        if (index < kNumParams)
        { //out channel
            if (roundToInt (param[index] * 16.0f) == 0)
                return String ("No Change");
            else
                return String (roundToInt (param[index] * 16.0f));
        }
    }
    return String();
}

const String MidiStep::getInputChannelName (const int channelIndex) const
{
    if (channelIndex < getNumInputChannels())
        return String (JucePlugin_Name) + String (" ") + String (channelIndex + 1);
    return String();
}

const String MidiStep::getOutputChannelName (const int channelIndex) const
{
    if (channelIndex < getNumOutputChannels())
        return String (JucePlugin_Name) + String (" ") + String (channelIndex + 1);
    return String();
}

bool MidiStep::isInputChannelStereoPair (int index) const
{
    return true;
}

bool MidiStep::isOutputChannelStereoPair (int index) const
{
    return true;
}

//======================Programs================================================
void MidiStep::setCurrentProgram (int index)
{
    //save non-parameter info to the old program, except the first time
    if (! init)
    {
        programs[curProgram].lastUIHeight = lastUIHeight;
        programs[curProgram].lastUIWidth = lastUIWidth;
    }
    init = false;

    //then set the new program
    JuceProgram* ap = &programs[index];
    curProgram = index;
    for (int i = 0; i < getNumParameters(); i++)
    {
        setParameter (i, ap->param[i]);
    }
    lastUIWidth = ap->lastUIWidth;
    lastUIHeight = ap->lastUIHeight;

    sendChangeMessage();
}

void MidiStep::changeProgramName (int index, const String& newName)
{
    if (index < getNumPrograms())
        programs[index].name = newName;
}

const String MidiStep::getProgramName (int index)
{
    if (index < getNumPrograms())
        return programs[index].name;
    return String();
}

int MidiStep::getCurrentProgram()
{
    return curProgram;
}

//==============================================================================
void MidiStep::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    keyboardState.reset();
}

void MidiStep::releaseResources()
{
    // when playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void MidiStep::processBlock (AudioSampleBuffer& buffer,
                             MidiBuffer& midiMessages)
{
    const int sampleFrames = buffer.getNumSamples();
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, sampleFrames);
    }

    AudioPlayHead::CurrentPositionInfo pos;
    if (getPlayHead() != 0 && getPlayHead()->getCurrentPosition (pos))
    {
        if (memcmp (&pos, &lastPosInfo, sizeof (pos)) != 0)
        {
            lastPosInfo = pos;
        }
    }
    else
    { //no timeinfo
        zeromem (&lastPosInfo, sizeof (lastPosInfo));
        lastPosInfo.timeSigNumerator = 4;
        lastPosInfo.timeSigDenominator = 4;
        lastPosInfo.bpm = 120;
        lastPosInfo.ppqPosition = samples * (120 / (60 * getSampleRate()));
    }
    //if (param[kSync]>=0.5f) { //sample sync
    //    lastPosInfo.ppqPosition = samples*(lastPosInfo.bpm/(60*getSampleRate()));
    //    lastPosInfo.isPlaying = true;
    //}

    //const int channel = floatToChannel(param[kChannel]);
    MidiBuffer output;

    for (int i = 0; i < numLoops; i++)
    {
        loop[i]->recChannel = floatToChannel (param[kChannel + i]);
        loop[i]->outChannel = floatToChannel (param[kOutChannel + i]);
        loop[i]->isRecArmed = param[kRecArm + i] >= 0.5f;
        loop[i]->setTriggerNote (floatToMidi (param[kTriggerKey + i], true));
        loop[i]->transpose = roundToInt (param[kTranspose + i] * 96.f) - 48;
    }

    if (param[kRecord] >= 0.5f && ! wasRecording)
    { //start recording
        for (int i = 0; i < numLoops; i++)
        {
            if (loop[i]->isRecArmed)
            {
                loop[i]->sendNoteOffMessagesToBuffer (output, 0);
                loop[i]->startRecording();
            }
        }
        sendChangeMessage();
    }
    else if (param[kRecord] < 0.5f && wasRecording)
    { //stop recording
        for (int i = 0; i < numLoops; i++)
        {
            loop[i]->isRecording = false;
        }
        sendChangeMessage();
    }
    wasRecording = param[kRecord] >= 0.5f;

    bool thru = param[kThru] >= 0.5f;
    for (auto&& msgMetadata : midiMessages)
    {
        auto msg = msgMetadata.getMessage();
        auto sample_number = msgMetadata.samplePosition;

        if (msg.isController())
        {
            if (msg.isAllNotesOff())
            {
                for (int i = 0; i < numLoops; i++)
                {
                    loop[i]->sendNoteOffMessagesToBuffer (output, sample_number);
                }
            }
        }
        else if (msg.isNoteOnOrOff())
        {
            const int n = msg.getNoteNumber();

            for (int i = 0; i < numLoops; i++)
            {
                Loop* L = loop[i];
                if (L->isRecording)
                {
                    if (msg.isForChannel (L->recChannel) || L->recChannel == 0)
                    {
                        L->addEvent (msg, L->recTime + sample_number * (960.0 * lastPosInfo.bpm / (60.0 * getSampleRate())));
                        if (msg.isNoteOnOrOff())
                        {
                            L->updateMatchedPairs();
                            sendChangeMessage();
                        }
                    }
                }
                else if (L->isTriggerNote (-1))
                {
                    //midi learn
                    L->setTriggerNote (n);
                    param[kTriggerKey + i] = midiToFloat (n, true);
                    sendChangeMessage();
                }
                else if (L->isTriggerNote (n) && L->getNumEvents() > 0 && L->playMode == playStep)
                {
                    thru = false;
                    if (msg.isNoteOn())
                    {
                        L->playAllNotesAtCurrentTime (output, sample_number, msg.getVelocity());
                    }
                    else
                    {
                        L->sendNoteOffMessagesToBuffer (output, sample_number);
                    }
                    sendChangeMessage();
                }
            }
            if (thru)
                output.addEvent (msg, sample_number);
        }
        else if (msg.isProgramChange())
        {
            if (msg.getProgramChangeNumber() < numLoops)
                setParameterNotifyingHost (kActiveLoop, (float) msg.getProgramChangeNumber() / (float) (numLoops - 1));
        }
        else
            output.addEvent (msg, sample_number);
    }

    midiMessages = output;

    samples += sampleFrames;
    for (int i = 0; i < numLoops; i++)
    {
        if (loop[i]->isRecording)
            loop[i]->recTime += (sampleFrames * 960.0 * lastPosInfo.bpm / (60.0 * getSampleRate()));
    }
}

//==============================================================================
AudioProcessorEditor* MidiStep::createEditor()
{
    return new StepEditor (this);
}

//==============================================================================
void MidiStep::getStateInformation (MemoryBlock& destData)
{
    // make sure the non-parameter settings are copied to the current program
    programs[curProgram].lastUIHeight = lastUIHeight;
    programs[curProgram].lastUIWidth = lastUIWidth;

    //save patterns
    for (int i = 0; i < numLoops; i++)
    {
        MemoryBlock midiData (512, true);
        MemoryOutputStream m (midiData, false);

        MidiFile midifile;
        midifile.setTicksPerQuarterNote (960);

        MidiMessageSequence midi (*loop[i]);
        midifile.addTrack (midi);
        midifile.writeTo (m);
        size_t dataSize = midiData.getSize();
        destData.append (&dataSize, sizeof (int));
        destData.append (midiData.getData(), dataSize);
    }

    MemoryBlock xmlData (512);

    XmlElement xmlState ("midiStepSettings");
    xmlState.setAttribute ("pluginVersion", 2);
    xmlState.setAttribute ("program", getCurrentProgram());
    for (int p = 0; p < getNumPrograms(); p++)
    {
        String prefix = "P" + String (p) + "_";
        xmlState.setAttribute (prefix + "progname", programs[p].name);
        for (int i = 0; i < kNumParams; i++)
        {
            xmlState.setAttribute (prefix + String (i), programs[p].param[i]);
        }
        xmlState.setAttribute (prefix + "uiWidth", programs[p].lastUIWidth);
        xmlState.setAttribute (prefix + "uiHeight", programs[p].lastUIHeight);
    }
    copyXmlToBinary (xmlState, xmlData);
    destData.append (xmlData.getData(), xmlData.getSize());
}

void MidiStep::setStateInformation (const void* data, int sizeInBytes)
{
    uint8* datab = (uint8*) data;
    int totalMidiSize = 0;
    for (int i = 0; i < numLoops; i++)
    {
        loop[i]->clear();
        int midiSize = *((int*) datab);
        datab += sizeof (int);
        totalMidiSize += sizeof (midiSize);
        MemoryInputStream m (datab, midiSize, true);
        MidiFile midifile;
        if (midifile.readFrom (m))
        {
            loop[i]->addSequence (*midifile.getTrack (midifile.getNumTracks() - 1), 0, 0, midifile.getLastTimestamp() + 1);
            //loop[i]->convertTimeBase(midifile.getTimeFormat());
            //loop[i]->deleteEvent(loop[i]->getNumEvents()-1,false);
            loop[i]->updateMatchedPairs();
        }
        datab += midiSize;
        totalMidiSize += midiSize;
    }

    auto const xmlState = getXmlFromBinary (datab, sizeInBytes - totalMidiSize);

    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName ("MYPLUGINSETTINGS") || xmlState->hasTagName ("midiStepSettings"))
        {
            for (int p = 0; p < getNumPrograms(); p++)
            {
                String prefix = "P" + String (p) + "_";
                for (int i = 0; i < kNumParams; i++)
                {
                    programs[p].param[i] = (float) xmlState->getDoubleAttribute (prefix + String (i), programs[p].param[i]);
                }
                programs[p].lastUIWidth = xmlState->getIntAttribute (prefix + "uiWidth", programs[p].lastUIWidth);
                programs[p].lastUIHeight = xmlState->getIntAttribute (prefix + "uiHeight", programs[p].lastUIHeight);
                programs[p].name = xmlState->getStringAttribute (prefix + "progname", programs[p].name);
            }
            init = true;
            setCurrentProgram (0);
        }
    }
}

//==============================================================================
bool MidiStep::writeMidiFile (int index, File& file)
{
    MidiFile midifile;
    midifile.setTicksPerQuarterNote (960);

    MidiMessageSequence midi (*loop[index]);
    uint8 tn[] = { 0xFF, 0x03, 4, 'l', 'o', 'o', 'p' };
    MidiMessage trackname = MidiMessage (tn, 7, 0);
    midi.addEvent (trackname);

    midifile.addTrack (midi);

    if (file.existsAsFile())
        if (! file.deleteFile())
            return false;
    if (file.create())
    {
        FileOutputStream fo (file);
        if (midifile.writeTo (fo))
            return true;
    }
    return false;
}

bool MidiStep::readMidiFile (int index, File& mid)
{
    if (mid.exists())
    {
        MidiFile midifile;
        FileInputStream fi (mid);
        if (midifile.readFrom (fi))
        {
            loop[index]->clear();
            loop[index]->addSequence (*midifile.getTrack (midifile.getNumTracks() - 1), 0, 0, midifile.getLastTimestamp() + 1);
            loop[index]->convertTimeBase (midifile.getTimeFormat());
            loop[index]->deleteEvent (loop[index]->getNumEvents() - 1, false);
            loop[index]->updateMatchedPairs();
            sendChangeMessage();
            return true;
        }
    }
    return false;
}
