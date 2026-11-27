cmake_minimum_required(VERSION 4.0)
project(
  supernovas
  VERSION 0.0.0
  DESCRIPTION "A C++ implementation of the United States Naval Observatory NOVAS library"
  HOMEPAGE_URL https://github.com/brobeson/supernovas
  LANGUAGES CXX
)

set(
  CACHE{CMAKE_CXX_STANDARD}
  TYPE STRING
  HELP "Default value for CXX_STANDARD target property."
  VALUE 20
)
set(
  CACHE{CMAKE_CXX_STANDARD_REQUIRED}
  TYPE BOOL
  HELP "Default value for CXX_STANDARD_REQUIRED target property."
  VALUE on
)

option(
  SUPERNOVAS_BUILD_TESTS
  "Build the supernovas tests. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
  ${PROJECT_IS_TOP_LEVEL}
)
enable_testing()
if(SUPERNOVAS_BUILD_TESTS)
  add_subdirectory(tests)
endif()

# Configure the Cppcheck project file. This is benign for users who do not
# want to use Cppcheck, so don't add the complexity of a CMake option.
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/cppcheck")
configure_file(.github/project.cppcheck.in project.cppcheck)
