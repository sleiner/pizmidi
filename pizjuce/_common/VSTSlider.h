#ifndef Piz_VSTSlider_h
#define Piz_VSTSlider_h

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_core/juce_core.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

class VSTSlider : public Slider
{
public:
    VSTSlider (String name);
    ~VSTSlider() override;
    String getTextFromValue (double value) override;
    void setOwner (AudioProcessor*, int);
    float mapToVSTRange();
    void setVSTSlider (float x);

    void setVSTSlider();
    void setVSTParam();
    void setIndex (int index);

private:
    AudioProcessor* ownerPlugin;
    int vstIndex;
};
#endif
