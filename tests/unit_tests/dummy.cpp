#include "supernovas/constants.h"
#include <catch2/catch_test_macros.hpp>

SCENARIO("A developer wants to ensure tests build and run.")
{
    GIVEN("A test")
    {
        WHEN("it runs an assertion")
        {
            THEN("the assertion passes")
            {
                CHECK(supernovas::c == 299792458.0);
            }
        }
    }
}