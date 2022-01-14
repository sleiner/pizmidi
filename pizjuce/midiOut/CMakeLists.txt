piz_add_plugin(
  piz_midiOut
  PRODUCT_NAME "midiOut"
  DESCRIPTION "Sends MIDI out"
  PLUGIN_CODE "miio"
  IS_MIDI_EFFECT TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE)

target_sources(
  piz_midiOut
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/midiOut.h"
          "${CMAKE_CURRENT_LIST_DIR}/midiOutEditor.h"
          "${CMAKE_CURRENT_LIST_DIR}/MidiPad.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/midiOut.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/midiOutEditor.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/MidiPad.cpp")

target_link_libraries(
  piz_midiOut PRIVATE juce::juce_audio_devices juce::juce_events
                      juce::juce_gui_basics)
