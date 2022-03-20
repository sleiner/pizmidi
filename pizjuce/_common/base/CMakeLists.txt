add_library(piz_common_base INTERFACE)
target_include_directories(piz_common_base
                           INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include")
target_sources(
  piz_common_base
  INTERFACE # Headers
            "${CMAKE_CURRENT_LIST_DIR}/include/piz/Array.h"
            "${CMAKE_CURRENT_LIST_DIR}/include/piz/JArray.h"
            "${CMAKE_CURRENT_LIST_DIR}/include/piz/MidiProcessor.h"
            "${CMAKE_CURRENT_LIST_DIR}/include/piz/PluginState.h"
            # Sources
)
target_link_libraries(
  piz_common_base INTERFACE juce::juce_audio_processors
                            juce::juce_data_structures juce::juce_core)
