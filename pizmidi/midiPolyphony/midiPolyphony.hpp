/*-----------------------------------------------------------------------------
midiPolyphony
by Reuben Vinal
-----------------------------------------------------------------------------*/
#ifndef __MIDIPOLYPHONY_H
#define __MIDIPOLYPHONY_H

#include "../common/PizMidi.h"

enum
{
    kPower,
    kLowCh,
    kHighCh,
    kMaxPoly,
    kSteal,
    kRetrig,
    kInCh,
    kSustain,
    kMode,
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

class MidiPolyphonyProgram
{
    friend class MidiPolyphony;

public:
    MidiPolyphonyProgram();
    ~MidiPolyphonyProgram() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen];
};

class MidiPolyphony : public PizMidi
{
public:
    MidiPolyphony (audioMasterCallback audioMaster);
    ~MidiPolyphony();

    virtual VstInt32 processEvents (VstEvents* ev);

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

    bool init (void);
    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    virtual void preProcess (void);

    int voices[MIDI_MAX_CHANNELS]; //voices per channel
    v Voice[MIDI_MAX_CHANNELS][maxPoly]; //
    v Queue[queueSize];
    //unsigned char queued;
    unsigned int oldness;
    v copyVoice (v);
    bool sustain;
    bool playing;
    bool wasplaying;
    int ntime[128][16];
    bool held[16][128];
    bool sounding[16][128];
    std::vector<VstMidiEvent> heldNotes;
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
    int poly;
    int lastoutch;

    MidiPolyphonyProgram* programs;
};

#endif
