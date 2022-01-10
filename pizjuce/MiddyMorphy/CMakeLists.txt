piz_add_plugin(
  piz_MiddyMorphy
  PRODUCT_NAME "MiddyMorphy"
  DESCRIPTION "Free & Open Version Of MiddyMorphy"
  PLUGIN_CODE "mmfr"
  IS_SYNTH TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE
  EDITOR_WANTS_KEYBOARD_FOCUS TRUE)

target_sources(
  piz_MiddyMorphy
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/Controller.h"
          "${CMAKE_CURRENT_LIST_DIR}/ControllerGUI.h"
          "${CMAKE_CURRENT_LIST_DIR}/ControllerList.h"
          "${CMAKE_CURRENT_LIST_DIR}/ControllerValue.h"
          "${CMAKE_CURRENT_LIST_DIR}/Cursor.h"
          "${CMAKE_CURRENT_LIST_DIR}/CursorGUI.h"
          "${CMAKE_CURRENT_LIST_DIR}/D3CKLook.h"
          "${CMAKE_CURRENT_LIST_DIR}/D3CKStdCommands.h"
          "${CMAKE_CURRENT_LIST_DIR}/MidiMorph.h"
          "${CMAKE_CURRENT_LIST_DIR}/MidiMorphGUI.h"
          "${CMAKE_CURRENT_LIST_DIR}/MidiMorphPlugInInterface.h"
          "${CMAKE_CURRENT_LIST_DIR}/Module.h"
          "${CMAKE_CURRENT_LIST_DIR}/ModuleGUI.h"
          "${CMAKE_CURRENT_LIST_DIR}/ModulePane.h"
          "${CMAKE_CURRENT_LIST_DIR}/ModulePaneModel.h"
          "${CMAKE_CURRENT_LIST_DIR}/MorphPane.h"
          "${CMAKE_CURRENT_LIST_DIR}/MorphPaneModel.h"
          "${CMAKE_CURRENT_LIST_DIR}/NumberBox.h"
          "${CMAKE_CURRENT_LIST_DIR}/Origin.h"
          "${CMAKE_CURRENT_LIST_DIR}/Scene.h"
          "${CMAKE_CURRENT_LIST_DIR}/TextBoxSlider.h"
          "${CMAKE_CURRENT_LIST_DIR}/XYItem.h"
          "${CMAKE_CURRENT_LIST_DIR}/ZoomableShiftableComponent.h"
          "${CMAKE_CURRENT_LIST_DIR}/ZoomingShiftingComponent.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/Controller.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/ControllerGUI.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/ControllerList.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/ControllerValue.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/Cursor.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/CursorGUI.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/D3CKLook.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/MidiMorph.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/MidiMorphGUI.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/MidiMorphPlugInInterface.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/Module.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/ModuleGUI.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/ModulePane.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/ModulePaneModel.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/MorphPane.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/MorphPaneModel.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/NumberBox.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/Origin.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/Scene.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/TextBoxSlider.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/XYItem.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/ZoomableShiftableComponent.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/ZoomingShiftingComponent.cpp")

target_link_libraries(
  piz_MiddyMorphy
  PRIVATE juce::juce_audio_basics juce::juce_events juce::juce_graphics
          juce::juce_gui_basics juce::juce_gui_extra)
