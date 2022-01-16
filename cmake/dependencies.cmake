include(FetchContent)

message(STATUS "Fetching dependency: JUCE")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
FetchContent_Declare(
  juce
  GIT_REPOSITORY https://github.com/sleiner/JUCE.git
  GIT_TAG 5a51ae15aa69a00d50b0a97d5e7a20e7acdfccab # 6.1.4 patched
)

# JUCE registers the built SDK for installation. But as for our own installers,
# we do not want a JUCE SDK to be included, we add the "EXCLUDE_FROM_ALL"
# parameter to the JUCE subdirectory being added. This has the notable effect
# that install commands are ignored.
FetchContent_Populate(juce)
add_subdirectory(${juce_SOURCE_DIR} ${juce_BINARY_DIR} EXCLUDE_FROM_ALL)
list(POP_BACK CMAKE_MESSAGE_INDENT)
