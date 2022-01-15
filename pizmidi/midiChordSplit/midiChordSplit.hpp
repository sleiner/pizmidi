/*-----------------------------------------------------------------------------
midiChordSplit
by Reuben Vinal
-----------------------------------------------------------------------------*/

#ifndef __MIDICHORDSPLIT_H
#define __MIDICHORDSPLIT_H

#include "../common/PizMidi.h"

enum
{
    kPower,
    kInCh,
    kSteal,
    kCh1Power,
    kCh1Low,
    kCh1High,
    kCh1Poly,
    kCh2Power,
    kCh2Low,
    kCh2High,
    kCh2Poly,
    kCh3Power,
    kCh3Low,
    kCh3High,
    kCh3Poly,
    kCh4Power,
    kCh4Low,
    kCh4High,
    kCh4Poly,
    kCh5Power,
    kCh5Low,
    kCh5High,
    kCh5Poly,
    kCh6Power,
    kCh6Low,
    kCh6High,
    kCh6Poly,
    kCh7Power,
    kCh7Low,
    kCh7High,
    kCh7Poly,
    kCh8Power,
    kCh8Low,
    kCh8High,
    kCh8Poly,
    kCh9Power,
    kCh9Low,
    kCh9High,
    kCh9Poly,
    kCh10Power,
    kCh10Low,
    kCh10High,
    kCh10Poly,
    kCh11Power,
    kCh11Low,
    kCh11High,
    kCh11Poly,
    kCh12Power,
    kCh12Low,
    kCh12High,
    kCh12Poly,
    kCh13Power,
    kCh13Low,
    kCh13High,
    kCh13Poly,
    kCh14Power,
    kCh14Low,
    kCh14High,
    kCh14Poly,
    kCh15Power,
    kCh15Low,
    kCh15High,
    kCh15Poly,
    kCh16Power,
    kCh16Low,
    kCh16High,
    kCh16Poly,
    kNumParams,
    kNumPrograms = 16,
    maxPoly = 64,
    queueSize = 256
};

struct v
{
    unsigned long oldness;
    unsigned char inch;
    char chan;
    signed short note;
    signed short vel;
    bool on;
    bool sustained;
    void reset()
    {
        oldness = 0;
        inch = 0;
        chan = 0;
        note = 0;
        vel = 0;
        on = false;
        sustained = false;
    }
    v() { this->reset(); }
};

class MidiChordSplitProgram
{
    friend class MidiChordSplit;

public:
    MidiChordSplitProgram();
    ~MidiChordSplitProgram() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen];
};

class MidiChordSplit : public PizMidi
{
public:
    MidiChordSplit (audioMasterCallback audioMaster);
    ~MidiChordSplit();

    virtual VstInt32 processEvents (VstEvents* events);

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

protected:
    float param[kNumParams];

    bool init();
    virtual void preProcess();
    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    int voices[16]; //voices per channel
    v Voice[16][maxPoly]; //
    v Queue[queueSize];
    unsigned int oldness;
    v copyVoice (v);
    bool sustain;
    bool playing;
    bool wasplaying;
    int ntime[128][16];
    bool held[16][128];
    bool sounding[16][128];
    int lastNote[16];
    //std::vector<VstMidiEvent> heldNotes;
    int centerNote;
    enum stealmodes
    {
        oldest,
        newest,
        quietest,
        lowest,
        highest,
        centered,
        random,
        numStealModes
    };
    char stealmode;
    struct midiSort
    {
        bool operator() (const VstMidiEvent& first, const VstMidiEvent& second);
        int mode;
        int centerNote;
        int priorityNote;
        int onvelocity[16][128];
        midiSort()
        {
            memset (onvelocity, 0, sizeof (onvelocity));
            centerNote = 60;
            mode = 0;
            priorityNote = -1;
        }
    };
    midiSort sorter;
    void findCenterNote();
    int poly[16];
    int lastoutch;

    MidiChordSplitProgram* programs;
};

#endif
