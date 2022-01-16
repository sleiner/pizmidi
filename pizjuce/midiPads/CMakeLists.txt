piz_add_plugin(
  piz_midiPads
  PRODUCT_NAME "midiPads"
  DESCRIPTION "piz midi pads"
  PLUGIN_CODE "m16p"
  HAS_GUI
  IS_SYNTH TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE)

target_sources(
  piz_midiPads
  INTERFACE # Headers
            "${CMAKE_CURRENT_LIST_DIR}/MidiPad.h"
            "${CMAKE_CURRENT_LIST_DIR}/midiPads.h"
            "${CMAKE_CURRENT_LIST_DIR}/midiPadsEditor.h"
            "${CMAKE_CURRENT_LIST_DIR}/PadEditor.h"
            # Sources
            "${CMAKE_CURRENT_LIST_DIR}/MidiPad.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/midiPads.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/midiPadsEditor.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/PadEditor.cpp")

target_link_libraries(
  piz_midiPads
  INTERFACE juce::juce_audio_processors juce::juce_core
            juce::juce_data_structures juce::juce_gui_basics
            juce::juce_gui_extra)
