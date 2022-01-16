piz_add_plugin(
  piz_midiPBCurve
  PRODUCT_NAME "midiPBCurve"
  DESCRIPTION "midiPBCurve"
  PLUGIN_CODE "PBcv"
  IS_SYNTH TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE)

target_sources(
  piz_midiPBCurve
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/curve.h"
          "${CMAKE_CURRENT_LIST_DIR}/curvegui.h"
          "${CMAKE_CURRENT_LIST_DIR}/MidiEnvelope.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/curve.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/curvegui.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/MidiEnvelope.cpp")

target_link_libraries(
  piz_midiPBCurve PRIVATE juce::juce_audio_processors juce::juce_events
                          juce::juce_gui_basics piz_common_gui)
