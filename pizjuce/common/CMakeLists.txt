add_library(pizjuce_common INTERFACE)
target_include_directories(pizjuce_common INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_sources(
  pizjuce_common
  INTERFACE "${CMAKE_CURRENT_LIST_DIR}/ChordFunctions.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/GuitarNeckComponent.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/Main.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/PizButton.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/VSTSlider.cpp")
target_link_libraries(
  pizjuce_common
  INTERFACE reaper_sdk #
            juce::juce_recommended_config_flags #
            juce::juce_recommended_lto_flags #
            # juce::juce_recommended_warning_flags #
            juce::juce_core
            juce::juce_audio_basics
            juce::juce_audio_devices
            juce::juce_audio_utils
            juce::juce_audio_plugin_client
            juce::juce_audio_processors
            juce::juce_data_structures
            juce::juce_gui_basics)

target_compile_definitions(
  pizjuce_common INTERFACE JUCE_MODAL_LOOPS_PERMITTED=1
                           JUCE_VST3_CAN_REPLACE_VST2=0)
