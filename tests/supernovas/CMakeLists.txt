include(CPM.cmake)
CPMAddPackage(URI "gh:catchorg/Catch2@3.12.0" SYSTEM) # cspell:ignore catchorg
set_target_properties(Catch2 Catch2WithMain PROPERTIES EXPORT_COMPILE_COMMANDS false)
include("${Catch2_SOURCE_DIR}/extras/Catch.cmake")

add_executable(sample_test sample_test.cpp)
target_link_libraries(sample_test PRIVATE Catch2::Catch2WithMain)
# target_compile_options(sample_test PRIVATE --coverage)
# target_link_options(sample_test PRIVATE --coverage)
# target_compile_options(sample_test PRIVATE -fprofile-instr-generate -fcoverage-mapping)
# target_link_options(sample_test PRIVATE -fprofile-instr-generate)
# target_compile_options(
#   sample_test
#   PRIVATE
#     $<$<BOOL:${SUPERNOVAS_BUILD_COVERAGE}>:-fno-default-inline -fno-inline --coverage -fprofile-abs-path>
# )
catch_discover_tests(sample_test PROPERTIES LABELS unit)
