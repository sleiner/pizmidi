#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_events/juce_events.h"
#include "juce_gui_basics/juce_gui_basics.h"

namespace juce { class AudioProcessor; }
class MidiMorph;
namespace juce { class ListBox; }
class ControllerList;
class MorphPaneModel;
namespace juce { class ResizableBorderComponent; }
class MidiMorphPlugInInterface;
class CursorGUI;
class MorphPane;
class D3CKLook;

class MidiMorphGUI : public juce::ChangeListener,
                     public juce::ApplicationCommandTarget,
                     public juce::AudioProcessorEditor
{
  public:
    MidiMorphGUI(AudioProcessor* const ownerPlugIn);

    ~MidiMorphGUI();


  private:
    MidiMorph* core;

    juce::ListBox* controllerList;

    AudioProcessor* ownerFilter;


  public:
    ControllerList* controllerListModel;


  private:
    MorphPaneModel* morphPaneModel;

    juce::ResizableBorderComponent* resizer;


  public:
    //ApplicationCommandTarget *  getNextCommandTarget
    ApplicationCommandTarget* getNextCommandTarget();

    void resized();

    //void  getAllCommands (Array< CommandID > &commands)=0
    void getAllCommands(Array<CommandID>& commands);

    //void  getCommandInfo (const CommandID commandID, ApplicationCommandInfo &result)=
    void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result);

    //perform ()=0
    bool perform(const InvocationInfo& info);

    void changeListenerCallback(ChangeBroadcaster* objectThatHasChanged) override;

    void paint(Graphics & g);

juce_UseDebuggingNewOperator
  private:
    MorphPane* morphPane;

};
