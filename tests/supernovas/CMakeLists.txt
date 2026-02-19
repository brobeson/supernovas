include(CPM)
CPMAddPackage("gh:catchorg/Catch2@3.12.0") # cspell:ignore catchorg
include("${Catch2_SOURCE_DIR}/extras/Catch.cmake")

add_executable(sample_test sample_test.cpp)
target_link_libraries(sample_test PRIVATE Catch2::Catch2WithMain)
catch_discover_tests(sample_test PROPERTIES LABELS unit)
