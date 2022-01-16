piz_add_plugin(
  piz_midiCurve
  PRODUCT_NAME "midiCurve"
  DESCRIPTION "midiCurve"
  PLUGIN_CODE "Curv"
  IS_MIDI_EFFECT TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE)

target_sources(
  piz_midiCurve
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/curve.h"
          "${CMAKE_CURRENT_LIST_DIR}/curvegui.h"
          "${CMAKE_CURRENT_LIST_DIR}/MidiEnvelope.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/curve.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/curvegui.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/MidiEnvelope.cpp")

target_link_libraries(
  piz_midiCurve PRIVATE juce::juce_events juce::juce_gui_basics piz_common_gui)
