include(FetchContent)

message(STATUS "Fetching dependency: WDL")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
FetchContent_Declare(
  wdl
  GIT_REPOSITORY https://github.com/sleiner/WDL.git # TODO: Use official source
  GIT_TAG 4ddec3bd31b31318e391f8aa6be7bf1eb686534d)
FetchContent_MakeAvailable(wdl)
add_library(wdl_headers INTERFACE)
target_include_directories(wdl_headers INTERFACE "${wdl_SOURCE_DIR}/WDL")
list(POP_BACK CMAKE_MESSAGE_INDENT)

message(STATUS "Fetching dependency: Reaper SDK")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
FetchContent_Declare(
  reaper_sdk
  GIT_REPOSITORY https://github.com/justinfrankel/reaper-sdk.git
  GIT_TAG 9e4c282e11036415e546144105a0d7d709d3f8f3 # main as of 2021-12-26
)
FetchContent_MakeAvailable(reaper_sdk)
add_library(reaper_sdk INTERFACE)
target_sources(
  reaper_sdk INTERFACE ${reaper_sdk_SOURCE_DIR}/reaper-plugins/reaper_plugin.h)
target_include_directories(reaper_sdk
                           INTERFACE "${reaper_sdk_SOURCE_DIR}/reaper-plugins")
target_link_libraries(reaper_sdk INTERFACE wdl_headers)
list(POP_BACK CMAKE_MESSAGE_INDENT)

message(STATUS "Fetching dependency: JUCE")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
FetchContent_Declare(
  juce
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  GIT_TAG ddaa09110392a4419fecbb6d3022bede89b7e841 # 6.1.4
)

# JUCE registers the built SDK for installation. But as for our own installers,
# we do not want a JUCE SDK to be included, we add the "EXCLUDE_FROM_ALL"
# parameter to the JUCE subdirectory being added. This has the notable effect
# that install commands are ignored.
FetchContent_Populate(juce)
add_subdirectory(${juce_SOURCE_DIR} ${juce_BINARY_DIR} EXCLUDE_FROM_ALL)
list(POP_BACK CMAKE_MESSAGE_INDENT)
