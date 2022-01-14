#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_events/juce_events.h"
#include "juce_gui_basics/juce_gui_basics.h"

namespace juce
{
    class AudioProcessor;
}
class MidiMorph;
namespace juce
{
    class ListBox;
}
class ControllerList;
class MorphPaneModel;
namespace juce
{
    class ResizableBorderComponent;
}
class MidiMorphPlugInInterface;
class CursorGUI;
class MorphPane;
class D3CKLook;

class MidiMorphGUI : public juce::ChangeListener,
                     public juce::ApplicationCommandTarget,
                     public juce::AudioProcessorEditor
{
public:
    MidiMorphGUI (juce::AudioProcessor* const ownerPlugIn);

    ~MidiMorphGUI() override;

private:
    MidiMorph* core;

    juce::ListBox* controllerList;

    juce::AudioProcessor* ownerFilter;

public:
    ControllerList* controllerListModel;

private:
    MorphPaneModel* morphPaneModel;

    juce::ResizableBorderComponent* resizer;

public:
    //ApplicationCommandTarget *  getNextCommandTarget
    ApplicationCommandTarget* getNextCommandTarget() override;

    void resized() override;

    //void  getAllCommands (Array< CommandID > &commands)=0
    void getAllCommands (juce::Array<juce::CommandID>& commands) override;

    //void  getCommandInfo (const CommandID commandID, ApplicationCommandInfo &result)=
    void getCommandInfo (juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;

    //perform ()=0
    bool perform (const InvocationInfo& info) override;

    void changeListenerCallback (juce::ChangeBroadcaster* objectThatHasChanged) override;

    void paint (juce::Graphics& g) override;

    juce_UseDebuggingNewOperator private : MorphPane* morphPane;
};
