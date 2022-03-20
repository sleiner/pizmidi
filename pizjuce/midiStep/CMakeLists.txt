piz_add_plugin(
  piz_midiStep
  PRODUCT_NAME "midiStep"
  DESCRIPTION "midiStep"
  PLUGIN_CODE "pizS"
  HAS_GUI
  IS_SYNTH TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE)

target_sources(
  piz_midiStep
  INTERFACE # Headers
            "${CMAKE_CURRENT_LIST_DIR}/MidiLoop.h"
            "${CMAKE_CURRENT_LIST_DIR}/PianoRoll.h"
            "${CMAKE_CURRENT_LIST_DIR}/step.h"
            "${CMAKE_CURRENT_LIST_DIR}/stepgui.h"
            # Sources
            "${CMAKE_CURRENT_LIST_DIR}/PianoRoll.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/step.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/stepgui.cpp")

target_link_libraries(
  piz_midiStep INTERFACE juce::juce_audio_processors juce::juce_events
                         juce::juce_gui_basics)
