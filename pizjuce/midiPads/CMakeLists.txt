piz_add_plugin(
  piz_midiPads
  PRODUCT_NAME "midiPads"
  DESCRIPTION "piz midi pads"
  PLUGIN_CODE "m16p"
  IS_SYNTH TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE
  VERSION "1.3")

target_sources(
  piz_midiPads
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiPad.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/midiPads.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/midiPadsEditor.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/PadEditor.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiPad.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/midiPads.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/midiPadsEditor.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/PadEditor.cpp")

target_link_libraries(
  piz_midiPads
  PUBLIC juce::juce_audio_processors juce::juce_core juce::juce_data_structures
         juce::juce_gui_basics juce::juce_gui_extra)
