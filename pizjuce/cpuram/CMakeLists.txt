if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  piz_add_plugin(
    piz_CpuRam
    DESCRIPTION "CPU/RAM meter"
    PLUGIN_CODE "cpur"
    PRODUCT_NAME "CPU and RAM"
    IS_SYNTH TRUE
    VERSION "1.2.0")

  target_sources(
    piz_CpuRam
    PRIVATE # Headers
            "${CMAKE_CURRENT_LIST_DIR}/CpuRam.h"
            "${CMAKE_CURRENT_LIST_DIR}/CpuRamEditor.h"
            "${CMAKE_CURRENT_LIST_DIR}/cputime.h"
            # Sources
            "${CMAKE_CURRENT_LIST_DIR}/CpuRamEditor.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/cputime.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/CpuRam.cpp")

  target_link_libraries(piz_CpuRam PUBLIC)
else()
  message(
    NOTICE
    "The CpuRam plugin is only available on Windows, not on ${CMAKE_SYSTEM_NAME}."
  )
endif()
