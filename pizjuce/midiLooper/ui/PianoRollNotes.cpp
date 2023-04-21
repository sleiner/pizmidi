
#include "PianoRollNotes.h"

#include "PianoRoll.h"

using juce::jmax;

PianoRollNotes::PianoRollNotes()
{
    setBufferedToImage(true);
}

PianoRollNotes::~PianoRollNotes() = default;

void PianoRollNotes::paint(juce::Graphics& g)
{
    const PianoRoll* roll = (PianoRoll*) getParentComponent();
    if (roll->barSize > 0)
    {
        if (roll->sequence)
        {
            for (int i = 0; i < roll->sequence->getNumEvents(); i++)
            {
                if (roll->sequence->getEventPointer(i)->message.isNoteOn() /* && (i-9999!=hoveringNote)*/)
                {
                    auto noteLength = (float) (jmax(1.0, (roll->sequence->getEventTime(roll->sequence->getIndexOfMatchingKeyUp(i)) - roll->sequence->getEventTime(i))) * (double) getWidth() / roll->seqLengthInPpq);
                    //if (i==sequence->indexOfLastNoteOn
                    //	|| abs(sequence->getEventTime(i)-sequence->getEventTime(sequence->indexOfLastNoteOn))<sequence->chordTolerance)
                    //	g.setColour(Colours::blue);
                    //else

                    const auto originalNoteRect = juce::Rectangle<float>(std::round((float) getWidth() * (float) (roll->sequence->getEventTime(i) / roll->seqLengthInPpq)),
                                                                         std::round((float) getHeight() - (float) (roll->sequence->getEventPointer(i)->message.getNoteNumber()) * roll->yinc - roll->yinc),
                                                                         std::round(noteLength),
                                                                         std::round(roll->yinc));

                    if (roll->selectedNotes.contains(roll->sequence->getEventPointer(i)))
                    {
                        //outline of original note position
                        g.setColour(juce::Colours::blue);
                        g.drawRect(originalNoteRect);

                        //dragging note position
                        const double newTime = (float) ((roll->sequence->getEventTime(i) + roll->draggingNoteTimeDelta));
                        const float newNote  = (float) (roll->sequence->getEventPointer(i)->message.getNoteNumber() + roll->draggingNoteTransposition);

                        const auto rect = juce::Rectangle<float>(std::round((float) getWidth() * (float) (newTime / roll->seqLengthInPpq)),
                                                                 std::round((float) getHeight() - newNote * roll->yinc - roll->yinc),
                                                                 std::round(noteLength),
                                                                 std::round(roll->yinc));
                        g.setColour(juce::Colours::red);
                        g.fillRect(rect);

                        g.setColour(juce::Colours::darkgoldenrod.withAlpha(roll->sequence->getEventPointer(i)->message.getFloatVelocity()));
                        g.fillRect(rect.reduced(1.0f));
                    }
                    else
                    {
                        g.setColour(juce::Colour::fromString("ff222222"));
                        g.fillRect(originalNoteRect);

                        g.setColour(juce::Colours::goldenrod.withAlpha(roll->sequence->getEventPointer(i)->message.getFloatVelocity()));
                        g.fillRect(originalNoteRect.reduced(1.0f));
                    }
                }
            }
        }
    }
}
