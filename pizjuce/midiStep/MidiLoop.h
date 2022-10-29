#ifndef PIZ_MIDI_LOOP_HEADER
#define PIZ_MIDI_LOOP_HEADER

#include <juce_audio_basics/juce_audio_basics.h>

enum playmodes
{
    playStep,
    playOneShot,
    playLooped,
    playWithHost,
    playLoopedWithHost,
    numPlayModes
};

class Loop : public juce::MidiMessageSequence
{
public:
    Loop()
        : MidiMessageSequence(),
          currentIndex(0),
          triggerNote(60),
          transpose(0),
          playMode(0),
          isRecording(false),
          isRecArmed(false),
          recChannel(0),
          outChannel(0),
          recTime(0),
          indexOfLastNoteOn(-1),
          velocitySensitivity(1.f),
          chordTolerance(120.0)
    {
        resetNotes();
    }
    Loop(MidiMessageSequence sequence, int triggerNote_, int transpose_ = 0, bool playMode_ = 0)
        : MidiMessageSequence(sequence),
          currentIndex(0),
          triggerNote(triggerNote_),
          transpose(transpose_),
          playMode(playMode_),
          isRecording(false),
          isRecArmed(false),
          recChannel(0),
          outChannel(0),
          recTime(0),
          indexOfLastNoteOn(-1),
          velocitySensitivity(1.f),
          chordTolerance(120.0)
    {
        resetNotes();
    }
    ~Loop(){};

    int currentIndex;
    int transpose;
    int playMode;
    bool isRecording;
    bool isRecArmed;
    int recChannel;
    int outChannel;
    double recTime;
    int indexOfLastNoteOn;
    float velocitySensitivity;
    double chordTolerance;

    void startRecording()
    {
        clear();
        recTime      = 0;
        currentIndex = 0;
        isRecording  = true;
    }

    bool findNextNote()
    {
        if (currentIndex >= getNumEvents())
            currentIndex = 0;
        if (this->getEventPointer(currentIndex)->message.isNoteOn())
        {
            return true;
        }
        ++currentIndex;
        while (currentIndex < getNumEvents())
        {
            if (this->getEventPointer(currentIndex)->message.isNoteOn())
            {
                return true;
            }
            ++currentIndex;
        }
        currentIndex = 0;
        while (currentIndex < getNumEvents())
        {
            if (this->getEventPointer(currentIndex)->message.isNoteOn())
            {
                return true;
            }
            ++currentIndex;
        }
        return false;
    }

    void playAllNotesAtCurrentTime(juce::MidiBuffer& buffer, int sample_number, int velocity)
    {
        if (findNextNote())
        {
            sendCurrentNoteToBuffer(buffer, sample_number, velocity);

            const double time = getCurrentTime();
            ++currentIndex;
            while (abs(getCurrentTime() - time) < chordTolerance)
            {
                if (this->getEventPointer(currentIndex)->message.isNoteOn())
                    sendCurrentNoteToBuffer(buffer, sample_number, velocity);
                ++currentIndex;
            }
        }
    }

    void sendCurrentNoteToBuffer(juce::MidiBuffer& buffer, int sample_number, int velocity)
    {
        juce::MidiMessage m                                = this->getEventPointer(currentIndex)->message;
        playingNote[m.getNoteNumber()][m.getChannel() - 1] = juce::jlimit(0, 127, m.getNoteNumber() + transpose);
        m.setNoteNumber(juce::jlimit(0, 127, m.getNoteNumber() + transpose));
        m.setVelocity((((float) velocity * midiScaler * velocitySensitivity) + (1.f - velocitySensitivity)));
        if (outChannel > 0)
            m.setChannel(outChannel);
        buffer.addEvent(m, sample_number);
        indexOfLastNoteOn = currentIndex;
    }

    bool isNotePlaying(int note, int channel)
    {
        return playingNote[note][channel] != NOT_PLAYING;
    }

    bool isTriggerNote(int note)
    {
        return note == triggerNote;
    }

    void setTriggerNote(int note)
    {
        triggerNote = note;
    }

    void resetNotes()
    {
        for (int n = 0; n < 128; n++)
        {
            for (int ch = 0; ch < 16; ch++)
                playingNote[n][ch] = NOT_PLAYING;
        }
    }

    void sendNoteOffMessagesToBuffer(juce::MidiBuffer& buffer, int sample_number)
    {
        for (int ch = 0; ch < 16; ch++)
        {
            for (int i = 0; i < 128; i++)
            {
                if (isNotePlaying(i, ch))
                {
                    int channel = outChannel > 0 ? outChannel - 1 : ch;
                    buffer.addEvent(juce::MidiMessage(MIDI_NOTEOFF + channel, playingNote[i][ch], 0), sample_number);
                    playingNote[i][ch] = NOT_PLAYING;
                }
            }
        }
    }

    juce::MidiMessage getCurrentMessage()
    {
        if (currentIndex >= this->getNumEvents())
            currentIndex = 0;
        return this->getEventPointer(currentIndex)->message;
    }

    double getCurrentTime()
    {
        return this->getEventTime(currentIndex);
    }

    int getIndexOfNote(int noteNumber, double time)
    {
        for (int i = 0; i < getNumEvents(); i++)
        {
            if (getEventPointer(i)->message.isNoteOn()
                && getEventPointer(i)->message.getNoteNumber() == noteNumber
                && (getEventTime(i) <= time && getTimeOfMatchingKeyUp(i) > time))
            {
                return i;
            }
        }
        return -1;
    }

    void convertTimeBase(short timeBase)
    {
        if (timeBase > 0)
        {
            double factor = 960.0 / (double) timeBase;
            for (int i = 0; i < getNumEvents(); i++)
            {
                getEventPointer(i)->message.setTimeStamp(getEventPointer(i)->message.getTimeStamp() * factor);
            }
        }
        else
        {
            /* SMTPE
			 If it's negative, the upper byte indicates the frames-per-second (but negative), and
			 the lower byte is the number of ticks per frame - see setSmpteTimeFormat().

				@param framesPerSecond      must be 24, 25, 29 or 30
				@param subframeResolution   the sub-second resolution, e.g. 4 (midi time code),
                                    8, 10, 80 (SMPTE bit resolution), or 100. For millisecond
                                    timing, setSmpteTimeFormat (25, 40)
			*/
            int framesPerSecond    = -(timeBase / 0x100);
            int subframeResolution = (timeBase & 0x00ff);
        }
    }

private:
    int triggerNote;
    int playingNote[128][16];
};

#endif
