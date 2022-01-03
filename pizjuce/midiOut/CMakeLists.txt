piz_add_plugin(
  piz_midiOut
  PRODUCT_NAME "midiOut"
  DESCRIPTION "Sends MIDI out"
  PLUGIN_CODE "miio"
  IS_MIDI_EFFECT TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE
  VERSION "1.3")

target_sources(
  piz_midiOut
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/src/midiOut.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/midiOutEditor.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiPad.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/src/midiOut.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/midiOutEditor.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiPad.cpp")

target_link_libraries(
  piz_midiOut PUBLIC juce::juce_audio_devices juce::juce_events
                     juce::juce_gui_basics)
