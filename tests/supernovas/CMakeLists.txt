include(CPM)
CPMAddPackage("gh:catchorg/Catch2@3.12.0") # cspell:ignore catchorg
include("${Catch2_SOURCE_DIR}/extras/Catch.cmake")

add_executable(julian_clock_test julian_clock_test.cpp)
target_link_libraries(julian_clock_test PRIVATE Catch2::Catch2WithMain supernovas::supernovas)
catch_discover_tests(julian_clock_test ADD_TAGS_AS_LABELS)
