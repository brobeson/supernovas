#include <catch2/catch_test_macros.hpp>

SCENARIO("A developer wants to ensure tests build and run.")
{
    GIVEN("A test")
    {
        WHEN("it runs an assertion")
        {
            THEN("the assertion passes")
            {
                CHECK(true);
            }
        }
    }
}