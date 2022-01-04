piz_add_plugin(
  piz_midiPBCurve
  PRODUCT_NAME "midiPBCurve"
  DESCRIPTION "midiPBCurve"
  PLUGIN_CODE "PBcv"
  IS_SYNTH TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE
  VERSION "1.2")

target_sources(
  piz_midiPBCurve
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/src/curve.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/curvegui.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiEnvelope.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/src/curve.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/curvegui.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiEnvelope.cpp")

target_link_libraries(
  piz_midiPBCurve PUBLIC juce::juce_audio_processors juce::juce_events
                         juce::juce_gui_basics)
