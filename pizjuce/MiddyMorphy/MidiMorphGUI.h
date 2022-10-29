#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_events/juce_events.h>
#include <juce_gui_basics/juce_gui_basics.h>

class MidiMorph;
class ControllerList;
class MorphPaneModel;
class MidiMorphPlugInInterface;
class CursorGUI;
class MorphPane;
class D3CKLook;

class MidiMorphGUI : public juce::ChangeListener,
                     public juce::ApplicationCommandTarget,
                     public juce::AudioProcessorEditor
{
public:
    MidiMorphGUI (juce::AudioProcessor* ownerPlugIn);

    ~MidiMorphGUI() override;

    ControllerList* controllerListModel;

    ApplicationCommandTarget* getNextCommandTarget() override;

    void resized() override;

    void getAllCommands (juce::Array<juce::CommandID>& commands) override;

    void getCommandInfo (juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;

    bool perform (const InvocationInfo& info) override;

    void changeListenerCallback (juce::ChangeBroadcaster* objectThatHasChanged) override;

    void paint (juce::Graphics& g) override;

private:
    MorphPane* morphPane;

private:
    MidiMorph* core;
    MorphPaneModel* morphPaneModel;

    juce::ListBox* controllerList;
    juce::AudioProcessor* ownerFilter;
    juce::ResizableBorderComponent* resizer;

    JUCE_LEAK_DETECTOR (MidiMorphGUI)
};
