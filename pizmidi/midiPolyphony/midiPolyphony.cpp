#include <string>
//for sort()
#include <algorithm>
//for seeding rand()
#include "midiPolyphony.hpp"

#include <time.h>

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    dbg("createEffectInstance");
    return new MidiPolyphony(audioMaster);
}

MidiPolyphonyProgram::MidiPolyphonyProgram()
{
    //dbg("make program");
    // default Program Values
    memset(param, 0, sizeof(param));
    for (int i = 0; i < kNumParams; i++)
    {
        switch (i)
        {
            case kPower:
                param[i] = 1.0f;
                break;
            case kHighCh:
                param[i] = 1.0f;
                break;
            default:
                break;
        }
    }
    // default program name
    strcpy(name, "Default");
}

bool MidiPolyphony::midiSort::operator()(const VstMidiEvent& first, const VstMidiEvent& second)
{
    unsigned int r = rand();
    if (this->mode == random)
    {
        return (r < (int) (RAND_MAX / 2)) ? true : false;
    }
    bool result = false;
    if (first.deltaFrames < second.deltaFrames)
    {
        result = true;
    }
    else if (first.deltaFrames == second.deltaFrames
             && isNoteOnOrOff(first) && isNoteOnOrOff(second))
    {
        bool bothOff = isNoteOff(first) && isNoteOff(second);
        //if (first.midiData[1]==priorityNote)
        // return isNoteOn(first);
        //if (second.midiData[1]==priorityNote)
        // return isNoteOff(second);
        switch (mode)
        {
            case quietest:
                if (bothOff)
                {
                    result = onvelocity[first.midiData[0] & 0x0f][first.midiData[1]]
                           < onvelocity[second.midiData[0] & 0x0f][second.midiData[1]];
                }
                else
                {
                    result = first.midiData[2] > second.midiData[2];
                }
                break;
            case lowest:
                if (bothOff)
                {
                    result = first.midiData[1] < second.midiData[1];
                }
                else
                {
                    result = first.midiData[1] > second.midiData[1];
                }
                break;
            case highest:
                if (bothOff)
                {
                    result = first.midiData[1] > second.midiData[1];
                }
                else
                {
                    result = first.midiData[1] < second.midiData[1];
                }
                break;
            case centered:
                if (abs(first.midiData[1] - centerNote) == abs(second.midiData[1] - centerNote))
                {
                    if (bothOff)
                    {
                        result = first.midiData[1] > second.midiData[1];
                    }
                    else
                    {
                        result = first.midiData[1] < second.midiData[1];
                    }
                }
                else
                {
                    if (bothOff)
                    {
                        result = (abs(first.midiData[1] - centerNote) > abs(second.midiData[1] - centerNote));
                    }
                    else
                    {
                        result = (abs(first.midiData[1] - centerNote) < abs(second.midiData[1] - centerNote));
                    }
                }
                break;
            case random:
                result = (r < (int) (RAND_MAX / 2)) ? true : false;
                break;
            default:
                break;
        } // /switch
    }
    if (isNoteOn(first))
    {
        priorityNote = first.midiData[1];
    }
    return result;
}

//-----------------------------------------------------------------------------
MidiPolyphony::MidiPolyphony(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams), programs(0)
{
    dbg("constructor");
    programs = new MidiPolyphonyProgram[numPrograms];
    memset(param, 0, sizeof(param));
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
            dbg("bank file loaded");
        }
        else
        {
            // built-in programs
            for (int i = 0; i < kNumPrograms; i++)
            {
                sprintf(programs[i].name, "Program %d", i + 1);
            }
            dbg("default programs loaded");
        }
        setProgram(0);
    }

    init();
}

//-----------------------------------------------------------------------------------------
MidiPolyphony::~MidiPolyphony()
{
    if (programs)
    {
        delete[] programs;
    }
}

//------------------------------------------------------------------------
void MidiPolyphony::setProgram(VstInt32 program)
{
    //dbg("setProgram("<<program<<")");
    MidiPolyphonyProgram* ap = &programs[program];

    curProgram = program;
    for (int i = 0; i < kNumParams; i++)
    {
        setParameter(i, ap->param[i]);
    }
}

//------------------------------------------------------------------------
void MidiPolyphony::setProgramName(char* name)
{
    //dbg("setProgramName("<<name<<")");
    vst_strncpy(programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiPolyphony::getProgramName(char* name)
{
    strcpy(name, programs[curProgram].name);
    //dbg("getProgramName("<<name<<")");
}

//-----------------------------------------------------------------------------------------
bool MidiPolyphony::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    //dbg("getProgramNameIndexed("<<index<<")");
    if (index < kNumPrograms)
    {
        strcpy(text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiPolyphony::setParameter(VstInt32 index, float value)
{
    dbg("setParameter(" << index << "," << value << ")");
    MidiPolyphonyProgram* ap = &programs[curProgram];
    param[index] = ap->param[index] = value;
    switch (index)
    {
        case kSteal:
            if (param[index] < 1.f / (float) (numStealModes))
            {
                stealmode = oldest;
            }
            else if (param[index] < 2.f / (float) (numStealModes))
            {
                stealmode = newest;
            }
            else if (param[index] < 3.f / (float) (numStealModes))
            {
                stealmode = quietest;
            }
            else if (param[index] < 4.f / (float) (numStealModes))
            {
                stealmode = lowest;
            }
            else if (param[index] < 5.f / (float) (numStealModes))
            {
                stealmode = highest;
            }
            else if (param[index] < 6.f / (float) (numStealModes))
            {
                stealmode = centered;
            }
            else
            {
                stealmode = random;
            }

            sorter.mode = stealmode;
            break;
        case kSustain:
            sustain = false;
            break;
        case kLowCh:
            if (value > param[kHighCh])
            {
                setParameterAutomated(kHighCh, value);
            }
            break;
        case kHighCh:
            if (value < param[kLowCh])
            {
                setParameterAutomated(kLowCh, value);
            }
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
float MidiPolyphony::getParameter(VstInt32 index)
{
    //dbg("getParameter("<<index<<")");
    return param[index];
}

//-----------------------------------------------------------------------------------------
void MidiPolyphony::getParameterName(VstInt32 index, char* label)
{
    switch (index)
    {
        case kPower:
            strcpy(label, "Power");
            break;
        case kLowCh:
            strcpy(label, "LowChannel");
            break;
        case kHighCh:
            strcpy(label, "HighChannel");
            break;
        case kMaxPoly:
            strcpy(label, "PolyPerChan");
            break;
        case kSteal:
            strcpy(label, "NotePriority");
            break;
        case kRetrig:
            strcpy(label, "Retrigger");
            break;
        case kInCh:
            strcpy(label, "InChannel");
            break;
        case kSustain:
            strcpy(label, "SustainPedal");
            break;
        case kMode:
            strcpy(label, "ChanMode");
            break;
        default:
            break;
    }
    dbg("getParameterName(" << index << "," << label << ")");
}

//-----------------------------------------------------------------------------------------
void MidiPolyphony::getParameterDisplay(VstInt32 index, char* text)
{
    switch (index)
    {
        case kPower:
            if (param[index] < 0.5)
            {
                strcpy(text, "Off (Bypass)");
            }
            else
            {
                strcpy(text, "On");
            }
            break;
        case kRetrig:
            if (param[index] < 0.5)
            {
                strcpy(text, "Off");
            }
            else
            {
                strcpy(text, "On");
            }
            break;
        case kLowCh:
            sprintf(text, "Ch %d", roundToInt(15.f * param[index]) + 1);
            break;
        case kHighCh:
            sprintf(text, "Ch %d", roundToInt(15.f * param[index]) + 1);
            break;
        case kInCh:
            if (FLOAT_TO_CHANNEL(param[index]) == -1)
            {
                strcpy(text, "Any");
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_CHANNEL(param[index]) + 1);
            }
            break;
        case kSteal:
            if (stealmode == oldest)
            {
                strcpy(text, "Steal Oldest");
            }
            else if (stealmode == newest)
            {
                strcpy(text, "Steal Last");
            }
            else if (stealmode == quietest)
            {
                strcpy(text, "Steal Quietest");
            }
            else if (stealmode == lowest)
            {
                strcpy(text, "High");
            }
            else if (stealmode == highest)
            {
                strcpy(text, "Low");
            }
            else if (stealmode == centered)
            {
                strcpy(text, "Centered");
            }
            else if (stealmode == random)
            {
                strcpy(text, "Random");
            }
            break;
        case kMaxPoly:
            int p;
            p = roundToInt(((float) maxPoly - 1) * param[index]) + 1;
            if (p == 1)
            {
                sprintf(text, "%d Note", p);
            }
            else
            {
                sprintf(text, "%d Notes", p);
            }
            break;
        case kSustain:
            if (param[index] < 0.5)
            {
                strcpy(text, "Pass Thru");
            }
            else
            {
                strcpy(text, "Use");
            }
            break;
        case kMode:
            if (param[index] < 0.5)
            {
                strcpy(text, "Reuse");
            }
            else
            {
                strcpy(text, "Prefer New");
            }
            break;
        default:
            break;
    }
    dbg("getParameterDisplay(" << index << "," << text << ")");
}

bool MidiPolyphony::init()
{
    srand((unsigned int) time(NULL));

    poly      = roundToInt(((float) maxPoly - 1) * param[kMaxPoly]) + 1;
    lastoutch = 0;
    oldness   = 0;
    //queued=0;
    sustain    = false;
    playing    = false;
    wasplaying = false;
    memset(ntime, -1, sizeof(ntime));
    memset(held, 0, sizeof(held));
    memset(sounding, 0, sizeof(sounding));
    centerNote = sorter.centerNote = 60;
    for (int ch = 0; ch < MIDI_MAX_CHANNELS; ch++)
    {
        voices[ch] = 0;
        for (int i = 0; i < maxPoly; i++)
        {
            Voice[ch][i].reset();
        }
    }
    for (int q = 0; q < queueSize; q++)
    {
        Queue[q].reset();
    }

    return PizMidi::init();
}

void MidiPolyphony::preProcess(void)
{
    // preparing Proccess
    VstTimeInfo* timeInfo = NULL;
    timeInfo              = getTimeInfo(0xffff); //ALL

    playing = false;
    if (timeInfo)
    {
        if (kVstTransportPlaying & timeInfo->flags)
        {
            playing = true;
        }
        else
        {
            playing = false;
        }
    }
    else
    {
        playing = true;
    }

    _cleanMidiOutBuffers();
}

VstInt32 MidiPolyphony::processEvents(VstEvents* ev)
{
    VstEvents* evts = (VstEvents*) ev;

    for (int i = 0; i < evts->numEvents; i++)
    {
        if ((evts->events[i])->type != kVstMidiType)
        {
            continue;
        }
        VstMidiEvent* event = (VstMidiEvent*) evts->events[i];
        _midiEventsIn[0].push_back(*event);
    }

    //sort according to note priority
    std::sort(_midiEventsIn[0].begin(), _midiEventsIn[0].end(), sorter);

    return true;
}

void MidiPolyphony::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    const char listenchannel = FLOAT_TO_CHANNEL(param[kInCh]);
    const char lowch         = roundToInt(15.f * param[kLowCh]);
    char highch              = roundToInt(15.f * param[kHighCh]);
    if (lowch > highch)
    {
        highch = lowch;
    }
    int newpoly = roundToInt(((float) maxPoly - 1) * param[kMaxPoly]) + 1;
    //int numchans = highch-lowch+1;
    if (newpoly < poly)
    {
        dbg("max polyphony decreased");
    }
    poly        = newpoly;
    bool retrig = param[kRetrig] >= 0.5f;
    //bool mono=lowch==highch && poly==1;
    bool usesustain = param[kSustain] >= 0.5f;

    if (stealmode == centered && inputs[0].size())
    {
        bool held2[16][128];
        memcpy(held2, held, sizeof(held2));
        // re-pre-process incoming events
        for (unsigned int i = 0; i < inputs[0].size(); i++)
        {
            //copying event "i" from input (with all its fields)
            VstMidiEvent tomod = inputs[0][i];

            unsigned char status  = tomod.midiData[0] & 0xf0; // scraping  channel
            unsigned char channel = tomod.midiData[0] & 0x0f; // isolating channel (1-16)
            unsigned char data1   = tomod.midiData[1] & 0x7f;
            unsigned char data2   = tomod.midiData[2] & 0x7f;

            //make 0-velocity notes look like "real" noteoffs for simplicity
            if (status == MIDI_NOTEON && data2 == 0)
            {
                status = MIDI_NOTEOFF;
            }
            if (status == MIDI_NOTEON)
            {
                held2[channel][data1] = true;
            }
            else if (status == MIDI_NOTEOFF)
            { //process every note-off to avoid hanging notes
                held2[channel][data1] = false;
            }
        }
        //find center note
        int top    = -1;
        int bottom = 128;
        std::vector<int> heldNotes;
        for (int n = 0; n < 128; n++)
        {
            for (int ch = 0; ch < 16; ch++)
            {
                if (held2[ch][n])
                {
                    heldNotes.push_back(n);
                    if (n < bottom)
                    {
                        bottom = n;
                    }
                    if (n > top)
                    {
                        top = n;
                    }
                }
            }
        }
        if (heldNotes.size() > 1)
        {
            //int c = (int)((float)heldNotes.size()/2.f);
            int total = 0;
            for (unsigned int i = 0; i < heldNotes.size(); i++)
            {
                total += heldNotes[i];
            }
            centerNote = roundToInt((float) total / (float) heldNotes.size());
            //if (heldNotes.size()%2==0)
            //	centerNote = roundToInt(0.5f * (float)(heldNotes[c]+heldNotes[c+1]));
            //else
            //	centerNote = heldNotes[c];
            sorter.centerNote = centerNote;
        }
        std::sort(inputs[0].begin(), inputs[0].end(), sorter);
    }

    memset(ntime, -1, sizeof(ntime));
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        unsigned char status  = tomod.midiData[0] & 0xf0; // scraping  channel
        unsigned char channel = tomod.midiData[0] & 0x0f; // isolating channel (1-16)
        unsigned char data1   = tomod.midiData[1] & 0x7f;
        unsigned char data2   = tomod.midiData[2] & 0x7f;

        //make 0-velocity notes look like "real" noteoffs for simplicity
        if (status == MIDI_NOTEON && data2 == 0)
        {
            status = MIDI_NOTEOFF;
        }

        bool discard = false;

        //only look at the selected channel for note-ons
        if (status == MIDI_NOTEON)
        {
            held[channel][data1]              = true;
            sorter.onvelocity[channel][data1] = data2;
            findCenterNote();
            if ((channel == listenchannel || listenchannel == -1) && param[kPower] >= 0.5f)
            {
                ntime[data1][channel] = tomod.deltaFrames;
                //start with the low channel, but use input channel if possible
                char outch = lowch;
                if (channel > lowch && channel <= highch)
                {
                    outch = channel;
                }
                dbg("noteon n" << (int) data1 << " ch" << (int) channel << ", " << voices[outch] << " voices");
                char outn = voices[outch] - 1;
                v stealme;
                //oldness: older==smaller
                if (stealmode == oldest)
                {
                    stealme.oldness = 0xFFFFFFFF;
                }
                else
                {
                    stealme.oldness = 0;
                }
                stealme.inch = outch;
                if (stealmode == highest)
                {
                    stealme.note = -1;
                }
                else if (stealmode == centered)
                {
                    stealme.note = centerNote;
                }
                else
                {
                    stealme.note = 128;
                }
                stealme.vel = 128;
                stealme.on  = true;

                bool stop    = false;
                bool nosteal = false;
                bool noreuse = param[kMode] >= 0.5f;
                if (noreuse)
                {
                    //always prefer a new channel
                    if (lastoutch == -1)
                    {
                        outch = lowch;
                    }
                    else
                    {
                        outch = lastoutch + 1;
                        if (outch > highch)
                        {
                            outch = lowch;
                        }
                        while (voices[outch] >= poly)
                        {
                            outch++;
                            if (outch > highch)
                            {
                                outch = lowch;
                            }
                            if (outch == lastoutch)
                            {
                                //need voice stealing below
                                break;
                            }
                        }
                    }
                }
                if (sounding[outch][data1] && ! noreuse)
                {
                    for (int n = 0; n < poly; n++)
                    {
                        if (Voice[outch][n].note == data1)
                        {
                            dbg("reusing voice " << n);
                            outn = n;
                            stop = true;
                            //kill note before restarting it
                            VstMidiEvent kill;
                            kill.midiData[0] = MIDI_NOTEOFF + outch;
                            kill.midiData[1] = data1;
                            kill.midiData[2] = 0;
                            kill.deltaFrames = tomod.deltaFrames;
                            kill.detune      = 0;
                            outputs[0].push_back(kill);
                            --voices[outch];
                        }
                    }
                }
                else if (voices[outch] < poly)
                {
                    dbg("voices are free, find one");
                    for (int n = 0; n < poly; n++)
                    {
                        if (stop)
                        {
                            break;
                        }
                        if (Voice[outch][n].on == false)
                        {
                            dbg("found empty voice " << n);
                            outn                   = n;
                            stop                   = true;
                            sounding[outch][data1] = true;
                        }
                    }
                }
                else
                {
                    dbg("channel full");
                    if (lowch != highch)
                    {
                        //if multiple output channels, find a channel with free voices
                        for (int ch = lowch; ch <= highch; ch++)
                        {
                            if (stop)
                            {
                                break;
                            }
                            if (voices[ch] < poly)
                            {
                                dbg("found empty channel " << ch);
                                outch = ch;
                                for (int n = 0; n < poly; n++)
                                {
                                    if (stop)
                                    {
                                        break;
                                    }
                                    if (Voice[ch][n].on == false)
                                    {
                                        dbg("found empty voice " << n);
                                        outn                   = n;
                                        stop                   = true;
                                        sounding[outch][data1] = true;
                                    }
                                    else
                                    {
                                        //something is wrong...
                                        dbg("HUH????? line 869");
                                    }
                                }
                            }
                        }
                    }
                    if (! stop)
                    {
                        dbg("all channels full");
                        //all channels full, need voice stealing
                        //todo: don't steal sometimes if dictated by note priority
                        for (int ch = lowch; ch <= highch; ch++)
                        {
                            for (int n = 0; n < poly; n++)
                            {
                                if (Voice[ch][n].on)
                                {
                                    //determine which voice to steal
                                    switch (stealmode)
                                    {
                                        case oldest:
                                            if (Voice[ch][n].oldness < stealme.oldness)
                                            {
                                                //find oldest note for stealing
                                                stealme.oldness = Voice[ch][n].oldness;
                                                stealme.note    = Voice[ch][n].note;
                                                outch           = ch;
                                                outn            = n;
                                            }
                                            break;
                                        case newest:
                                            if (Voice[ch][n].oldness >= stealme.oldness)
                                            {
                                                //find oldest note for stealing
                                                stealme.oldness = Voice[ch][n].oldness;
                                                stealme.note    = Voice[ch][n].note;
                                                outch           = ch;
                                                outn            = n;
                                            }
                                            break;
                                        case quietest:
                                            if (Voice[ch][n].vel < stealme.vel)
                                            {
                                                //find quietest note for stealing
                                                stealme.vel  = Voice[ch][n].vel;
                                                stealme.note = Voice[ch][n].note;
                                                outch        = ch;
                                                outn         = n;
                                            }
                                            break;
                                        case lowest:
                                            if (Voice[ch][n].note < stealme.note)
                                            {
                                                //find lowest note for stealing
                                                stealme.note = Voice[ch][n].note;
                                                outch        = ch;
                                                outn         = n;
                                            }
                                            break;
                                        case highest:
                                            if (Voice[ch][n].note > stealme.note)
                                            {
                                                //find highest note for stealing
                                                stealme.note = Voice[ch][n].note;
                                                outch        = ch;
                                                outn         = n;
                                            }
                                            break;
                                        case centered:
                                            //steal note farthest from center
                                            if (abs(Voice[ch][n].note - centerNote) > abs(stealme.note - centerNote))
                                            {
                                                stealme.note = Voice[ch][n].note;
                                                outch        = ch;
                                                outn         = n;
                                            }
                                            break;
                                        case random:
                                            //steal random voice
                                            if (rand() < RAND_MAX / 2)
                                            {
                                                stealme.note = Voice[ch][n].note;
                                                outch        = ch;
                                                outn         = n;
                                                nosteal      = false;
                                            }
                                            else
                                            {
                                                nosteal = true;
                                            }

                                    } //switch(stealmode)
                                }     //if voice on
                            }         //for n
                        }             //for ch
                        //if (retrig) {
                        if (stealmode == lowest && stealme.note > data1)
                        {
                            nosteal = true;
                        }
                        else if (stealmode == highest && stealme.note < data1)
                        {
                            nosteal = true;
                        }
                        else if (stealmode == centered && abs(stealme.note - centerNote) < abs(data1 - centerNote))
                        {
                            nosteal = true;
                        }
                        // else if (ntime[stealme.note][stealme.chan] == tomod.deltaFrames)
                        // {
                        //     nosteal = true;
                        // }
                        stealme = copyVoice(Voice[outch][outn]);
                        dbg("stealme n=" << stealme.note << " v=" << stealme.vel);
                        //}
                        if (! nosteal)
                        {
                            dbg("!nosteal");
                            //kill the stolen voice and put it in the queue for restarting
                            if (1 /*queued>=0 && queued<queueSize*/)
                            {
                                //todo: apply voice stealing to queue
                                //now it just ignores the (queueSize+1)th note
                                bool gotq = false;
                                for (int q = 0; q < queueSize; q++)
                                {
                                    if (gotq)
                                    {
                                        break;
                                    }
                                    if (Queue[q].on && Queue[q].note == stealme.note && Queue[q].chan == outch)
                                    {
                                        dbg("same note already queued q=" << q);
                                        Queue[q]           = copyVoice(stealme);
                                        Queue[q].on        = true;
                                        Queue[q].sustained = false;
                                        gotq               = true;
                                    }
#ifdef _DEBUG
                                    else
                                    {
                                        if (Queue[q].on || Queue[q].note)
                                        {
                                            dbg("no match q[" << q << "] on=" << Queue[q].on << " n=" << Queue[q].note << " ch=" << (int) Queue[q].chan);
                                        }
                                    }
#endif
                                }
                                for (int q = 0; q < queueSize; q++)
                                {
                                    if (gotq)
                                    {
                                        break;
                                    }
                                    if (! Queue[q].on)
                                    {
                                        dbg("found empty queue slot q=" << q);
                                        Queue[q]           = copyVoice(stealme);
                                        Queue[q].on        = true;
                                        Queue[q].sustained = false;
                                        gotq               = true;
                                        //++queued;
                                    }
                                }
                                dbg("voice [" << (int) outch << "][" << (int) outn << "]." << Voice[outch][outn].note << " stolen, queued");
                            }
                            else
                            {
                                discard = true;
                            }
                            VstMidiEvent kill;
                            kill.midiData[0] = MIDI_NOTEOFF + outch;
                            kill.midiData[1] = (char) stealme.note;
                            kill.midiData[2] = 0;
                            kill.deltaFrames = tomod.deltaFrames;
                            kill.detune      = 0;
                            outputs[0].push_back(kill);
                            sounding[outch][stealme.note] = false;
                            --voices[outch];
                            Voice[outch][outn].on = false;
                            if (! sustain)
                            {
                                Voice[outch][outn].sustained = false;
                            }
                            dbg("voice [" << (int) outch << "][" << (int) outn << "]." << Voice[outch][outn].note << " killed, " << voices[outch] << " voices");
                        }
                    } //voice-stealing
                }     //voices[channel]>=poly
                if (nosteal)
                {
                    //queue the note instead
                    if (1 /*queued>=0 && queued<queueSize*/)
                    {
                        //todo: apply voice stealing to queue
                        //now it just ignores the (queueSize+1)th note
                        bool gotq = false;
                        for (int q = 0; q < queueSize; q++)
                        {
                            if (gotq)
                            {
                                break;
                            }
                            if (Queue[q].on && Queue[q].note == data1 && Queue[q].chan == outch)
                            {
                                Queue[q].oldness   = ++oldness;
                                Queue[q].inch      = channel;
                                Queue[q].chan      = outch;
                                Queue[q].note      = data1;
                                Queue[q].vel       = data2;
                                Queue[q].on        = true;
                                Queue[q].sustained = false;
                                gotq               = true;
                            }
                        }
                        for (int q = 0; q < queueSize; q++)
                        {
                            if (gotq)
                            {
                                break;
                            }
                            if (! Queue[q].on)
                            {
                                Queue[q].oldness   = ++oldness;
                                Queue[q].inch      = channel;
                                Queue[q].chan      = outch;
                                Queue[q].note      = data1;
                                Queue[q].vel       = data2;
                                Queue[q].on        = true;
                                Queue[q].sustained = false;
                                gotq               = true;
                                //++queued;
                                dbg("voice [" << (int) outch << "][" << (int) outn << "]." << Voice[outch][outn].note << " not used, queued");
                            }
                        }
                    }
                    discard = true;
                    dbg(voices[outch] << " voices");
                }
                else
                {
                    Voice[outch][outn].oldness   = ++oldness;
                    Voice[outch][outn].inch      = channel;
                    Voice[outch][outn].chan      = outch;
                    Voice[outch][outn].note      = data1;
                    Voice[outch][outn].vel       = data2;
                    Voice[outch][outn].on        = true;
                    Voice[outch][outn].sustained = false;
                    tomod.midiData[0]            = outch | MIDI_NOTEON;
                    sorter.priorityNote          = data1;
                    sounding[outch][data1]       = true;
                    ++voices[outch];
                    lastoutch = outch;

                    dbg("voice [" << (int) outch << "][" << (int) outn << "]." << Voice[outch][outn].note << " starting, " << voices[outch] << " voices");

                    bool gotq = false;
                    for (int q = 0; q < queueSize; q++)
                    {
                        if (gotq)
                        {
                            break;
                        }
                        if (Queue[q].on && Queue[q].note == data1 && Queue[q].chan == outch)
                        {
                            dbg("playing note already queued q=" << q);
                            Queue[q].on        = false;
                            Queue[q].sustained = false;
                            gotq               = true;
                            //dbg(queued<<" queued");
                        }
#ifdef _DEBUG
                        else
                        {
                            if (Queue[q].on || Queue[q].note)
                            {
                                dbg("no match q[" << q << "] on=" << Queue[q].on << " n=" << Queue[q].note << " ch=" << (int) Queue[q].chan);
                            }
                        }
#endif
                    }
                }
            }
        } //if note-on
        else if (status != MIDI_NOTEOFF)
        {
            if (status == MIDI_CONTROLCHANGE && data1 == MIDI_SUSTAIN && usesustain)
            {
                discard = true;
                if (data2 > 63)
                {
#ifdef _DEBUG
                    if (! sustain)
                    {
                        dbg("sustain on");
                    }
#endif
                    sustain = true;
                }
                else
                {
                    sustain = false;
                    for (int q = 0; q < queueSize; q++)
                    {
                        if (Queue[q].on && Queue[q].sustained)
                        {
                            //--queued;
                            Queue[q].sustained = false;
                            Queue[q].on        = false;
                            dbg("sustain off, q[" << q << "]." << Queue[q].note << " off");
                        }
                    }
                    for (int ch = 0; ch < MIDI_MAX_CHANNELS; ch++)
                    {
                        for (int n = 0; n < maxPoly; n++)
                        {
                            if (Voice[ch][n].on && Voice[ch][n].sustained)
                            {
                                //end all sustained notes
                                VstMidiEvent off;
                                off.midiData[0] = MIDI_NOTEOFF + ch;
                                off.midiData[1] = (char) Voice[ch][n].note;
                                off.midiData[2] = 0;
                                off.deltaFrames = tomod.deltaFrames;
                                off.detune      = 0;
                                outputs[0].push_back(off);
                                sounding[ch][Voice[ch][n].note] = false;
                                --voices[ch];
                                Voice[ch][n].on        = false;
                                Voice[ch][n].sustained = false;
                                dbg("sustain off, v[" << ch << "][" << n << "]." << Voice[ch][n].note << " off, " << voices[ch] << " voices");
                            }
                        }
                    }
                }
            }
            else if (status == MIDI_CONTROLCHANGE && data1 == MIDI_ALL_NOTES_OFF)
            {
                //ignore, see below
            }
            else
            {
                //send other midi to all used channels
                //this part crashes reaper on startup (when it sends pitchbend 0 on all channels)
                if (! (status == MIDI_PITCHBEND && data1 == 0 && data2 == 64 && ! playing && ! wasplaying))
                {
                    for (int ch = lowch; ch <= highch; ch++)
                    {
                        VstMidiEvent m;
                        m.midiData[0] = status | ch;
                        m.midiData[1] = data1;
                        m.midiData[2] = data2;
                        m.deltaFrames = tomod.deltaFrames;
                        m.detune      = 0;
                        outputs[0].push_back(m);
                        discard = true; //to avoid sending twice on input channel
                    }
                }
            }
        }
        else if (status == MIDI_NOTEOFF)
        { //process every note-off to avoid hanging notes
            held[channel][data1]              = false;
            sorter.onvelocity[channel][data1] = 0;
            findCenterNote();
            dbg("noteoff input n" << (int) data1 << " ch" << (int) channel);
            discard            = true;
            bool done          = false;
            bool queuednoteoff = false;
            //see if the note is in the queue
            for (int q = 0; q < queueSize; q++)
            {
                if (Queue[q].inch == channel && Queue[q].note == data1 && Queue[q].on)
                {
                    //ending a silent queued note
                    if (sustain)
                    {
                        Queue[q].sustained = true;
                        dbg("silent queued note q[" << q << "]." << Queue[q].note << " sustained");
                    }
                    else
                    {
                        Queue[q].on        = false;
                        Queue[q].sustained = false;
                        //--queued;
                        dbg("queued note q[" << q << "]." << Queue[q].note << " note-off");
                    }
                    queuednoteoff = true;
                }
                if (queuednoteoff)
                {
                    break;
                }
            }
            //see if the note is playing
            if (! queuednoteoff)
            {
                for (int ch = 0; ch < MIDI_MAX_CHANNELS; ch++)
                {
                    if (done)
                    {
                        break;
                    }
                    for (int n = 0; n < maxPoly; n++)
                    {
                        if (done)
                        {
                            break;
                        }
                        if (Voice[ch][n].inch == channel && Voice[ch][n].note == data1 && Voice[ch][n].on)
                        {
                            //got match
                            done = true;
                            if (sustain)
                            {
                                Voice[ch][n].sustained = true;
                                dbg("voice [" << ch << "][" << n << "]." << Voice[ch][n].note << " sustained, " << voices[ch] << " voices");
                            }
                            else
                            {
                                VstMidiEvent off = tomod;
                                off.midiData[0]  = MIDI_NOTEOFF | ch;
                                off.midiData[1]  = (char) Voice[ch][n].note;
                                off.midiData[2]  = 0;
                                off.deltaFrames  = tomod.deltaFrames;
                                off.detune       = 0;
                                outputs[0].push_back(off);
                                sounding[ch][Voice[ch][n].note] = false;
                                --voices[ch];
                                //if (voices[outch]<0) voices[outch]=0;
                                Voice[ch][n].on        = false;
                                Voice[ch][n].sustained = false;
                                dbg("voice [" << ch << "][" << n << "]." << Voice[ch][n].note << " stopped, " << voices[ch] << " voices");
                                //this voice is now free, so look for a queued note
                                if (1 /*queued>0*/)
                                {
                                    v useme;
                                    //oldness: older==smaller
                                    if (stealmode == oldest)
                                    {
                                        useme.oldness = 0;
                                    }
                                    else
                                    {
                                        useme.oldness = 0xFFFFFFFF;
                                    }
                                    useme.inch = 0;
                                    if (stealmode == highest)
                                    {
                                        useme.note = 128;
                                    }
                                    else
                                    {
                                        useme.note = -1;
                                    }
                                    useme.vel = 1;
                                    useme.on  = true;
                                    int useq  = 0;
                                    bool stop = false;
                                    for (int q = 0; q < queueSize; q++)
                                    {
                                        if (Queue[q].on && ! stop)
                                        {
                                            switch (stealmode)
                                            {
                                                case oldest: //steal oldest==replace with newest???
                                                    if (Queue[q].oldness >= useme.oldness)
                                                    {
                                                        //find newest queued note
                                                        useme = copyVoice(Queue[q]);
                                                        useq  = q;
                                                    }
                                                    break;
                                                case newest:
                                                    if (Queue[q].oldness < useme.oldness)
                                                    {
                                                        //find oldest queued note
                                                        useme = copyVoice(Queue[q]);
                                                        useq  = q;
                                                    }
                                                    break;
                                                case quietest:
                                                    if (Queue[q].vel >= useme.vel)
                                                    {
                                                        //find loudest queued note
                                                        useme = copyVoice(Queue[q]);
                                                        useq  = q;
                                                    }
                                                    break;
                                                case lowest:
                                                    if (Queue[q].note > useme.note)
                                                    {
                                                        //find highest queued note
                                                        useme = copyVoice(Queue[q]);
                                                        useq  = q;
                                                    }
                                                    break;
                                                case highest:
                                                    if (Queue[q].note < useme.note)
                                                    {
                                                        //find lowest queued note
                                                        useme = copyVoice(Queue[q]);
                                                        useq  = q;
                                                    }
                                                    break;
                                                case centered:
                                                    if (abs(Queue[q].note - centerNote) < abs(useme.note - centerNote))
                                                    {
                                                        //find lowest queued note
                                                        useme = copyVoice(Queue[q]);
                                                        useq  = q;
                                                    }
                                                    break;
                                                case random:
                                                    if (rand() < RAND_MAX / 2)
                                                    {
                                                        //get random note
                                                        useme = copyVoice(Queue[q]);
                                                        useq  = q;
                                                    }
                                                    else
                                                    {
                                                        //in case no voice is found
                                                        if (useme.note == -1)
                                                        {
                                                            useme = copyVoice(Queue[0]);
                                                            useq  = q;
                                                        }
                                                    }
                                                    break;
                                            } // /switch
                                        }     //if Queue[q].on
                                    }         // /for 0<=q<16
                                    //send the queued note
                                    if (useme.note != -1)
                                    {
                                        //if (voices[useme.chan]>=maxPoly) {
                                        useme.chan = ch;
                                        //}
                                        if (retrig)
                                        {
                                            VstMidiEvent queue = tomod;
                                            queue.midiData[0]  = MIDI_NOTEON + useme.chan;
                                            queue.midiData[1]  = (unsigned char) useme.note;
                                            queue.midiData[2]  = (unsigned char) useme.vel;
                                            queue.deltaFrames  = tomod.deltaFrames;
                                            queue.detune       = 0;
                                            //--queued;
                                            Queue[useq].on        = false;
                                            Queue[useq].sustained = false;
                                            outputs[0].push_back(queue);
                                            dbg("queued note q[" << useq << "]." << Queue[useq].note << " unqueued");
                                            //update the voice
                                            Voice[ch][n]                     = copyVoice(useme);
                                            Voice[ch][n].sustained           = false;
                                            sounding[useme.chan][useme.note] = true;
                                            ++voices[ch];
                                            dbg("voice [" << ch << "][" << n << "]." << Voice[ch][n].note << " retriggered, " << voices[ch] << " voices");
                                        }
                                    }
                                    else
                                    {
                                        dbg("queue empty");
                                    }
                                } // /if queued>0
                            }
                        } //got match
                    }     // for n
                }         //for ch
            }             //if(!done)
            if (done)
            {
                discard = true;
            }
            else if (! queuednoteoff)
            {
                discard = false;
                dbg("no match????, v=" << voices[channel] << " s=" << (int) sustain << " n=" << (int) data1);
            }
        } //if note-off
        if (status == MIDI_CONTROLCHANGE && data1 == MIDI_ALL_NOTES_OFF)
        {
            dbg("all notes off ch" << (int) channel);
            memset(held, 0, sizeof(held));
            memset(sounding, 0, sizeof(sounding));
            //queued=0;
            voices[channel] = 0;
            sustain         = false;
            for (int i = 0; i < maxPoly; i++)
            {
                Voice[channel][i].on        = false;
                Voice[channel][i].sustained = false;
            }
            for (int q = 0; q < queueSize; q++)
            {
                if (Queue[q].chan == channel)
                {
                    Queue[q].on        = false;
                    Queue[q].sustained = false;
                }
            }
        }
        if (! discard)
        {
            outputs[0].push_back(tomod);
        }
    }

    if (wasplaying && ! playing)
    { //just stopped
        dbg("stopped");
        //queued=0;
        sustain = false;
        //end artifically sustained notes
        for (int channel = 0; channel < MIDI_MAX_CHANNELS; channel++)
        {
            voices[channel] = 0;
            for (int i = 0; i < maxPoly; i++)
            {
                if (Voice[channel][i].sustained)
                {
                    VstMidiEvent kill;
                    kill.midiData[0] = MIDI_NOTEOFF + channel;
                    kill.midiData[1] = (char) Voice[channel][i].note;
                    kill.midiData[2] = 0;
                    kill.deltaFrames = 0;
                    kill.detune      = 0;
                    outputs[0].push_back(kill);
                    Voice[channel][i].on                      = false;
                    Voice[channel][i].sustained               = false;
                    sounding[channel][Voice[channel][i].note] = false;
                }
            }
        }
        for (int q = 0; q < queueSize; q++)
        {
            if (Queue[q].sustained)
            {
                //VstMidiEvent kill;
                //kill.midiData[0] = MIDI_NOTEOFF + Queue[q].chan;
                //kill.midiData[1] = (char)Queue[q].note;
                //kill.midiData[2] = 0;
                //kill.deltaFrames = 0;
                //kill.detune = 0;
                //outputs[0].push_back(kill);
                //sounding[Queue[q].chan][Queue[q].note]=false;
                Queue[q].on        = false;
                Queue[q].sustained = false;
            }
        }
    }
    wasplaying = playing;

    //clean 0-length notes from stream here?????????
}

v MidiPolyphony::copyVoice(v in)
{
    v out;
    out.oldness   = in.oldness;
    out.inch      = in.inch;
    out.chan      = in.chan;
    out.note      = in.note;
    out.vel       = in.vel;
    out.on        = in.on;
    out.sustained = in.sustained;
    return out;
}

void MidiPolyphony::findCenterNote()
{
    //find center note
    int top    = -1;
    int bottom = 128;
    std::vector<int> heldNotes;
    for (int n = 0; n < 128; n++)
    {
        for (int ch = 0; ch < 16; ch++)
        {
            if (held[ch][n])
            {
                heldNotes.push_back(n);
                if (n < bottom)
                {
                    bottom = n;
                }
                if (n > top)
                {
                    top = n;
                }
            }
        }
    }
    if (heldNotes.size() > 1)
    {
        //int c = (int)((float)heldNotes.size()/2.f);
        int total = 0;
        for (unsigned int i = 0; i < heldNotes.size(); i++)
        {
            total += heldNotes[i];
        }
        centerNote = roundToInt((float) total / (float) heldNotes.size());
        //if (heldNotes.size()%2==0)
        //	centerNote = roundToInt(0.5f * (float)(heldNotes[c]+heldNotes[c+1]));
        //else
        //	centerNote = heldNotes[c];
        sorter.centerNote = centerNote;
    }
}
