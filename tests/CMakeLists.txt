add_executable(supernovas.test)
target_sources(supernovas.test PRIVATE sample_test.cpp)
add_test(NAME supernovas.test COMMAND supernovas.test)