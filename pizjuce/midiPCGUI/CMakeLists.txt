piz_add_plugin(
  piz_midiPCGUI
  PRODUCT_NAME "midiPCGUI"
  DESCRIPTION "MIDI Program Change plugin (with GUI)"
  PLUGIN_CODE "mpcg"
  IS_SYNTH TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE)

target_sources(
  piz_midiPCGUI
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/midiPCGUI.h"
          "${CMAKE_CURRENT_LIST_DIR}/midiPCGUIEditor.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/midiPCGUI.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/midiPCGUIEditor.cpp")

target_link_libraries(
  piz_midiPCGUI PRIVATE juce::juce_audio_processors juce::juce_core
                        juce::juce_events juce::juce_gui_basics piz_common_gui)
