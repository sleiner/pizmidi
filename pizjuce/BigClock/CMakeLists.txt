piz_add_plugin(
  piz_BigClock
  DESCRIPTION "Time Counter"
  IS_SYNTH TRUE
  PLUGIN_CODE "9wMw"
  PRODUCT_NAME "Big Clock"
  VERSION "1.3.0")

target_sources(
  piz_BigClock
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/BigClock.h"
          "${CMAKE_CURRENT_LIST_DIR}/BigClockEditor.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/BigClockEditor.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/BigClock.cpp")

target_link_libraries(
  piz_BigClock PUBLIC juce::juce_events juce::juce_gui_basics
                      juce::juce_gui_extra)
