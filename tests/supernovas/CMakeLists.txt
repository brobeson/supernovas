include(CPM)
CPMAddPackage("gh:catchorg/Catch2@3.12.0") # cspell:ignore catchorg
include("${Catch2_SOURCE_DIR}/extras/Catch.cmake")

set(TESTS julian_clock julian_date)

foreach(TEST IN LISTS TESTS)
  add_executable(${TEST}_test ${TEST}_test.cpp)
  target_link_libraries(${TEST}_test PRIVATE Catch2::Catch2WithMain supernovas::supernovas)
  catch_discover_tests(${TEST}_test ADD_TAGS_AS_LABELS)
endforeach()
