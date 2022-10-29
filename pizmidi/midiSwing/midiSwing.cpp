#include "midiSwing.hpp"

//for fmod()
#include <cmath>

//for seeding rand()
#include <time.h>

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiSwing(audioMaster);
}

MidiSwingProgram::MidiSwingProgram()
{
    // default Program Values
    param[kMode]      = 0.4f;
    param[kValue]     = 11.5f / 21.f; //8
    param[kAmount]    = 0.0f;
    param[kRadius]    = 0.25f;
    param[kShape]     = 0.0f;
    param[kVelocity]  = 0.5f;
    param[kProb]      = 1.0f;
    param[kRandom]    = 0.0f;
    param[kChannel]   = 0.0f;
    param[kMaxAmount] = 0.0f;
    param[kOverlap]   = 0.0f;
    param[kOffset]    = 0.5f;
    // default program name
    strcpy(name, "Default");
}

//-----------------------------------------------------------------------------
MidiSwing::MidiSwing(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams), programs(0)
{
    programs = new MidiSwingProgram[numPrograms];

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
                switch (i)
                {
                    case 0:
                        sprintf(programs[i].name, "Swing");
                        programs[i].param[kAmount] = 1.0f;
                        programs[i].param[kRadius] = 0.0f;
                        //default, see above
                        break;
                    case 1:
                        sprintf(programs[i].name, "Sub-Light Swing");
                        programs[i].param[kValue]     = 14.5f / 21.f; //16
                        programs[i].param[kAmount]    = 1.0f;
                        programs[i].param[kRadius]    = 0.25f;
                        programs[i].param[kShape]     = 0.0f;
                        programs[i].param[kVelocity]  = 0.4f;
                        programs[i].param[kProb]      = 0.8f;
                        programs[i].param[kRandom]    = 0.1f;
                        programs[i].param[kMaxAmount] = 0.0f;
                        programs[i].param[kOffset]    = 0.5f;
                        break;
                    case 2:
                        sprintf(programs[i].name, "Light Swing");
                        programs[i].param[kValue]     = 11.5f / 21.f;
                        programs[i].param[kAmount]    = 1.0f;
                        programs[i].param[kRadius]    = 0.25f;
                        programs[i].param[kShape]     = 0.0f;
                        programs[i].param[kVelocity]  = 0.45f;
                        programs[i].param[kProb]      = 1.0f;
                        programs[i].param[kRandom]    = 0.05f;
                        programs[i].param[kMaxAmount] = 0.2f;
                        programs[i].param[kOffset]    = 0.5f;
                        break;
                    case 3:
                        sprintf(programs[i].name, "Heavy Swing");
                        programs[i].param[kValue]     = 11.5f / 21.f;
                        programs[i].param[kAmount]    = 1.0f;
                        programs[i].param[kRadius]    = 0.25f;
                        programs[i].param[kShape]     = 0.0f;
                        programs[i].param[kVelocity]  = 0.5f;
                        programs[i].param[kProb]      = 1.0f;
                        programs[i].param[kRandom]    = 0.0f;
                        programs[i].param[kMaxAmount] = 0.3f;
                        programs[i].param[kOffset]    = 0.5f;
                        break;
                    case 4:
                        sprintf(programs[i].name, "Ridiculous Swing");
                        programs[i].param[kValue]     = 11.5f / 21.f;
                        programs[i].param[kAmount]    = 1.0f;
                        programs[i].param[kRadius]    = 0.1f;
                        programs[i].param[kShape]     = 0.0f;
                        programs[i].param[kVelocity]  = 0.6f;
                        programs[i].param[kProb]      = 0.8f;
                        programs[i].param[kRandom]    = 0.1f;
                        programs[i].param[kMaxAmount] = 0.7f;
                        programs[i].param[kOffset]    = 0.5f;
                        break;
                    case 5:
                        sprintf(programs[i].name, "Ludicrous Swing");
                        programs[i].param[kValue]     = 9.5f / 21.f;
                        programs[i].param[kAmount]    = 0.95f;
                        programs[i].param[kRadius]    = 0.0f;
                        programs[i].param[kShape]     = 0.0f;
                        programs[i].param[kVelocity]  = 0.7f;
                        programs[i].param[kProb]      = 0.5f;
                        programs[i].param[kRandom]    = 0.2f;
                        programs[i].param[kMaxAmount] = 1.0f;
                        programs[i].param[kOffset]    = 0.5f;
                        break;
                    case 6:
                        sprintf(programs[i].name, "Quarter");
                        programs[i].param[kMode]      = 1.0f;
                        programs[i].param[kValue]     = 8.5f / 21.f;
                        programs[i].param[kAmount]    = 0.5f;
                        programs[i].param[kRadius]    = 0.5f;
                        programs[i].param[kShape]     = 0.0f;
                        programs[i].param[kVelocity]  = 0.6f;
                        programs[i].param[kProb]      = 1.0f;
                        programs[i].param[kRandom]    = 0.0f;
                        programs[i].param[kMaxAmount] = 0.0f;
                        programs[i].param[kOffset]    = 0.0f;
                        break;
                    case 7:
                        sprintf(programs[i].name, "Dime");
                        break;
                    case 8:
                        sprintf(programs[i].name, "Nickel");
                        break;
                    case 9:
                        sprintf(programs[i].name, "Tire Swing");
                        break;
                    case 10:
                        sprintf(programs[i].name, "Porch Swing");
                        break;
                    default:
                        sprintf(programs[i].name, "Program %d", i + 1);
                        break;
                }
            }
        }
        setProgram(0);
    }

    init();
}

//-----------------------------------------------------------------------------------------
MidiSwing::~MidiSwing()
{
    if (programs)
    {
        delete[] programs;
    }
}

//------------------------------------------------------------------------
void MidiSwing::setProgram(VstInt32 program)
{
    MidiSwingProgram* ap = &programs[program];

    curProgram = program;
    for (int i = 0; i < kNumParams; i++)
    {
        setParameter(i, ap->param[i]);
    }
}

//------------------------------------------------------------------------
void MidiSwing::setProgramName(char* name)
{
    vst_strncpy(programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiSwing::getProgramName(char* name)
{
    strcpy(name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool MidiSwing::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy(text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiSwing::setParameter(VstInt32 index, float value)
{
    MidiSwingProgram* ap = &programs[curProgram];
    param[index] = ap->param[index] = value;
    if (index == kValue)
    { //stepsize = "how many fit in half a beat"
        const float inc = 1.f / 21.f;
        if (param[kValue] < 1 * inc)
        {
            stepsize = 0.0416666667f; //0.3333333 (3 bars)
        }
        else if (param[kValue] < 2 * inc)
        {
            stepsize = 0.0625f; //0.5 (2 whole notes)
        }
        else if (param[kValue] < 3 * inc)
        {
            stepsize = 0.0833333333f; //0.75 (dotted whole)
        }
        else if (param[kValue] < 4 * inc)
        {
            stepsize = 0.125f; //1 (whole note)
        }
        else if (param[kValue] < 5 * inc)
        {
            stepsize = 0.166666667f; //1.5 dotted half
        }
        else if (param[kValue] < 6 * inc)
        {
            stepsize = 0.25f; //2 (half note)
        }
        else if (param[kValue] < 7 * inc)
        {
            stepsize = 0.333333333f; //4.
        }
        else if (param[kValue] < 8 * inc)
        {
            stepsize = 0.375f; //2T
        }
        else if (param[kValue] < 9 * inc)
        {
            stepsize = 0.5f; //4
        }
        else if (param[kValue] < 10 * inc)
        {
            stepsize = 0.666666667f; //8.
        }
        else if (param[kValue] < 11 * inc)
        {
            stepsize = 0.75f; //4T
        }
        else if (param[kValue] < 12 * inc)
        {
            stepsize = 1.0f; //8
        }
        else if (param[kValue] < 13 * inc)
        {
            stepsize = 1.333333333f; //16.
        }
        else if (param[kValue] < 14 * inc)
        {
            stepsize = 1.5f; //8T
        }
        else if (param[kValue] < 15 * inc)
        {
            stepsize = 2.0f; //16
        }
        else if (param[kValue] < 16 * inc)
        {
            stepsize = 2.666666667f; //32.
        }
        else if (param[kValue] < 17 * inc)
        {
            stepsize = 3.0f; //16T
        }
        else if (param[kValue] < 18 * inc)
        {
            stepsize = 4.0f; //32
        }
        else if (param[kValue] < 19 * inc)
        {
            stepsize = 5.333333333f; //64.
        }
        else if (param[kValue] < 20 * inc)
        {
            stepsize = 6.0f; //32T
        }
        else
        {
            stepsize = 8.0f; //64
        }
    }
    else if (index == kProb)
    {
        //force top & bottom values to be what they say they are
        if (roundToInt(param[kProb] * 100.0f) == 0)
        {
            param[kProb] = 0.0f;
        }
        else if (roundToInt(param[kProb] * 100.0f) == 100)
        {
            param[kProb] = 1.0f;
        }
    }
    else if (index == kOffset)
    {
        if (abs(2.0f * param[index] - 1.0f) < 0.01f)
        {
            param[index] = 0.5f;
        }
    }
    else if (index == kMaxAmount)
    {
        updateDisplay();
    }
}

//-----------------------------------------------------------------------------------------
float MidiSwing::getParameter(VstInt32 index)
{
    return param[index];
}

//-----------------------------------------------------------------------------------------
void MidiSwing::getParameterName(VstInt32 index, char* label)
{
    switch (index)
    {
        case kMode:
            strcpy(label, "Mode");
            break;
        case kValue:
            strcpy(label, "NoteValue");
            break;
        case kAmount:
            strcpy(label, "Swing");
            break;
        case kRadius:
            strcpy(label, "TimeFocus");
            break;
        case kShape:
            strcpy(label, "TimeShape");
            break;
        case kVelocity:
            strcpy(label, "Velocity");
            break;
        case kProb:
            strcpy(label, "Probability");
            break;
        case kRandom:
            strcpy(label, "Randomness");
            break;
        case kMaxAmount:
            strcpy(label, "MaxSwing");
            break;
        case kChannel:
            strcpy(label, "Channel");
            break;
        case kOffset:
            strcpy(label, "TimeOffset");
            break;
        case kOverlap:
            strcpy(label, "NoteOff");
            break;
        default:
            strcpy(label, " ");
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiSwing::getParameterDisplay(VstInt32 index, char* text)
{
    const float inc = 1.f / 21.f;
    switch (index)
    {
        case kMode:
            if (param[index] < 0.33333f)
            {
                strcpy(text, "Off");
            }
            else if (param[index] < 0.66667)
            {
                strcpy(text, "Notes Only");
            }
            else
            {
                strcpy(text, "Notes & CCs");
            }
            break;
        case kValue:
            if (param[index] < 1 * inc)
            {
                strcpy(text, "3bars");
            }
            else if (param[index] < 2 * inc)
            {
                strcpy(text, "2bars");
            }
            else if (param[index] < 3 * inc)
            {
                strcpy(text, "dotted1");
            }
            else if (param[index] < 4 * inc)
            {
                strcpy(text, "1");
            }
            else if (param[index] < 5 * inc)
            {
                strcpy(text, "dotted2");
            }
            else if (param[index] < 6 * inc)
            {
                strcpy(text, "2");
            }
            else if (param[index] < 7 * inc)
            {
                strcpy(text, "dotted4");
            }
            else if (param[index] < 8 * inc)
            {
                strcpy(text, "2T");
            }
            else if (param[index] < 9 * inc)
            {
                strcpy(text, "4");
            }
            else if (param[index] < 10 * inc)
            {
                strcpy(text, "dotted8");
            }
            else if (param[index] < 11 * inc)
            {
                strcpy(text, "4T");
            }
            else if (param[index] < 12 * inc)
            {
                strcpy(text, "8");
            }
            else if (param[index] < 13 * inc)
            {
                strcpy(text, "dotted16");
            }
            else if (param[index] < 14 * inc)
            {
                strcpy(text, "8T");
            }
            else if (param[index] < 15 * inc)
            {
                strcpy(text, "16");
            }
            else if (param[index] < 16 * inc)
            {
                strcpy(text, "dotted32");
            }
            else if (param[index] < 17 * inc)
            {
                strcpy(text, "16T");
            }
            else if (param[index] < 18 * inc)
            {
                strcpy(text, "32");
            }
            else if (param[index] < 19 * inc)
            {
                strcpy(text, "dotted64");
            }
            else if (param[index] < 20 * inc)
            {
                strcpy(text, "32T");
            }
            else
            {
                strcpy(text, "64");
            }
            break;
        case kAmount:
            float m;
            if (param[kMaxAmount] < 0.2f)
            {
                m = 1.0f;
            }
            else if (param[kMaxAmount] < 0.4f)
            {
                m = 1.5f;
            }
            else if (param[kMaxAmount] < 0.6f)
            {
                m = 2.0f;
            }
            else if (param[kMaxAmount] < 0.8)
            {
                m = 2.5f;
            }
            else
            {
                m = 3.0f;
            }
            sprintf(text, "%d", roundToInt(param[index] * 100.0f * m));
            break;
        case kRadius:
            sprintf(text, "%d", roundToInt(param[index] * 100.0f));
            break;
        case kShape:
            if (param[index] < 0.25f)
            {
                strcpy(text, "Parabolic");
            }
            else if (param[index] < 0.5f)
            {
                strcpy(text, "Flat");
            }
            else if (param[index] < 0.75f)
            {
                strcpy(text, "Linear Up");
            }
            else
            {
                strcpy(text, "Linear Down");
            }
            break;
        case kVelocity:
            signed char v;
            v = roundToInt(127.0f * (2.0f * param[index] - 1.0f));
            if (v == 0)
            {
                strcpy(text, "No Change");
            }
            else if (v > 0)
            {
                sprintf(text, "+%d", roundToInt(127.0f * (2.0f * param[index] - 1.0f)));
            }
            else
            {
                sprintf(text, "%d", roundToInt(127.0f * (2.0f * param[index] - 1.0f)));
            }
            break;
        case kProb:
            sprintf(text, "%d", roundToInt(param[index] * 100.0f));
            break;
        case kRandom:
            sprintf(text, "%d", roundToInt(param[index] * 100.0f));
            break;
        case kChannel:
            if (FLOAT_TO_CHANNEL(param[index]) == -1)
            {
                strcpy(text, "All");
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_CHANNEL(param[index]) + 1);
            }
            break;
        case kMaxAmount:
            if (param[index] < 0.2f)
            {
                strcpy(text, "100"); //2:1
            }
            else if (param[index] < 0.4f)
            {
                strcpy(text, "150"); //3:1
            }
            else if (param[index] < 0.6f)
            {
                strcpy(text, "200"); //5:1
            }
            else if (param[index] < 0.8f)
            {
                strcpy(text, "250"); //11:1
            }
            else
            {
                strcpy(text, "300"); //oo:1
            }
            break;
        case kOffset:
            sprintf(text, "%.2f", 2.0f * param[index] - 1.0f);
            break;
        case kOverlap:
            if (param[index] < 0.33333f)
            {
                strcpy(text, "Equal Delay");
            }
            else if (param[index] < 0.66667f)
            {
                strcpy(text, "Stingy Delay");
            }
            else
            {
                strcpy(text, "Legato");
            }
            break;
        default:
            sprintf(text, "%f", param[index]);
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiSwing::getParameterLabel(VstInt32 index, char* label)
{
    switch (index)
    {
        case kAmount:
            strcpy(label, "%");
            break;
        case kRadius:
            strcpy(label, "%");
            break;
        case kProb:
            strcpy(label, "%");
            break;
        case kRandom:
            strcpy(label, "%");
            break;
        case kMaxAmount:
            strcpy(label, "%");
            break;
        default:
            strcpy(label, " ");
            break;
    }
}

//-----------------------------------------------------------------------------------------
bool MidiSwing::init(void)
{
    srand((unsigned int) time(NULL));

    _ppq           = 0.0;
    _bpm           = 120.0f;
    _beatpos       = 0.0;
    samplesPerBeat = 0;

    for (int ch = 0; ch < 16; ch++)
    {
        for (int n = 0; n < 128; n++)
        {
            for (int v = 0; v < MAX_VOICES_PER_NOTE; v++)
            {
                noteOnCounter[n][ch][v]  = 0;
                noteOffCounter[n][ch][v] = 0;
                noteOrigVel[n][ch][v]    = 0;
                noteOrigPos[n][ch][v]    = 0;
                noteDelay[n][ch][v]      = 0;
                noteOffDelay[n][ch][v]   = 0;
                notePlaying[n][ch][v]    = false;
            }
            noteKilled[n][ch]  = false;
            voices[n][ch]      = 0;
            CCCounter[n][ch]   = 0;
            CCDelay[n][ch]     = 0;
            CCOrigValue[n][ch] = 0;
            CCRecieved[n][ch]  = false;
        }
        expectingDelayedEvents[ch] = 0;
        expectingDelayedCC[ch]     = 0;
    }

    wasplaying = false;
    isplaying  = false;

    return PizMidi::init();
}

void MidiSwing::preProcess()
{
    // preparing Process
    VstTimeInfo* timeInfo = NULL;
    timeInfo              = getTimeInfo(0xffff); //ALL

    if (timeInfo)
    {
        if (kVstTempoValid & timeInfo->flags)
        {
            _bpm = (float) timeInfo->tempo;
        }
        if (kVstPpqPosValid & timeInfo->flags)
        {
            _ppq = timeInfo->ppqPos;
        }
        if (kVstTransportPlaying & timeInfo->flags)
        {
            isplaying = true;
        }
        else
        {
            isplaying = false;
        }
        dbg("tempo=" << _bpm);

        _ppq += (1.0f - 2.0f * param[kOffset]) / (stepsize * 2.0f);
        _beatpos = stepsize * fmod(_ppq, 1.0 / stepsize);
    }

    samplesPerBeat = roundToInt(60.0f * sampleRate / _bpm);
    samplesPerTick = roundToInt((float) samplesPerBeat / 960.0f);
    totalSamples   = roundToInt(_ppq * (double) samplesPerBeat);

    _cleanMidiOutBuffers();
}

void MidiSwing::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 samples)
{
    char listenchannel = FLOAT_TO_CHANNEL(param[kChannel]);

    bool on = param[kMode] >= 0.33333f;

    float amount = param[kAmount];
    if (! on)
    {
        amount = 0;
    }

    float delaymult = 4.0f; //6=100%, 4=150%, 3=200%
    if (param[kMaxAmount] < 0.2f)
    {
        delaymult = 6.0f;
    }
    else if (param[kMaxAmount] < 0.4f)
    {
        delaymult = 4.0f;
    }
    else if (param[kMaxAmount] < 0.6f)
    {
        delaymult = 3.0f;
    }
    else if (param[kMaxAmount] < 0.8)
    {
        delaymult = 2.4f;
    }
    else
    {
        delaymult = 2.0f;
    }

    bool overlap   = param[kOverlap] < 0.33333f;
    bool swingoffs = param[kOverlap] >= 0.66667f;

    //special case for applying velocity offset only
    bool velonly = (param[kAmount] == 0 && on);

    //find delayed events that are due in this block--------------------------------
    for (int ch = 0; ch < 16; ch++)
    {
        if (expectingDelayedCC[ch] > 0 || expectingDelayedEvents[ch] > 0)
        { //to decrease CPU usage
            for (int n = 0; n < 128; n++)
            {
                if (expectingDelayedCC[ch] > 0)
                {
                    //Check for CCs-----------------------------------------------------------------
                    if (CCCounter[n][ch] > 0 && CCDelay[n][ch] > 0)
                    {
                        if (CCCounter[n][ch] + samples > CCDelay[n][ch])
                        {
                            dbg("-------- ");
                            VstInt32 delta = CCDelay[n][ch] - CCCounter[n][ch];
                            if (delta < 0)
                            {
                                delta = 0;
                            }
                            else if (delta > samples)
                            {
                                delta = samples;
                            }
                            VstMidiEvent cc;
                            cc.midiData[0] = MIDI_CONTROLCHANGE + ch;
                            cc.midiData[1] = n;
                            cc.midiData[2] = CCOrigValue[n][ch];
                            cc.deltaFrames = delta;
                            cc.detune      = 0;
                            outputs[0].push_back(cc);
                            dbg("sending delayed CC=" << n << " total_delay=" << CCCounter[n][ch] + cc.deltaFrames << " delta=" << cc.deltaFrames);
                            resetCC(n, ch);
                            expectingDelayedCC[ch]--;
                        }
                        else
                        {
                            // not due yet, keep counting
                            CCCounter[n][ch] += samples;
                            CCRecieved[n][ch] = false;
                        }
                    }
                }
                if (expectingDelayedEvents[ch] > 0)
                {
                    for (int v = 0; v < MAX_VOICES_PER_NOTE; v++)
                    {
                        //Check for NoteOffs------------------------------------------------------------
                        if (noteOffCounter[n][ch][v] > 0 && noteOffDelay[n][ch][v] > 0)
                        {
                            if (noteOffCounter[n][ch][v] + samples > noteOffDelay[n][ch][v])
                            {
                                dbg("-------- "
                                    << " v=" << v);
                                VstInt32 delta = noteOffDelay[n][ch][v] - noteOffCounter[n][ch][v];
                                if (delta < 0)
                                {
                                    delta = 0;
                                }
                                else if (delta > samples)
                                {
                                    delta = samples;
                                }
                                if (! noteKilled[n][ch])
                                {
                                    VstMidiEvent noteoff;
                                    noteoff.midiData[0] = MIDI_NOTEOFF + ch;
                                    noteoff.midiData[1] = n;
                                    noteoff.midiData[2] = 0;
                                    noteoff.deltaFrames = delta;
                                    noteoff.detune      = 0;
                                    outputs[0].push_back(noteoff);
                                    dbg("sending delayed noteoff=" << n << " v=" << v << " total_delay=" << noteOffCounter[n][ch][v] + noteoff.deltaFrames << " delta=" << noteoff.deltaFrames);
                                    resetNote(n, ch, v, true);
                                    expectingDelayedEvents[ch]--;
                                }
                                else
                                {
                                    // if the note has been killed, that probably means a delayed
                                    // note overlapped a following note, and this belongs the delayed
                                    // one which should be ignored.
                                    dbg("ignored previously killed delayed noteoff=" << n << " v=" << v);
                                    //reset properties for voice but keep note "playing"
                                    //resetNote(n,ch,v-1,true);
                                    noteKilled[n][ch]     = false;
                                    notePlaying[n][ch][v] = true;
                                    expectingDelayedEvents[ch]--;
                                }
                            }
                            else
                            {
                                // not due yet, keep counting
                                noteOffCounter[n][ch][v] += samples;
                            }
                        }
                        //check for NoteOns-------------------------------------------------------------
                        if (noteOnCounter[n][ch][v] > 0 && noteDelay[n][ch][v] > 0)
                        {
                            if (noteOnCounter[n][ch][v] + samples > noteDelay[n][ch][v])
                            {
                                dbg("-------- "
                                    << " v=" << v);
                                VstInt32 delta = noteDelay[n][ch][v] - noteOnCounter[n][ch][v];
                                if (delta < 0)
                                {
                                    delta = 0;
                                }
                                else if (delta > samples)
                                {
                                    delta = samples;
                                }
                                // if note is already playing, send a noteoff first
                                if (notePlaying[n][ch][v - 1])
                                {
                                    VstMidiEvent kill;
                                    kill.midiData[0] = MIDI_NOTEOFF + ch;
                                    kill.midiData[1] = n;
                                    kill.midiData[2] = 0;
                                    kill.deltaFrames = delta;
                                    kill.detune      = 0;
                                    outputs[0].push_back(kill);
                                    dbg("killed note " << n);
                                    resetNote(n, ch, v - 1, true);
                                    noteKilled[n][ch] = true;
                                }

                                VstMidiEvent note;
                                note.midiData[0] = MIDI_NOTEON + ch;
                                note.midiData[1] = n;
                                note.midiData[2] = noteOrigVel[n][ch][v];
                                note.deltaFrames = delta;
                                note.detune      = 0;
                                outputs[0].push_back(note);
                                dbg("sending delayed noteon " << n << " v=" << v << " total_delay=" << noteOnCounter[n][ch][v] + note.deltaFrames << " delta=" << note.deltaFrames);

                                //after sending, reset note properties
                                resetNote(n, ch, v);
                                expectingDelayedEvents[ch]--;
                            }
                            else
                            {
                                // not due yet, keep counting
                                noteOnCounter[n][ch][v] += samples;
                                //dbg("delay note more, counter=" << noteOnCounter[n][ch]);
                            }
                        }
                    }
                }
            }
        }
    }

    //process incoming events-------------------------------------------------------
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        unsigned char status = tomod.midiData[0] & 0xf0; // scraping  channel
        char channel         = tomod.midiData[0] & 0x0f; // isolating channel (0-15)
        char data1           = tomod.midiData[1] & 0x7f;
        char data2           = tomod.midiData[2] & 0x7f;

        dbg("incoming midi event, i=" << i << " status=" << status << " ch=" << (int) channel);

        //make 0-velocity notes into "real" noteoffs for simplicity
        if (status == MIDI_NOTEON && data2 == 0)
        {
            status            = MIDI_NOTEOFF;
            tomod.midiData[0] = MIDI_NOTEOFF | channel;
        }

        bool discard = false;

        //only look at the selected channel
        if (channel == listenchannel || listenchannel == -1)
        {
            //incoming NoteOff--------------------------------------------------------------
            if (status == MIDI_NOTEOFF)
            {
                char v = voices[data1][channel];
                dbg("NoteOff " << data1 << " delta=" << tomod.deltaFrames << " playing=" << notePlaying[data1][channel][v] << " v=" << voices[data1][channel]);
                //we assume every noteoff had a matching noteon as the previous event
                //for each channel/note.
                //therefore this noteoff belongs to the same voice as the last noteon.

                if (! overlap)
                {
                    VstInt32 delay = 0;
                    if (swingoffs)
                    {
                        VstInt32 beatpos_samples = roundToInt(_beatpos * samplesPerBeat) + tomod.deltaFrames;
                        if (abs(beatpos_samples - samplesPerBeat / 2) <= samplesPerBeat / (param[kRadius] * 13 + 3))
                        {
                            float random = 1.0f - param[kRandom] * float(rand() % 1000) * 0.001f;
                            float c      = closeness(beatpos_samples);

                            //determine delay amount, in samples:
                            delay = roundToInt((float) samplesPerBeat * (c * amount * random / (delaymult * stepsize)))
                                //+ roundToInt(250.0f*amount*c)
                                ;
                        }
                    }
                    if ((noteDelay[data1][channel][v] + noteOrigPos[data1][channel][v])
                        >= (totalSamples + tomod.deltaFrames + delay))
                    {
                        //make noteoff 1 tick after noteon
                        dbg("shortnote olddelay=" << delay << " on=" << noteOrigPos[data1][channel][v] + noteDelay[data1][channel][v] << " off=" << totalSamples + tomod.deltaFrames + delay);
                        dbg("samplesPerTick=" << samplesPerTick);
                        //delay=noteOrigPos[data1][channel][v]+noteDelay[data1][channel][v] - (totalSamples+tomod.deltaFrames+delay) + 10*samplesPerTick;
                        delay = noteDelay[data1][channel][v];
                    }

                    if (delay > 0)
                    {
                        dbg("NoteOff needs delay=" << delay);
                        noteOffCounter[data1][channel][v] = 0;
                        if (samples - tomod.deltaFrames - delay > 0)
                        {
                            // delayed event is within the current block
                            if (noteKilled[data1][channel])
                            {
                                // if the note has been killed, that probably means a delayed
                                // note overlapped a following note, and this belongs the
                                // one which should be ignored.
                                dbg("ignored previously killed noteoff=" << data1);
                                //discard=true;
                            }
                            else
                            {
                                tomod.deltaFrames += delay;
                                notePlaying[data1][channel][v] = false;
                            }
                            resetNote(data1, channel, v, true);
                        }
                        else
                        {
                            expectingDelayedEvents[channel]++;
                            // delayed event is in a future block, so start counter
                            noteOffCounter[data1][channel][v] += samples - tomod.deltaFrames;
                            discard                         = true;
                            noteOffDelay[data1][channel][v] = delay;
                        }
                    }
                    else
                    {
                        dbg("no delay");
                        resetNote(data1, channel, v, true);
                    }
                    //                    if (shortnote)
                    //                    {
                    //                        //a delayed note is playing, so we'll delay the note-off
                    //                        VstInt32 delay = noteDelay[data1][channel][v];
                    //                        dbg("NoteOff needs delay=" << delay);
                    //                        noteOffCounter[data1][channel][v] = 0;
                    //                        if (samples - tomod.deltaFrames - delay > 0) {
                    //    						// delayed event is within the current block
                    //                            if (noteKilled[data1][channel]) {
                    //                                // if the note has been killed, that probably means a delayed
                    //                                // note overlapped a following note, and this belongs the
                    //                                // one which should be ignored.
                    //                                 dbg("ignored previously killed noteoff=" << data1);
                    //                                 //discard=true;
                    //                            }
                    //            				else {
                    //                                tomod.deltaFrames += delay;
                    //                                notePlaying[data1][channel][v] = false;
                    //                            }
                    //                            resetNote(data1,channel,v,true);
                    //                        }
                    //                        else {
                    //    						// delayed event is in a future block, so start counter
                    //                            noteOffCounter[data1][channel][v] += samples - tomod.deltaFrames;
                    //    						discard=true;
                    //        					noteOffDelay[data1][channel][v] = delay;
                    //                            expectingDelayedEvents[channel]++;
                    //                        }
                    //                    }
                    //                    else //no delay
                    //                    {
                    //                         dbg("no delay");
                    //                        resetNote(data1,channel,v,true);
                    //                    }
                }
                else
                {
                    //see if this belongs to a delayed note, and delay it if needed
                    if (noteDelay[data1][channel][v] > 0)
                    {
                        //a delayed note is playing, so we'll delay the note-off
                        VstInt32 delay = noteDelay[data1][channel][v];
                        dbg("NoteOff needs delay=" << delay);
                        noteOffCounter[data1][channel][v] = 0;
                        if (samples - tomod.deltaFrames - delay > 0)
                        {
                            // delayed event is within the current block
                            if (noteKilled[data1][channel])
                            {
                                // if the note has been killed, that probably means a delayed
                                // note overlapped a following note, and this belongs the
                                // one which should be ignored.
                                dbg("ignored previously killed noteoff=" << data1);
                                //discard=true;
                            }
                            else
                            {
                                tomod.deltaFrames += delay;
                                notePlaying[data1][channel][v] = false;
                            }
                            resetNote(data1, channel, v, true);
                        }
                        else
                        {
                            expectingDelayedEvents[channel]++;
                            // delayed event is in a future block, so start counter
                            noteOffCounter[data1][channel][v] += samples - tomod.deltaFrames;
                            discard                         = true;
                            noteOffDelay[data1][channel][v] = delay;
                        }
                    }
                    else //no delay
                    {
                        dbg("no delay");
                        resetNote(data1, channel, v, true);
                    }
                }
            }
            //Incoming NoteOn---------------------------------------------------------------
            else if (status == MIDI_NOTEON)
            {
                voices[data1][channel]++;
                if (voices[data1][channel] >= MAX_VOICES_PER_NOTE)
                {
                    voices[data1][channel] = 1;
                }
                char v = voices[data1][channel];
                // find the location of the note within the beat, in samples:
                VstInt32 beatpos_samples = roundToInt(_beatpos * samplesPerBeat) + tomod.deltaFrames;
                dbg("Incoming NoteOn=" << data1 << " v" << v << " delta=" << tomod.deltaFrames << " beatpos=" << beatpos_samples << " samplesPerBeat=" << samplesPerBeat);

                VstInt32 delay = 0;
                int velocity   = data2;
                // if "close" to the beat division, delay it:
                if (abs(beatpos_samples - samplesPerBeat / 2) <= samplesPerBeat / (param[kRadius] * 13 + 3))
                {
                    float random = 1.0f - param[kRandom] * float(rand() % 1000) * 0.001f;
                    float c      = closeness(beatpos_samples);

                    //determine delay amount, in samples:
                    delay = roundToInt((float) samplesPerBeat * (c * amount * random / (delaymult * stepsize)))
                        //+ roundToInt(250.0f*amount*c)
                        ;
                    dbg("delay=" << delay << " closeness=" << c << " amount=" << amount << " stepsize=" << stepsize);

                    //determine velocity
                    float v = 127.0f * (2.0f * param[kVelocity] - 1.0f); //max velocity offset
                    velocity += roundToInt(c * v * random);              //new (offset) velocity
                    if (velocity < 1)
                    {
                        velocity = 1;
                    }
                    else if (velocity > 127)
                    {
                        velocity = 127;
                    }

                    //probability
                    //don't do anything unless prob < 100%, and mode is not "off"
                    if (param[kProb] < 1.0f && on)
                    {
                        if ((float) (rand() % 99) >= c * 100.0f * param[kProb])
                        {
                            discard                    = true;
                            velocity                   = 0;
                            delay                      = 0;
                            noteKilled[data1][channel] = true; //so the noteoff can be dealt with
                        }
                    }
                }
                if (delay > 0)
                {
                    if (samples - tomod.deltaFrames - delay > 0)
                    {
                        // delayed note is within the current block, don't need counter
                        tomod.deltaFrames += delay;
                        dbg("delay note within block, deltaFrames=" << tomod.deltaFrames);

                        //if the note is already playing, first send a noteoff to avoid overlap
                        if (notePlaying[data1][channel][v - 1])
                        {
                            VstMidiEvent kill;
                            kill.midiData[0] = MIDI_NOTEOFF | channel;
                            kill.midiData[1] = data1;
                            kill.midiData[2] = 0;
                            kill.deltaFrames = tomod.deltaFrames;
                            kill.detune      = 0;
                            outputs[0].push_back(kill);
                            dbg("killed note " << data1 << " v=" << v - 1);
                            resetNote(data1, channel, v - 1, true);
                            noteKilled[data1][channel] = true;
                        }
                        notePlaying[data1][channel][v] = true;
                        tomod.midiData[2]              = velocity;
                    }
                    else
                    {
                        expectingDelayedEvents[channel]++;
                        // delayed note is in a future block, so start counter:
                        noteOnCounter[data1][channel][v] += samples - tomod.deltaFrames;
                        discard = true; //takes the event out of the current block
                        dbg("delay note in later block, counter=" << noteOnCounter[data1][channel][v]);
                    }
                    noteDelay[data1][channel][v] = delay;
                    //remember the velocity to use for the delayed note
                    noteOrigVel[data1][channel][v] = velocity;
                    noteOrigPos[data1][channel][v] = totalSamples + tomod.deltaFrames;
                }
                else if (velocity > 0)
                {
                    dbg("no delay");
                    noteDelay[data1][channel][v] = 0;
                    if (notePlaying[data1][channel][v - 1])
                    {
                        VstMidiEvent kill;
                        kill.midiData[0] = MIDI_NOTEOFF + channel;
                        kill.midiData[1] = data1;
                        kill.midiData[2] = 0;
                        kill.deltaFrames = tomod.deltaFrames - 1;
                        kill.detune      = 0;
                        outputs[0].push_back(kill);
                        dbg("killed note " << data1);
                        resetNote(data1, channel, v - 1, true);
                        noteKilled[data1][channel] = true;
                    }
                    notePlaying[data1][channel][v] = true;

                    if (velonly)
                    {
                        //when "amount" is at 0 but mode is not "off", apply velocity without delay
                        tomod.midiData[2] = velocity;
                    }
                }
            }
            //Incoming CC-------------------------------------------------------------
            else if (status == MIDI_CONTROLCHANGE)
            {
                if (param[kMode] >= 0.66667f)
                {
                    VstInt32 beatpos_samples = roundToInt(_beatpos * samplesPerBeat) + tomod.deltaFrames;
                    dbg("Incoming CC=" << data1 << " delta=" << tomod.deltaFrames << " beatpos=" << beatpos_samples << " samplesPerBeat=" << samplesPerBeat);

                    VstInt32 delay = 0;
                    // if "close" to the beat division, delay it:
                    if (abs(beatpos_samples - samplesPerBeat / 2) <= samplesPerBeat / (param[kRadius] * 13 + 3))
                    {
                        float random = 1.0f - param[kRandom] * float(rand() % 1000) * 0.001f;
                        float c      = closeness(beatpos_samples);

                        //determine delay amount, in samples:
                        float delaymult = 4.0f; //6=100%, 4=150%, 3=200%
                        if (param[kMaxAmount] < 0.25f)
                        {
                            delaymult = 6.0f;
                        }
                        else if (param[kMaxAmount] < 0.5f)
                        {
                            delaymult = 4.0f;
                        }
                        else if (param[kMaxAmount] < 0.75f)
                        {
                            delaymult = 3.0f;
                        }
                        else if (param[kMaxAmount] < 1.0)
                        {
                            delaymult = 2.4f;
                        }
                        else
                        {
                            delaymult = 2.0f;
                        }
                        delay = roundToInt((float) samplesPerBeat * (c * amount * random / (delaymult * stepsize)))
                            //+ roundToInt(250.0f*amount*c)
                            ;
                        dbg("delay=" << delay << " closeness=" << c << " amount=" << amount << " stepsize=" << stepsize);

                        //                        //probability
                        //                        //don't do anything unless prob < 100%
                        //                        if (roundToInt(100.0f*param[kProb])<100) {
                        //                            if ( (float)(rand()%99) > c*100.0f*param[kProb] ) {
                        //                                discard=true;
                        //                                delay=0;
                        //                            }
                        //                        }
                    }
                    if (delay > 0)
                    {
                        if (samples - tomod.deltaFrames - delay > 0)
                        {
                            // delayed CC is within the current block, don't need counter
                            tomod.deltaFrames += delay;
                            dbg("delay CC within block, deltaFrames=" << tomod.deltaFrames);
                        }
                        else
                        {
                            // delayed CC is in a future block, so start counter:
                            CCCounter[data1][channel] += samples - tomod.deltaFrames;
                            if (! CCRecieved[data1][channel])
                            {
                                //only count one event per block
                                expectingDelayedCC[channel]++;
                            }
                            CCRecieved[data1][channel] = true;
                            discard                    = true; //takes the event out of the current block
                            dbg("delay CC in later block, counter=" << CCCounter[data1][channel]);
                        }
                        CCDelay[data1][channel] = delay;
                        //remember the value to use for the delayed CC
                        CCOrigValue[data1][channel] = data2;
                    }
                    else
                    {
                        dbg("no delay");
                        CCDelay[data1][channel] = 0;
                    }
                }
            }
        } // if listenchannel==channel
        if (! discard)
        {
            outputs[0].push_back(tomod);
        }
    } //for() inputs loop

    sortMidiEvents(outputs[0]);

    if (wasplaying && ! isplaying)
    { //just stopped
        dbg("stopped");
        for (int ch = 0; ch < 16; ch++)
        {
            expectingDelayedEvents[ch] = 0;
            for (int n = 0; n < 128; n++)
            {
                for (int v = 0; v < MAX_VOICES_PER_NOTE; v++)
                {
                    if (notePlaying[n][ch][v])
                    {
                        VstMidiEvent kill;
                        kill.midiData[0] = MIDI_NOTEOFF + ch;
                        kill.midiData[1] = n;
                        kill.midiData[2] = 0;
                        kill.deltaFrames = samples - 1;
                        kill.detune      = 0;
                        outputs[0].push_back(kill);
                        dbg("stopped, killed note " << n);
                        resetNote(n, ch, v, true);
                    }
                }
            }
        }
    }
    wasplaying = isplaying;
}

inline float MidiSwing::closeness(long int samplepos)
{
    float x = (float) samplepos / (float) samplesPerBeat;
    float r = 1.5f + 14.5f * getParameter(kRadius);
    float result;
    if (param[kShape] < 0.25f)
    {
        // parabola
        result = 1 - (2 * r * x - r) * (2 * r * x - r);
    }
    else if (param[kShape] < 0.5f)
    {
        // square
        result = 1.0f;
    }
    else if (param[kShape] < 0.75f)
    {
        // linear up
        result = (r + 2.0f) * x - (r + 1.0f) * 0.5f;
    }
    else
    {
        // linear down
        result = r * 0.5f + 1.5f - (r + 2.0f) * x;
    }
    if (result < 0.0001f)
    {
        result = 0.f;
    }
    else if (result > 0.95f)
    {
        result = 1.f;
    }
    return result;
}

inline void MidiSwing::resetNote(char n, char ch, char v, bool off)
{
    if (off)
    {
        dbg("Resetting NoteOff v" << v);
        noteOffCounter[n][ch][v] = 0;
        noteOffDelay[n][ch][v]   = 0;
        noteDelay[n][ch][v]      = 0;
        notePlaying[n][ch][v]    = false;
        noteOrigPos[n][ch][v]    = 0;
        noteKilled[n][ch]        = false;
    }
    else
    {
        dbg("Resetting NoteOn v" << v);
        noteOnCounter[n][ch][v] = 0;
        noteOrigVel[n][ch][v]   = 0;
        notePlaying[n][ch][v]   = true;
    }
}

inline void MidiSwing::resetCC(char n, char ch)
{
    CCCounter[n][ch]  = 0;
    CCDelay[n][ch]    = 0;
    CCRecieved[n][ch] = false;
}
