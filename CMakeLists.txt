cmake_minimum_required(VERSION 3.19)
project(
  supernovas
  VERSION 0.0.0
  DESCRIPTION "A pure C++ implementation of the NOVAS library."
  HOMEPAGE_URL "https://github.com/brobeson/supernovas"
  LANGUAGES CXX
)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  message(
    FATAL_ERROR
    "Your compiler, ${CMAKE_CXX_COMPILER_ID}, is not supported yet."
  )
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-tools")
find_package(Lizard REQUIRED)

set(CMAKE_DEBUG_POSTFIX "-d")
add_subdirectory(supernovas)

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(unit_tests)
endif()
