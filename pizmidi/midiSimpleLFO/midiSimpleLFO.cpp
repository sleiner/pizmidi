#include "midiSimpleLFO.hpp"
#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <time.h>

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new MidiSimpleLFO (audioMaster);
}

MidiSimpleLFOProgram::MidiSimpleLFOProgram()
{
    // default Program Values
    fWave = 0.0f;
    fCC = MIDI_TO_FLOAT (1.1);
    fChannel = 0.0f;
    fSync = 0.0f;
    fFreq = 0.333333333333333f;
    fPhase = 0.5f;
    fRange = 1.0f; //0..127
    fOffset = 0.5f;
    fTrigger = 0.0f;
    fPower = 1.0f;
    // default program name
    strcpy (name, "Default");
}

//-----------------------------------------------------------------------------
MidiSimpleLFO::MidiSimpleLFO (audioMasterCallback audioMaster)
    : PizMidi (audioMaster, kNumPrograms, kNumParams), programs (0)
{
    programs = new MidiSimpleLFOProgram[numPrograms];

    if (programs)
    {
        CFxBank* defaultBank = new CFxBank (kNumPrograms, kNumParams);
        if (readDefaultBank (PLUG_NAME, defaultBank))
        {
            if ((VstInt32) defaultBank->GetFxID() == PLUG_IDENT)
            {
                for (int i = 0; i < kNumPrograms; i++)
                {
                    programs[i].fWave = defaultBank->GetProgParm (i, 0);
                    programs[i].fCC = defaultBank->GetProgParm (i, 1);
                    programs[i].fChannel = defaultBank->GetProgParm (i, 2);
                    programs[i].fSync = defaultBank->GetProgParm (i, 3);
                    programs[i].fFreq = defaultBank->GetProgParm (i, 4);
                    programs[i].fPhase = defaultBank->GetProgParm (i, 5);
                    programs[i].fRange = defaultBank->GetProgParm (i, 6);
                    programs[i].fOffset = defaultBank->GetProgParm (i, 7);
                    programs[i].fTrigger = defaultBank->GetProgParm (i, 8);
                    programs[i].fPower = defaultBank->GetProgParm (i, 9);
                    strcpy (programs[i].name, defaultBank->GetProgramName (i));
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
                        programs[i].fWave = 0.7f;
                        programs[i].fSync = 1.0f;
                        programs[i].fTrigger = 0.9f;
                        programs[i].fFreq = 0.15f;
                        sprintf (programs[i].name, "1 bar Sine");
                        break;
                    case 1:
                        programs[i].fWave = 0.0f;
                        sprintf (programs[i].name, "1Hz Saw");
                        break;
                    default:
                        sprintf (programs[i].name, "Program %d", i + 1);
                }
            }
            setProgram (0);
        }
    }

    if (programs)
        setProgram (0);

    oldenv = 0;
    samp = 0;
    data2 = 0;
    on = false;
    retrigger = false;
    voices = 0;
    triggerme = false;
    barstarted = false;
    beats = 4;
    ppqPerBar = 4;
    _ppq = 0;
    lastppq = 0;
    _bpm = 120;
    numerator = 4;
    //modinput=0;
    playing = false;
    ccsent = false;

    srand ((unsigned int) time (NULL));
    init();
}

//-----------------------------------------------------------------------------------------
MidiSimpleLFO::~MidiSimpleLFO()
{
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void MidiSimpleLFO::setProgram (VstInt32 program)
{
    MidiSimpleLFOProgram* ap = &programs[program];

    curProgram = program;
    setParameter (kWave, ap->fWave);
    setParameter (kCC, ap->fCC);
    setParameter (kChannel, ap->fChannel);
    setParameter (kSync, ap->fSync);
    setParameter (kFreq, ap->fFreq);
    setParameter (kPhase, ap->fPhase);
    setParameter (kRange, ap->fRange);
    setParameter (kOffset, ap->fOffset);
    setParameter (kTrigger, ap->fTrigger);
    setParameter (kPower, ap->fPower);
}

//------------------------------------------------------------------------
void MidiSimpleLFO::setProgramName (char* name)
{
    vst_strncpy (programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiSimpleLFO::getProgramName (char* name)
{
    strcpy (name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool MidiSimpleLFO::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy (text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiSimpleLFO::setParameter (VstInt32 index, float value)
{
    float old = fTrigger;

    MidiSimpleLFOProgram* ap = &programs[curProgram];

    switch (index)
    {
        case kWave:
            fWave = ap->fWave = value;
            break;
        case kCC:
            fCC = ap->fCC = value;
            break;
        case kChannel:
            fChannel = ap->fChannel = value;
            break;
        case kSync:
            fSync = ap->fSync = value;
            updateDisplay();
            break;
        case kFreq:
            fFreq = ap->fFreq = value;
            break;
        case kPhase:
            if (fabs (value - 0.25) < 0.005)
                fPhase = ap->fPhase = 0.25;
            else if (fabs (value - 0.5) < 0.005)
                fPhase = ap->fPhase = 0.5;
            else if (fabs (value - 0.75) < 0.005)
                fPhase = ap->fPhase = 0.75;
            else
                fPhase = ap->fPhase = value;
            break;
        case kRange:
            fRange = ap->fRange = value;
            break;
        case kOffset:
            fOffset = ap->fOffset = value;
            break;
        case kTrigger:
            fTrigger = ap->fTrigger = value;
            if (fTrigger < 0.3)
            {
                if (old >= 0.3)
                    on = false;
            }
            else if (fTrigger < 0.7)
            {
                if (old < 0.3 || old >= 0.7)
                    on = false;
            }
            else if (fTrigger < 1.0)
            {
                if (old < 0.7 || old == 1.0)
                    on = true;
            }
            else
                on = true;
            break;
        case kPower:
            fPower = ap->fPower = value;
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
float MidiSimpleLFO::getParameter (VstInt32 index)
{
    float v = 0;

    switch (index)
    {
        case kWave:
            v = fWave;
            break;
        case kCC:
            v = fCC;
            break;
        case kChannel:
            v = fChannel;
            break;
        case kSync:
            v = fSync;
            break;
        case kFreq:
            v = fFreq;
            break;
        case kPhase:
            v = fPhase;
            break;
        case kRange:
            v = fRange;
            break;
        case kOffset:
            v = fOffset;
            break;
        case kTrigger:
            v = fTrigger;
            break;
        case kPower:
            v = fPower;
            break;
        default:
            break;
    }
    return v;
}

//-----------------------------------------------------------------------------------------
void MidiSimpleLFO::getParameterName (VstInt32 index, char* label)
{
    switch (index)
    {
        case kWave:
            strcpy (label, "Waveform");
            break;
        case kCC:
            strcpy (label, "CC Out");
            break;
        case kChannel:
            strcpy (label, "Channel Out");
            break;
        case kSync:
            strcpy (label, "Freq Mode");
            break;
        case kFreq:
            strcpy (label, "Frequency");
            break;
        case kPhase:
            strcpy (label, "Phase");
            break;
        case kRange:
            strcpy (label, "Amplitude");
            break;
        case kOffset:
            strcpy (label, "Offset");
            break;
        case kTrigger:
            strcpy (label, "Triggering");
            break;
        case kPower:
            strcpy (label, "Power");
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiSimpleLFO::getParameterDisplay (VstInt32 index, char* text)
{
    switch (index)
    {
        case kWave:
            if (fWave == 0.0)
                strcpy (text, "Sawtooth");
            else if (fWave < 0.2)
                strcpy (text, "Ramp");
            else if (fWave < 0.4)
                strcpy (text, "Triangle");
            else if (fWave < 0.6)
                strcpy (text, "Square");
            else if (fWave < 0.8)
                strcpy (text, "Sine");
            else if (fWave < 1.0)
                strcpy (text, "Slow Random");
            else
                strcpy (text, "Fast Random");
            break;
        case kCC:
            sprintf (text, "%d", FLOAT_TO_MIDI (fCC));
            break;
        case kChannel:
            sprintf (text, "%d", FLOAT_TO_CHANNEL015 (fChannel) + 1);
            break;
        case kSync:
            if (fSync < 0.5)
                strcpy (text, "Hz");
            else
                strcpy (text, "Tempo");
            break;
        case kFreq:
            if (fSync < 0.5)
                sprintf (text, "%f", 27 * fFreq * fFreq * fFreq);
            else
            {
                if (fFreq == 0.0f)
                    strcpy (text, "32 bars");
                else if (fFreq < 0.01)
                    strcpy (text, "24 bars");
                else if (fFreq < 0.02)
                    strcpy (text, "16 bars");
                else if (fFreq < 0.03)
                    strcpy (text, "12 bars");
                else if (fFreq < 0.04)
                    strcpy (text, "8 bars");
                else if (fFreq < 0.06)
                    strcpy (text, "6 bars");
                else if (fFreq < 0.08)
                    strcpy (text, "4 bars");
                else if (fFreq < 0.10)
                    strcpy (text, "3 bars");
                else if (fFreq < 0.12)
                    strcpy (text, "2 bars");
                else if (fFreq < 0.14)
                    strcpy (text, "1.5 bars");
                else if (fFreq < 0.16)
                    strcpy (text, "1 bar");
                else if (fFreq < 0.20)
                    strcpy (text, "dotted half");
                else if (fFreq < 0.3)
                    strcpy (text, "half");
                else if (fFreq < 0.35)
                    strcpy (text, "dotted quarter");
                else if (fFreq < 0.4)
                    strcpy (text, "quarter");
                else if (fFreq < 0.5)
                    strcpy (text, "dotted 8th");
                else if (fFreq < 0.6)
                    strcpy (text, "4th triplet");
                else if (fFreq < 0.7)
                    strcpy (text, "8th");
                else if (fFreq < 0.8)
                    strcpy (text, "8th triplet");
                else if (fFreq < 0.9)
                    strcpy (text, "16th");
                else if (fFreq < 1.0)
                    strcpy (text, "16th triplet");
                else if (fFreq == 1.0)
                    strcpy (text, "32nd");
            }
            break;
        case kPhase:
            sprintf (text, "%.1f", fPhase * 360 - 180);
            break;
        case kRange:
            sprintf (text, "%d", FLOAT_TO_MIDI (fRange));
            break;
        case kOffset:
            sprintf (text, "%d", FLOAT_TO_MIDI (fOffset) - 64);
            break;
        case kTrigger:
            if (fTrigger < 0.3)
                strcpy (text, "Notes (Mono)");
            else if (fTrigger < 0.7)
                strcpy (text, "Notes (Poly)");
            else if (fTrigger < 1.0)
                strcpy (text, "Beat Sync");
            else
                strcpy (text, "Free Running");
            break;
        case kPower:
            if (fPower < 0.5)
                strcpy (text, "Off");
            else
                strcpy (text, "On");
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiSimpleLFO::getParameterLabel (VstInt32 index, char* label)
{
    switch (index)
    {
        case kFreq:
            if (fSync < 0.5)
                strcpy (label, "Hz");
            else
                strcpy (label, "");
            break;
        case kPhase:
            strcpy (label, "°");
            break;
        default:
            strcpy (label, "");
            break;
    }
}

void MidiSimpleLFO::preProcess (void)
{
    // preparing Proccess
    sampleRate = getSampleRate();
    VstTimeInfo* timeInfo = NULL;
    timeInfo = getTimeInfo (0xffff); //ALL

    numerator = 4;
    int denominator = 4;
    lastppq = _ppq;
    //playing=false;
    if (timeInfo)
    {
        if (kVstTempoValid & timeInfo->flags)
            _bpm = (float) timeInfo->tempo;
        if (kVstPpqPosValid & timeInfo->flags)
            _ppq = (float) timeInfo->ppqPos;
        if (kVstTimeSigValid & timeInfo->flags)
        {
            numerator = timeInfo->timeSigNumerator;
            denominator = timeInfo->timeSigDenominator;
        }
        if (kVstTransportPlaying & timeInfo->flags)
            playing = true;
        else
            playing = false;

        ppqPerBar = ((float) numerator * 4 / (float) denominator);

        beats = ppqPerBar;
        if (fSync >= 0.5)
        {
            if (fFreq == 0.0f)
                beats = ppqPerBar * 32; //32 bars
            else if (fFreq < 0.01)
                beats = ppqPerBar * 24; //12 bars
            else if (fFreq < 0.02)
                beats = ppqPerBar * 16; //12 bars
            else if (fFreq < 0.03)
                beats = ppqPerBar * 12; //12 bars
            else if (fFreq < 0.04)
                beats = ppqPerBar * 8; //8 bars
            else if (fFreq < 0.06)
                beats = ppqPerBar * 6; //6 bars
            else if (fFreq < 0.08)
                beats = ppqPerBar * 4; //4 bars
            else if (fFreq < 0.10)
                beats = ppqPerBar * 3; //3 bars
            else if (fFreq < 0.12)
                beats = ppqPerBar * 2; //2 bars
            else if (fFreq < 0.14)
                beats = ppqPerBar * 1.5f; //1.5 bars
            else if (fFreq < 0.16)
                beats = ppqPerBar; //1 bar
            else if (fFreq < 0.20)
                beats = 3; //3 beats
            else if (fFreq < 0.3)
                beats = 2; //2 beats
            else if (fFreq < 0.35)
                beats = 1.5f;
            else if (fFreq < 0.4)
                beats = 1;
            else if (fFreq < 0.5)
                beats = 0.75f;
            else if (fFreq < 0.6)
                beats = 0.6666666666666666666666667f;
            else if (fFreq < 0.7)
                beats = 0.5f;
            else if (fFreq < 0.8)
                beats = 0.3333333333333333333333333f;
            else if (fFreq < 0.9)
                beats = 0.25f;
            else if (fFreq < 1.0)
                beats = 0.1666666666666666666666667f;
            else if (fFreq == 1.0)
                beats = 0.125f;
        }
        double lastbeats = (fmod (lastppq, beats) / beats);
        _beats = (fmod (_ppq, beats) / beats);
        //if (_beats<0.01) barstarted=true;
        if (lastbeats > _beats)
            barstarted = true;
    }
    else
    {
        _bpm = 120.0f;
        ppqPerBar = 4;
    }
    beatsPerSample = _bpm / (beats * 60.0 * sampleRate);

    _cleanMidiOutBuffers();
}

void MidiSimpleLFO::processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        short status = tomod.midiData[0] & 0xf0; // scraping  channel
        //const short channel = tomod.midiData[0] & 0x0f;  // isolating channel (0-15)
        const short data1 = tomod.midiData[1] & 0x7f;
        const short data2 = tomod.midiData[2] & 0x7f;

        //make 0-velocity notes look like "real" noteoffs for simplicity
        if (status == MIDI_NOTEON && data2 == 0)
            status = MIDI_NOTEOFF;

        bool discard = false;

        if (status == MIDI_CONTROLCHANGE)
        {
            //don't send through LFO output CC:
            if (data1 == FLOAT_TO_MIDI (fCC))
            {
                discard = true;
            }
            if (data1 == MIDI_ALL_NOTES_OFF)
            {
                voices = 0; //not needed?
            }
        }
        //handle monophonic note triggering:
        else if (status == MIDI_NOTEON)
        {
            if (fTrigger >= 0.3 && fTrigger < 0.7)
                retrigger = true;
            else if (! on && fTrigger < 0.3)
                retrigger = true;
            ++voices;
            if (voices > 128)
                voices = 128;
            on = true;
            //discard=true;
        }
        else if (status == MIDI_NOTEOFF)
        {
            --voices;
            if (voices < 0)
                voices = 0;
            if (voices == 0 && fTrigger < 0.7)
                on = false;
            //discard=true;
        }
        if (! discard)
            outputs[0].push_back (tomod);
    }
    int channel = FLOAT_TO_CHANNEL015 (fChannel); //outgoing midi channel
    int data1 = FLOAT_TO_MIDI (fCC);
    float freq = 27 * fFreq * fFreq * fFreq;
    int offset = FLOAT_TO_MIDI (fOffset) - 64;

    if (fSync >= 0.5f)
    {
        freq = (_bpm / 60.0f) / beats;
    }

    if (fTrigger == 1.0f)
        on = true;

    int wave;
    if (fWave == 0.0)
        wave = 0; //saw
    else if (fWave < 0.2)
        wave = 1; //ramp
    else if (fWave < 0.4)
        wave = 2; //tri
    else if (fWave < 0.6)
        wave = 3; //square
    else if (fWave < 0.8)
        wave = 4; //sine
    else if (fWave < 1.0)
        wave = 5; //slow random
    else
        wave = 6; //fast random

    bool beatsync = fTrigger < 1.0f && fTrigger >= 0.7f && wave != 6;

    double period, steps, stepsize;
    if (freq > 0.0)
    {
        //find number of samples needed for a whole waveform period
        period = sampleRate / freq; //# of samples per cycle
        //divide that into some pieces (enough to go up and down smoothly)
        steps = 512.0;
        stepsize = period / steps; //# of samples per step
    }
    else
    {
        period = 0.0;
        steps = 1.0;
        stepsize = sampleRate / 500.0;
    }
    signed int phase = roundToInt ((fPhase - 0.5) * steps);
    if (phase < 0)
        phase = roundToInt (steps) + phase;

    if (beatsync && barstarted)
    {
        if (fSync < 0.5 || wave == 5)
            retrigger = true;
        on = true;
        barstarted = false;
    }

    //create LFO output
    for (VstInt32 i = 0; i < sampleFrames; i++)
    {
        _beats += beatsPerSample;
        if (freq > 0.0)
        {
            //if (fTrigger<1.0f && fTrigger>=0.7f) {
            //   if (lastppq<_ppq) samp+=1;
            //}
            //else
            ++samp;
        }
        if (retrigger)
        {
            if (wave < 5)
                oldenv = 999;
            ccsent = false;
            step = 0;
            retrigger = false;
            samp = roundToInt (stepsize) + 1;
        }
        //for every step, calculate the new value
        if (samp >= roundToInt (stepsize) || (beatsync && fSync >= 0.5))
        {
            if (beatsync && fSync >= 0.5)
                step = roundToInt (_beats * steps);
            else if (step >= steps)
            {
                step = 0;
            }
            if (step == 0)
                ccsent = false;

            //sawtooth
            if (wave == 0)
            {
                if (step < (steps - phase))
                    data2 = roundToInt ((-phase - step) * (128 / steps)) + 128;
                else
                    data2 = roundToInt ((steps - phase - step) * (128 / steps)) + 128;
            }

            //ramp
            else if (wave == 1)
            {
                if (step < (steps - phase))
                    data2 = roundToInt ((step + phase) * (128 / steps)) + 1;
                else
                    data2 = roundToInt ((step + phase - steps) * (128 / steps)) + 1;
            }

            //triangle
            else if (wave == 2)
            {
                double ph = (double) phase + (0.25 * steps);
                if (ph > steps)
                    ph = ph - steps;
                if (step < (steps * 0.5 - ph))
                    data2 = roundToInt (((double) step + ph) * (256.0 / steps));
                else if (step < (steps - ph))
                    data2 = roundToInt (-(step + ph) * (256 / steps)) + 256;
                else if (step < (1.5 * steps - ph))
                    data2 = roundToInt ((step + ph - steps) * (256 / steps));
                //else if (step<(2.0*steps-ph)) data2=(int)((step+ph)*(256/steps))-256;
                else
                    data2 = roundToInt (-(step + ph - steps) * (256 / steps)) + 256;
            }

            //square
            else if (wave == 3)
            {
                if (step < (steps * 0.5 - phase))
                    data2 = 127;
                else if (step < (steps - phase))
                    data2 = 0;
                else if (step < (1.5 * steps - phase))
                    data2 = 127;
                else
                    data2 = 0;
            }

            //sine
            else if (wave == 4)
            {
                data2 = roundToInt (64.0 * sin ((float) (step + phase) * 2 * PI / steps) + 64.0);
            }

            //random
            else if (wave == 5)
            {
                if (! ccsent)
                {
                    int randdata2 = rand() % 128; //random number between 0 and 127
                    data2 = (roundToInt ((float) randdata2 * fRange + 63 - (fRange * 127.0) / 2.0) + offset);
                    ccsent = true;
                }
                else
                    data2 = oldenv;
            }

            //fast random
            else if (wave == 6)
            {
                int randdata2 = rand() % 128; //random number between 0 and 127
                data2 = (roundToInt ((float) randdata2 * fRange + 63 - (fRange * 127.0) / 2.0) + offset);
            }

            //for non-random waveforms:
            if (wave < 5)
            {
                //amplitude
                data2 = roundToInt ((float) data2 * fRange + 63 - (fRange * 127.0) / 2.0);
                //offset
                data2 += offset;
            }
            if (data2 > 127)
                data2 = 127;
            else if (data2 < 0)
                data2 = 0;

            bool stop = (! playing && beatsync);
            if (wave == 6 && fTrigger < 1.0f && fTrigger >= 0.7f && ! playing)
                stop = true;
            else if (on && fPower >= 0.5f && ! stop)
            {
                //send the CC value if it's different from the last one we sent
                if (data2 != oldenv)
                {
                    VstMidiEvent cc;
                    cc.deltaFrames = i;
                    cc.midiData[0] = MIDI_CONTROLCHANGE + channel;
                    cc.midiData[1] = data1;
                    cc.midiData[2] = data2;
                    _midiEventsOut[0].push_back (cc);
                    oldenv = data2;
                }
            }
            if (! beatsync || fSync < 0.5)
                ++step;
            samp = 0;
        }
    }
}
