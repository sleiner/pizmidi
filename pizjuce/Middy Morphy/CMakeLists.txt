piz_add_plugin(
  pizmidi_MiddyMorphy
  PRODUCT_NAME "MiddyMorphy"
  DESCRIPTION "Free & Open Version Of MiddyMorphy"
  PLUGIN_CODE "mmfr"
  IS_SYNTH TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE
  EDITOR_WANTS_KEYBOARD_FOCUS TRUE
  VERSION "1.2.0")

target_sources(
  pizmidi_MiddyMorphy
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/src/Controller.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/ControllerGUI.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/ControllerList.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/ControllerValue.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/Cursor.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/CursorGUI.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/D3CKLook.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/D3CKStdCommands.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiMorph.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiMorphGUI.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiMorphPlugInInterface.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/Module.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/ModuleGUI.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/ModulePane.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/ModulePaneModel.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/MorphPane.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/MorphPaneModel.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/NumberBox.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/Origin.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/Scene.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/SceneGUI.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/TextBoxSlider.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/XYItem.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/ZoomableShiftableComponent.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/ZoomingShiftingComponent.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/src/Controller.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/ControllerGUI.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/ControllerList.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/ControllerValue.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/Cursor.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/CursorGUI.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/D3CKLook.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/D3CKStdCommands.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiMorph.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiMorphGUI.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiMorphPlugInInterface.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/Module.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/ModuleGUI.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/ModulePane.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/ModulePaneModel.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/MorphPane.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/MorphPaneModel.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/NumberBox.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/Origin.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/Scene.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/SceneGUI.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/TextBoxSlider.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/XYItem.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/ZoomableShiftableComponent.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/ZoomingShiftingComponent.cpp")

target_link_libraries(
  pizmidi_MiddyMorphy
  PUBLIC juce::juce_audio_basics juce::juce_events juce::juce_graphics
         juce::juce_gui_basics juce::juce_gui_extra)
