piz_add_plugin(
  piz_midiStep
  PRODUCT_NAME "midiStep"
  DESCRIPTION "midiStep"
  PLUGIN_CODE "pizS"
  IS_SYNTH TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE
  VERSION "1.2")

target_sources(
  piz_midiStep
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiLoop.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/PianoRoll.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/step.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/stepgui.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/src/PianoRoll.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/step.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/stepgui.cpp")

target_link_libraries(
  piz_midiStep PUBLIC juce::juce_audio_processors juce::juce_events
                      juce::juce_gui_basics)
