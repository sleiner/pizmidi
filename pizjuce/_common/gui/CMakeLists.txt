add_library(piz_common_gui INTERFACE)
target_include_directories(piz_common_gui
                           INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include")
target_sources(
  piz_common_gui
  INTERFACE # Headers
            "${CMAKE_CURRENT_LIST_DIR}/include/piz/BankStorage.h"
            "${CMAKE_CURRENT_LIST_DIR}/include/piz/ChannelSlider.h"
            "${CMAKE_CURRENT_LIST_DIR}/include/piz/ChordFunctions.h"
            "${CMAKE_CURRENT_LIST_DIR}/include/piz/ClickableLabel.h"
            "${CMAKE_CURRENT_LIST_DIR}/include/piz/DrawablePad.h"
            "${CMAKE_CURRENT_LIST_DIR}/include/piz/MIDI.h"
            "${CMAKE_CURRENT_LIST_DIR}/include/piz/NoteSlider.h"
            "${CMAKE_CURRENT_LIST_DIR}/include/piz/PizAudioProcessor.h"
            "${CMAKE_CURRENT_LIST_DIR}/include/piz/PizButton.h"
            "${CMAKE_CURRENT_LIST_DIR}/include/piz/VSTSlider.h"
            "${CMAKE_CURRENT_LIST_DIR}/include/piz/key.h"
            "${CMAKE_CURRENT_LIST_DIR}/include/piz/midistuff.h"
            # Sources
            "${CMAKE_CURRENT_LIST_DIR}/src/piz/ChordFunctions.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/piz/DrawablePad.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/piz/PizButton.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/piz/BankStorage.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/piz/VSTSlider.cpp")
target_link_libraries(
  piz_common_gui
  INTERFACE juce::juce_core #
            juce::juce_audio_processors #
            juce::juce_data_structures #
            juce::juce_gui_basics)
target_compile_definitions(piz_common_gui
                           INTERFACE JUCE_MODAL_LOOPS_PERMITTED=1)
