#include "PlayBar.h"

#include "PianoRoll.h"

PlayBar::PlayBar(PianoRoll* pianoRoll)
    : pianoRoll(pianoRoll)
{
}

PlayBar::~PlayBar() = default;

void PlayBar::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0x0));
    //if (sequence->isRecording) {
    //	g.setColour(Colours::red);
    //	g.drawVerticalLine((int)((float)sequence->recTime*(float)getWidth()/seqLengthInPpq),0.f,(float)getHeight());
    //}
    //else {
    g.setColour(juce::Colours::green);
    g.drawVerticalLine((int) pianoRoll->ppqToPixelsWithOffset(pianoRoll->playTime), 0.f, (float) getHeight());
    //}
}
