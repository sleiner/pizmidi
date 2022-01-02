piz_add_plugin(
  piz_midiCurve
  PRODUCT_NAME "midiCurve"
  DESCRIPTION "midiCurve"
  PLUGIN_CODE "Curv"
  IS_MIDI_EFFECT TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE
  VERSION "1.3.0")

target_sources(
  piz_midiCurve
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/src/curve.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/curvegui.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiEnvelope.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/src/curve.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/curvegui.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiEnvelope.cpp")

target_link_libraries(piz_midiCurve PUBLIC juce::juce_events
                                           juce::juce_gui_basics)
