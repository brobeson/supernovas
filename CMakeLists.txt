cmake_minimum_required(VERSION 3.19)

# list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-tools")
find_package(cmake-tools REQUIRED)
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

set(CMAKE_DEBUG_POSTFIX "-d")
add_subdirectory(supernovas)

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(unit_tests)
endif()
