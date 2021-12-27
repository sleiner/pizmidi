juce_add_plugin(
  pizmidi_BigClock
  COMPANY_NAME "Insert Piz Here"
  DESCRIPTION "Time Counter"
  IS_SYNTH TRUE
  PLUGIN_CODE "9wMw"
  PLUGIN_MANUFACTURER_CODE "IPHz"
  PRODUCT_NAME "Big Clock"
  FORMATS VST3
  VERSION "1.3.0")

target_sources(
  pizmidi_BigClock
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/BigClock.h"
          "${CMAKE_CURRENT_LIST_DIR}/BigClockEditor.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/BigClockEditor.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/BigClock.cpp")

target_link_libraries(
  pizmidi_BigClock PUBLIC juce::juce_events juce::juce_gui_basics
                          juce::juce_gui_extra pizjuce_common)
