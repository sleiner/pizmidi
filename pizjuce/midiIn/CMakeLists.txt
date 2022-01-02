piz_add_plugin(
  piz_midiIn
  PRODUCT_NAME "midiIn"
  DESCRIPTION "Receives external MIDI"
  PLUGIN_CODE "miii"
  IS_MIDI_EFFECT TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE
  VERSION "1.2.0")

target_sources(
  piz_midiIn
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/src/DrawablePad.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/midiIn.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/midiInEditor.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiPad.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/src/DrawablePad.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/midiIn.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/midiInEditor.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiPad.cpp")

target_link_libraries(piz_midiIn PUBLIC juce::juce_audio_devices
                                        juce::juce_events)
