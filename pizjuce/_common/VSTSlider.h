#ifndef Piz_VSTSlider_h
#define Piz_VSTSlider_h

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

class VSTSlider : public juce::Slider
{
public:
    VSTSlider(juce::String name);
    ~VSTSlider() override;
    juce::String getTextFromValue(double value) override;
    void setOwner(juce::AudioProcessor*, int);
    float mapToVSTRange();
    void setVSTSlider(float x);

    void setVSTSlider();
    void setVSTParam();
    void setIndex(int index);

private:
    juce::AudioProcessor* ownerPlugin;
    int vstIndex;
};
#endif
