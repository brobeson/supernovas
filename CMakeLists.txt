cmake_minimum_required(VERSION 3.19)

include(FetchContent)
FetchContent_Declare(
  cmake-tools
  GIT_REPOSITORY https://github.com/brobeson/cmake-tools.git
  GIT_TAG main
)
FetchContent_MakeAvailable(cmake-tools)
list(APPEND CMAKE_MODULE_PATH "${cmake-tools_SOURCE_DIR}")

include(CMakeToolsVersionFromGit)
project(
  supernovas
  VERSION ${CMAKE_TOOLS_GIT_TAG}
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

find_package(Lizard REQUIRED)
include(CMakeToolsDefaultCompileOptions)

FetchContent_Declare(
  date
  GIT_REPOSITORY https://github.com/HowardHinnant/date.git
  GIT_TAG v3.0.1
)
FetchContent_MakeAvailable(date)

# BUG This is a work around for https://github.com/brobeson/supernovas/issues/1.
list(REMOVE_ITEM CMAKE_TOOLS_COMPILE_OPTIONS -Wnoexcept)

set(CMAKE_DEBUG_POSTFIX "-d")
add_subdirectory(supernovas)

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(unit_tests)
endif()
