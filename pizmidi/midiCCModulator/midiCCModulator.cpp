#include "midiCCModulator.hpp"
#include "../common/MIDI.h"
#include <cstdlib>

//for seeding rand()
#include <time.h>

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new MidiCCModulator (audioMaster);
}

MidiCCModulatorProgram::MidiCCModulatorProgram()
{
    // default Program Values
    for (int i = 0; i < kNumParams; i++)
    {
        switch (i)
        {
            case kPower:
                param[i] = .5f;
                break;
            case kAmount:
                param[i] = .5f;
                break;
            case kMode:
                param[i] = .17f;
                break;
            case kThru:
                param[i] = .7f;
                break;
            case kOutput:
                param[i] = .5f;
                break;

            default:
                param[i] = 0.f;
                break;
        }
    }
    // default program name
    strcpy (name, "Default");
}

//-----------------------------------------------------------------------------
MidiCCModulator::MidiCCModulator (audioMasterCallback audioMaster)
    : PizMidi (audioMaster, kNumPrograms, kNumParams)
{
    programs = new MidiCCModulatorProgram[numPrograms];

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

    srand ((unsigned int) time (NULL));
    init();
}

//-----------------------------------------------------------------------------------------
MidiCCModulator::~MidiCCModulator()
{
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void MidiCCModulator::setProgram (VstInt32 program)
{
    MidiCCModulatorProgram* ap = &programs[program];

    curProgram = program;
    for (int i = 0; i < kNumParams; i++)
        setParameter (i, ap->param[i]);
}

//------------------------------------------------------------------------
void MidiCCModulator::setProgramName (char* name)
{
    vst_strncpy (programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiCCModulator::getProgramName (char* name)
{
    strcpy (name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool MidiCCModulator::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy (text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiCCModulator::resume()
{
    lastmod = -1;
    lastout = -1;
    lastin = -1;
    AudioEffectX::resume();
}

//-----------------------------------------------------------------------------------------
void MidiCCModulator::setParameter (VstInt32 index, float value)
{
    MidiCCModulatorProgram* ap = &programs[curProgram];
    param[index] = ap->param[index] = value;
    switch (index)
    {
        case kPower:
            on = param[index] >= 0.33f;
            modnotes = param[index] >= 0.67f;
            updateDisplay();
            break;
        case kMode:
            if (param[index] == 0.0f)
                mode = bNOT;
            else if (param[index] < 0.04f)
                mode = uniadd;
            else if (param[index] < 0.08f)
                mode = average;
            else if (param[index] < 0.12f)
                mode = wrap;
            else if (param[index] < 0.16f)
                mode = sub;
            else if (param[index] < 0.20f)
                mode = biadd;
            else if (param[index] < 0.24f)
                mode = mult;
            else if (param[index] < 0.28f)
                mode = comp;
            else if (param[index] < 0.32f)
                mode = invcomp;
            else if (param[index] < 0.36f)
                mode = bite;
            else if (param[index] < 0.40f)
                mode = invbite;
            else if (param[index] < 0.44f)
                mode = gate;
            else if (param[index] < 0.48f)
                mode = block;
            else if (param[index] < 0.52f)
                mode = AND;
            else if (param[index] < 0.56f)
                mode = NAND;
            else if (param[index] < 0.60f)
                mode = OR;
            else if (param[index] < 0.64f)
                mode = NOR;
            else if (param[index] < 0.68f)
                mode = XOR;
            else if (param[index] < 0.72f)
                mode = XNOR;
            else if (param[index] < 0.76f)
                mode = equal;
            else if (param[index] < 0.80f)
                mode = bOR;
            else if (param[index] < 0.84f)
                mode = bAND;
            else if (param[index] < 0.88f)
                mode = bXOR;
            else if (param[index] < 0.92f)
                mode = lshift;
            else if (param[index] < 0.96f)
                mode = rshift;
            else if (param[index] < 1.00f)
                mode = random;
            else
                mode = random;
            break;
        case kThru:
            thru = param[index] >= 0.5f;
            break;
        case kInCC:
            lastin = -1;
            break;
        case kOutCC:
            lastout = -1;
            break;
        case kModCC:
            modcc = FLOAT_TO_MIDI_X (param[index]);
            lastmod = -1;
            break;
        case kSlider:
            if (modcc == -1)
            {
                if (lastmod != FLOAT_TO_MIDI (param[index]))
                {
                    lastmod = FLOAT_TO_MIDI (param[index]);
                    slidermoved = true;
                }
            }
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
float MidiCCModulator::getParameter (VstInt32 index)
{
    return param[index];
}

//-----------------------------------------------------------------------------------------
void MidiCCModulator::getParameterName (VstInt32 index, char* label)
{
    switch (index)
    {
        case kPower:
            strcpy (label, "Mode");
            break;
        case kInCC:
            strcpy (label, "Input");
            break;
        case kModCC:
            strcpy (label, "Modulator");
            break;
        case kSlider:
            strcpy (label, "Manual");
            break;
        case kOutCC:
            strcpy (label, "Output");
            break;
        case kMode:
            strcpy (label, "ModType");
            break;
        case kAmount:
            strcpy (label, "Amt/Thresh");
            break;
        case kOutput:
            strcpy (label, "OutLevel");
            break;
        case kChannel:
            strcpy (label, "Channel");
            break;
        case kThru:
            strcpy (label, "Thru");
            break;
        default:
            sprintf (label, "param %d", index);
    }
}

//-----------------------------------------------------------------------------------------
void MidiCCModulator::getParameterDisplay (VstInt32 index, char* text)
{
    signed char cc = FLOAT_TO_MIDI_X (param[index]);

    switch (index)
    {
        case kPower:
            if (param[index] < 0.33f)
                strcpy (text, "Off");
            else if (param[index] < 0.67f)
                strcpy (text, "CC -> CC");
            else
                strcpy (text, "CC -> Velocity");
            break;
        case kInCC:
            if (! modnotes)
            {
                if (cc == -1)
                    strcpy (text, "Any CC");
                else
                    sprintf (text, "CC %d", cc);
            }
            else
            {
                if (cc == -1)
                    strcpy (text, "Any Note");
                else
                    sprintf (text, "Note %d (%s)", cc, getNoteName (cc, bottomOctave));
            }
            break;
        case kModCC:
            if (cc == -1)
                strcpy (text, "Manual");
            else
                sprintf (text, "CC %d", cc);
            break;
        case kOutCC:
            if (cc == -1 || modnotes)
                strcpy (text, "Same as Input");
            else
            {
                sprintf (text, "CC %d", cc);
            }
            break;
        case kAmount:
            sprintf (text, "%d", FLOAT_TO_MIDI (param[index]));
            break;
        case kSlider:
            sprintf (text, "%d", FLOAT_TO_MIDI (param[index]));
            break;
        case kChannel:
            if (FLOAT_TO_CHANNEL (param[index]) == -1)
                strcpy (text, "Any");
            else
                sprintf (text, "%d", FLOAT_TO_CHANNEL (param[index]) + 1);
            break;
        case kMode:
            switch (mode)
            {
                case bNOT:
                    strcpy (text, "Invert");
                    break;
                case uniadd:
                    strcpy (text, "Add");
                    break;
                case wrap:
                    strcpy (text, "WrapAdd");
                    break;
                case sub:
                    strcpy (text, "Subtract");
                    break;
                case biadd:
                    strcpy (text, "Add/Subtract");
                    break;
                case mult:
                    strcpy (text, "Multiply");
                    break;
                case comp:
                    strcpy (text, "Compare");
                    break;
                case invcomp:
                    strcpy (text, "Limit");
                    break;
                case gate:
                    strcpy (text, "Gate");
                    break;
                case block:
                    strcpy (text, "Block");
                    break;
                case AND:
                    strcpy (text, "AND");
                    break;
                case NAND:
                    strcpy (text, "NAND");
                    break;
                case OR:
                    strcpy (text, "OR");
                    break;
                case NOR:
                    strcpy (text, "NOR");
                    break;
                case XOR:
                    strcpy (text, "XOR");
                    break;
                case XNOR:
                    strcpy (text, "XNOR");
                    break;
                case bOR:
                    strcpy (text, "bitwise OR");
                    break;
                case bAND:
                    strcpy (text, "bitwise AND");
                    break;
                case bXOR:
                    strcpy (text, "bitwise XOR");
                    break;
                case lshift:
                    strcpy (text, "<<");
                    break;
                case rshift:
                    strcpy (text, ">>");
                    break;
                case bite:
                    strcpy (text, "Bite");
                    break;
                case invbite:
                    strcpy (text, "Mangle");
                    break;
                case equal:
                    strcpy (text, "Match");
                    break;
                case average:
                    strcpy (text, "Average");
                    break;
                case random:
                    strcpy (text, "Randomize");
                    break;
                default:
                    strcpy (text, "????");
                    break;
            }
            break;
        case kThru:
            if (param[index] < 0.5f)
                strcpy (text, "Off");
            else if (param[index] < 1.f)
                strcpy (text, "Block ModCC");
            else
                strcpy (text, "All Thru");
            break;
        case kOutput:
            sprintf (text, "%d%%", roundToInt (param[index] * 200.f));
            break;
        default:
            sprintf (text, "%f", param[index]);
    }
}

//-----------------------------------------------------------------------------------------
bool MidiCCModulator::init (void)
{
    lastmod = -1;
    lastout = -1;
    lastin = -1;
    playing = false;
    wasplaying = false;
    slidermoved = false;
    modnotes = param[kPower] >= 0.67f;

    return PizMidi::init();
}

void MidiCCModulator::preProcess()
{
    VstTimeInfo* timeInfo = NULL;
    timeInfo = getTimeInfo (0xffff);

    playing = false;
    if (timeInfo)
    {
        if (kVstTransportPlaying & timeInfo->flags)
            playing = true;
        else
            playing = false;
    }
    else
        playing = true;

    _cleanMidiOutBuffers();
}

void MidiCCModulator::processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    signed char incc = FLOAT_TO_MIDI_X (param[kInCC]);
    signed char outcc = FLOAT_TO_MIDI_X (param[kOutCC]);
    signed char ch = FLOAT_TO_CHANNEL (param[kChannel]);
    if (slidermoved)
    {
        slidermoved = false;
        if (outcc == -1)
            outcc = incc;
        if (outcc != -1 && ch != -1 && ! modnotes)
        {
            if (lastin == -1)
                lastin = 0;
            VstInt32 o = modulate (lastin, lastmod);
            o = roundToInt (2.f * param[kOutput] * (float) o);
            if (o > 127)
                o = 127;
            else if (o < 0)
                o = 0;
            if (lastout != o)
            {
                VstMidiEvent manual;
                manual.midiData[0] = MIDI_CONTROLCHANGE | ch;
                manual.midiData[1] = outcc;
                manual.midiData[2] = o;
                manual.deltaFrames = 0;
                outputs[0].push_back (manual);
                lastout = manual.midiData[2];
            }
        }
    }

    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        unsigned char status = tomod.midiData[0] & 0xf0; // scraping  channel
        unsigned char channel = tomod.midiData[0] & 0x0f; // isolating channel
        unsigned char data1 = tomod.midiData[1] & 0x7f;
        unsigned char data2 = tomod.midiData[2] & 0x7f;

        //reset these for every event
        bool discard = ! thru;
        incc = FLOAT_TO_MIDI_X (param[kInCC]);
        outcc = FLOAT_TO_MIDI_X (param[kOutCC]);
        ch = FLOAT_TO_CHANNEL (param[kChannel]);
        if (status == MIDI_NOTEON && data2 == 0)
            status = MIDI_NOTEOFF;

        if (ch == -1)
            ch = channel; //any channel
        if (on && channel == ch)
        {
            if (status == MIDI_CONTROLCHANGE)
            {
                if (incc == -1)
                    incc = data1; //any cc
                if (outcc == -1)
                    outcc = incc; //same as input cc
                if (data1 == modcc)
                {
                    lastmod = data2;
                    discard = param[kThru] < 1.f; //block it unless Thru=="All"
                    if (! modnotes)
                    {
                        if (incc == modcc)
                        {
                            //ignore
                        }
                        else if (lastin != -1 && mode != bNOT)
                        {
                            VstInt32 o = modulate (lastin, lastmod);
                            o = roundToInt (2.f * param[kOutput] * (float) o);
                            if (o > 127)
                                o = 127;
                            else if (o < 0)
                                o = 0;
                            tomod.midiData[2] = o;
                            if (lastout == tomod.midiData[2])
                                discard = true;
                            lastout = tomod.midiData[2];
                            tomod.midiData[1] = outcc;
                            tomod.midiData[0] = status | ch;
                        }
                    }
                }
                else if (data1 == incc && ! modnotes)
                {
                    discard = false; //when thru is off, only outcc is sent
                    lastin = data2;
                    if (lastmod != -1 || mode == bNOT)
                    {
                        VstInt32 o = modulate (lastin, lastmod);
                        o = roundToInt (2.f * param[kOutput] * (float) o);
                        if (o > 127)
                            o = 127;
                        else if (o < 0)
                            o = 0;
                        tomod.midiData[2] = o;
                        if (lastout == tomod.midiData[2])
                            discard = true;
                    }
                    lastout = tomod.midiData[2];
                    tomod.midiData[1] = outcc;
                    tomod.midiData[0] = status | ch;
                }
            }
            else if (status == MIDI_NOTEON)
            {
                if (modnotes)
                {
                    if (incc == -1)
                        incc = data1; //any note
                    if (data1 == incc)
                    {
                        //modulate velocity
                        discard = false; //when thru is off, only outcc is sent
                        lastin = data2;
                        if (lastmod != -1 || mode == bNOT)
                        { //bNOT is unary, so we don't need a real mod input
                            VstInt32 o = modulate (lastin, lastmod, true);
                            if (o == 0)
                                discard = true;
                            else
                            {
                                o = roundToInt (2.f * param[kOutput] * (float) o);
                                if (o > 127)
                                    o = 127;
                                else if (o < 1)
                                    o = 1;
                                tomod.midiData[2] = o;
                            }
                        }
                        lastout = tomod.midiData[2];
                        tomod.midiData[1] = data1;
                        tomod.midiData[0] = MIDI_NOTEON | ch;
                    }
                }
            }
            else if (status == MIDI_NOTEOFF)
            {
                if (modnotes)
                {
                    if (incc == -1)
                        incc = data1; //any note
                    if (outcc == -1)
                        outcc = incc; //same as input note
                    if (data1 == incc)
                    {
                        //modulate velocity
                        discard = false; //when thru is off, only outcc is sent
                    }
                }
            }
        }

        if (! discard)
            outputs[0].push_back (tomod);
    }
    if (wasplaying && ! playing)
    { //just stopped
        lastmod = -1;
        lastout = -1;
        lastin = -1;
    }
    wasplaying = playing;
}

VstInt32 MidiCCModulator::modulate (char input, char mod, bool notes)
{
    VstInt32 output = input;
    char threshold = FLOAT_TO_MIDI (param[kAmount]);
    switch (mode)
    {
        case uniadd:
            output = roundToInt ((float) input + (param[kAmount] * (float) mod));
            if (output <= 0)
            {
                if (notes)
                    output = 1;
                else
                    output = 0;
            }
            break;

        case biadd:
            output = roundToInt ((float) input + (param[kAmount] * 2.f * (float) (mod - 64)));
            if (output <= 0)
            {
                if (notes)
                    output = 1;
                else
                    output = 0;
            }
            break;

        case mult:
            output = FLOAT_TO_MIDI ((MIDI_TO_FLOAT (input)) * (2.f * param[kAmount] * MIDI_TO_FLOAT (mod)));
            if (output <= 0)
            {
                if (notes)
                    output = 1;
                else
                    output = 0;
            }
            break;

        case sub:
            output = roundToInt ((float) input - (param[kAmount] * (float) (mod)));
            if (output <= 0)
            {
                if (notes)
                    output = 1;
                else
                    output = 0;
            }
            break;

        case comp:
            if (input < mod)
                output = mod;
            else
                output = input;
            if (output <= 0)
            {
                if (notes)
                    output = 1;
                else
                    output = 0;
            }
            break;

        case bite:
            if (input > mod)
                output = roundToInt ((float) input - (param[kAmount] * (float) (mod)));
            else
                output = input;
            if (output <= 0)
            {
                if (notes)
                    output = 1;
                else
                    output = 0;
            }
            break;

        case invbite:
            if (input < mod)
                output = roundToInt ((float) input - (param[kAmount] * 2.f * (float) (mod - 64)));
            else
                output = input;
            if (output <= 0)
            {
                if (notes)
                    output = 1;
                else
                    output = 0;
            }
            break;

        case invcomp:
            if (input > mod)
                output = mod;
            else
                output = input;
            if (output <= 0)
            {
                if (notes)
                    output = 1;
                else
                    output = 0;
            }
            break;

        case gate:
            if (mod >= threshold)
                output = input;
            else
                output = 0;
            break;

        case block:
            if (mod >= threshold)
                output = 0;
            else
                output = input;
            break;

        case AND:
            if (input >= threshold && mod >= threshold)
                output = 127;
            else
                output = 0;
            break;

        case OR:
            if (input >= threshold || mod >= threshold)
                output = 127;
            else
                output = 0;
            break;

        case XOR:
            if (input >= threshold != mod >= threshold)
                output = 127;
            else
                output = 0;
            break;

        case NAND:
            if (input >= threshold && mod >= threshold)
                output = 0;
            else
                output = 127;
            break;

        case NOR:
            if (input >= threshold || mod >= threshold)
                output = 0;
            else
                output = 127;
            break;

        case XNOR:
            if (input >= threshold == mod >= threshold)
                output = 127;
            else
                output = 0;
            break;

        case bNOT:
            output = (~input) & 0x7f;
            break;

        case bOR:
            output = (input | mod) & 0x7f;
            break;

        case bAND:
            output = (input & mod) & 0x7f;
            break;

        case bXOR:
            output = (input ^ mod) & 0x7f;
            break;

        case lshift:
            output = (input << mod) & 0x7f;
            break;

        case rshift:
            output = (input >> mod) & 0x7f;
            break;

        case equal:
            output = (input == mod) * 127;
            break;

        case average:
            output = roundToInt (param[kAmount] * ((float) input + (float) mod) * 0.5f + (1.f - param[kAmount]) * (float) input);
            break;

        case wrap:
            output = roundToInt ((float) input + param[kAmount] * (float) mod) & 0x7f;
            break;

        case random:
            output = roundToInt ((float) input + param[kAmount] * (float) mod * (float) (rand() % 200 - 100) * 0.01f) & 0x7f;
            break;

        default:
            output = input;
            break;
    }
    return output;
}
