cmake_minimum_required(VERSION 3.21)

project(
  pizmidi
  VERSION 2.0.0
  HOMEPAGE_URL "https://github.com/sleiner/pizmidi"
  LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_EXTENSIONS OFF)

if(NOT ${CMAKE_SIZEOF_VOID_P} EQUAL 8)
  message(WARNING "\
sizeof(void*) = ${CMAKE_SIZEOF_VOID_P} detected! \
You are trying to configure a non-64 bit build, which is not supported.")
endif()

option(PIZ_ENABLE_TESTS "If ON, test targets are available" ON)

include(cmake/installer.cmake)
include(cmake/dependencies.cmake)

add_subdirectory(pizjuce)
add_subdirectory(pizmidi)

if(${PIZ_ENABLE_TESTS})
  enable_testing()
  add_subdirectory(test)
endif()
