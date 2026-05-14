#include "supernovas/julian_clock.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

using namespace novas::literals;
using namespace std::chrono_literals;

SCENARIO("julian_day_number meets type requirements", "[unit]") {
  CHECK(std::is_constructible_v<novas::julian_day_number>);
  CHECK(std::is_default_constructible_v<novas::julian_day_number>);
  CHECK(std::is_trivially_move_assignable_v<novas::julian_day_number>);
  CHECK(std::is_trivially_move_constructible_v<novas::julian_day_number>);
  CHECK(std::is_trivially_copyable_v<novas::julian_day_number>);
  CHECK(std::is_trivially_copy_assignable_v<novas::julian_day_number>);
  CHECK(std::is_trivially_copy_constructible_v<novas::julian_day_number>);
  CHECK(std::is_standard_layout_v<novas::julian_day_number>);
  CHECK(std::is_integral_v<novas::julian_day_number::day_type>);
}

SCENARIO("developers can creat julian_day_number literals", "[unit]") {
  CHECK(0_jdn == novas::julian_day_number{});
  CHECK(2'451'545_jdn == novas::julian_day_number{2'451'545});
  CHECK(-2'451'545_jdn == novas::julian_day_number{-2'451'545});
}

SCENARIO("developers can retrieve the raw day number") {
  // There are no classes of input data for this. Run a few assertions to
  // ensure it's not hard-coded `return 0` or something like that.
  CHECK((0_jdn).day() == 0);
  CHECK((1_jdn).day() == 1);
  CHECK(novas::julian_day_number{-1}.day() == -1);
}

SCENARIO("developers can negate a julian_day_number", "[unit]") {
  GIVEN("a julian_day_number") {
    const auto [original, expected]{GENERATE(
        table<novas::julian_day_number, novas::julian_day_number::day_type>(
            {{0_jdn, 0},
             {1_jdn, -1},
             {novas::julian_day_number{-1}, 1},
             {2'147'483'647_jdn, -2'147'483'647},
             {novas::julian_day_number{-2'147'483'648}, -2'147'483'648}}))};
    CAPTURE(original, expected);
    WHEN("the day is negated") {
      THEN("the new day is correct") { CHECK((-original).day() == expected); }
    }
  }
}

SCENARIO("developers can increment and decrement julian_days") {
  GIVEN("a julian_day_number") {
    const auto [start_day, expected_incremented, expected_decremented]{
        GENERATE(table<novas::julian_day_number, novas::julian_day_number,
                       novas::julian_day_number>(
            {{0_jdn, 1_jdn, -1_jdn},
             {-1_jdn, 0_jdn, -2_jdn},
             {1_jdn, 2_jdn, 0_jdn},
             {2147483647_jdn, -2147483648_jdn, 2147483646_jdn},
             {-2147483648_jdn, -2147483647_jdn, 2147483647_jdn}}))};
    CAPTURE(start_day);
    WHEN("the day is post-incremented") {
      auto actual_end_day{start_day};
      const auto actual_start_day{actual_end_day++};
      THEN("the new day is one more than the starting day") {
        CHECK(actual_end_day == expected_incremented);
      }
      THEN("the returned day is the same as the starting day") {
        CHECK(actual_start_day == start_day);
      }
    }
    WHEN("the day is pre-incremented") {
      auto actual_end_day{start_day};
      const auto actual_start_day{++actual_end_day};
      THEN("the new day is one more than the starting day") {
        CHECK(actual_end_day == expected_incremented);
      }
      THEN("the returned day is the same as the new day") {
        CHECK(actual_start_day == expected_incremented);
      }
    }
    WHEN("the day is post-decremented") {
      auto actual_end_day{start_day};
      const auto actual_start_day{actual_end_day--};
      THEN("the new day is one less than the starting day") {
        CHECK(actual_end_day == expected_decremented);
      }
      THEN("the returned day is the same as the starting day") {
        CHECK(actual_start_day == start_day);
      }
    }
    WHEN("the day is pre-decremented") {
      auto actual_end_day{start_day};
      const auto actual_start_day{--actual_end_day};
      THEN("the new day is one less than the starting day") {
        CHECK(actual_end_day == expected_decremented);
      }
      THEN("the returned day is the same as the new day") {
        CHECK(actual_start_day == expected_decremented);
      }
    }
  }
}

SCENARIO("developers can add and subtract days") {
  GIVEN("a julian_day_number") {
    auto [start_day, delta_days, expected_added, expected_subtracted]{
        GENERATE(table<novas::julian_day_number, std::chrono::days::rep,
                       novas::julian_day_number, novas::julian_day_number>(
            {{0_jdn, 3, 3_jdn, -3_jdn}}))};
    CAPTURE(start_day, delta_days);
    WHEN("the delta is added with compound assignment") {
      start_day += std::chrono::days{delta_days};
      THEN("the julian_day_number matches") {
        CHECK(start_day == expected_added);
      }
    }
    WHEN("the delta is added") {
      const auto actual_day{start_day + std::chrono::days{delta_days}};
      THEN("the new julian_day_number is correct") {
        CHECK(actual_day == expected_added);
      }
    }
    WHEN("the delta is subtracted with compound assignment") {
      start_day -= std::chrono::days{delta_days};
      THEN("the julian_day_number matches") {
        CHECK(start_day == expected_subtracted);
      }
    }
    WHEN("the delta is subtracted") {
      const auto actual_day{start_day - std::chrono::days{delta_days}};
      THEN("the new julian_day_number is correct") {
        CHECK(actual_day == expected_subtracted);
      }
    }
  }
}

SCENARIO("developers can compare two julian_days") {
  // Let the compiler generate the comparison operators, but keep this test
  // in case changes to the class cause the compiler to generate unexpected
  // comparisons.
  GIVEN("two julian_day_numbers") {
    const auto [a, b, expected_equal, expected_unequal, expected_less,
                expected_less_equal, expected_greater, expected_greater_equal]{
        GENERATE(table<novas::julian_day_number, novas::julian_day_number, bool,
                       bool, bool, bool, bool, bool>(
            {{0_jdn, 0_jdn, true, false, false, true, false, true},
             {0_jdn, 1_jdn, false, true, true, true, false, false},
             {0_jdn, -1_jdn, false, true, false, false, true, true}}))};
    WHEN("the two days are compared for equality") {
      const auto actual{a == b};
      THEN("the result is correct") { CHECK(actual == expected_equal); }
    }
    WHEN("The two days are compared for inequality") {
      const auto actual{a != b};
      THEN("The result is correct") { CHECK(actual == expected_unequal); }
    }
    WHEN("The two days are compared for less-than") {
      const auto actual{a < b};
      THEN("The result is correct") { CHECK(actual == expected_less); }
    }
    WHEN("The two days are compared for less-than-or-equal") {
      const auto actual{a <= b};
      THEN("The result is correct") { CHECK(actual == expected_less_equal); }
    }
    WHEN("The two days are compared for greater-than") {
      const auto actual{a > b};
      THEN("The result is correct") { CHECK(actual == expected_greater); }
    }
    WHEN("The two days are compared for greater-than-or-equal") {
      const auto actual{a >= b};
      THEN("The result is correct") { CHECK(actual == expected_greater_equal); }
    }
  }
}

SCENARIO("developers can insert julian_days into a stream") {
  std::ostringstream s;
  s << 2456_jdn;
  CHECK(s.str() == "2456");
}

namespace {
// In CHECK_THROWS_AS(), the compiler seems to optimize out a lambda; it
// wouldn't throw the exception. Using a normal function works.
auto makeInvalidJulianDay(const auto &date) {
  return novas::julian_day_number{date};
}
} // namespace

SCENARIO("developers can convert calendar dates <-> Julian day numbers") {
  GIVEN("a calendar date") {
    const auto [gregorian, expected_julian]{
        // I used this as the source of truth for date conversion:
        // https://numerical.recipes/julian.html
        GENERATE(table<std::chrono::year_month_day, novas::julian_day_number>(
            {{2009y / std::chrono::June / 19d, 2'455'002_jdn},
             {2026y / std::chrono::March / 22d, 2'461'122_jdn},
             {1458y / std::chrono::January / 1d, 2253593_jdn},
             // change over from Julian to Gregorian calendars
             {1582y / std::chrono::October / 15d, 2299161_jdn},
             {1582y / std::chrono::October / 4d, 2299160_jdn},
             // Skip over year 0: go from Dec 31 0001 BC to Jan 1 0001 AD
             {1y / std::chrono::January / 1d, 1721424_jdn},
             {-1y / std::chrono::December / 31d, 1721423_jdn},
             {-4713y / std::chrono::January / 1d, 0_jdn},
             {-4717y / std::chrono::March / 1d, -1401_jdn}}))};
    CAPTURE(gregorian);
    WHEN("the Gregorian date is converted to a julian_day_number") {
      THEN("the julian_day_number is correct") {
        CHECK(novas::julian_day_number{gregorian} == expected_julian);
      }
    }
    WHEN("the Julian day number is converted to a calendar date") {
      THEN("the calendar date is correct") {
        CHECK(novas::calendar_date(expected_julian) == gregorian);
      }
    }
  }
  GIVEN("an invalid calendar date") {
    const auto date{GENERATE(values({
        -4717y / std::chrono::February / 28d,
        0y / std::chrono::January / 1d,
        1582y / std::chrono::October / 5d,
        1582y / std::chrono::October / 6d,
        1582y / std::chrono::October / 7d,
        1582y / std::chrono::October / 8d,
        1582y / std::chrono::October / 9d,
        1582y / std::chrono::October / 10d,
        1582y / std::chrono::October / 11d,
        1582y / std::chrono::October / 12d,
        1582y / std::chrono::October / 13d,
        1582y / std::chrono::October / 14d,
    }))};
    WHEN("a Julian day is created") {
      THEN("an exception is thrown") {
        CHECK_THROWS_AS(makeInvalidJulianDay(date), std::invalid_argument);
      }
    }
  }
}

#if 0
SCENARIO("The Julian clock can convert Gregorian to Julian", "[unit]")
{
  GIVEN("a Gregorian date")
  {
    using namespace std::chrono_literals;
    const auto [gregorian, expected]{
      GENERATE(table<std::chrono::year_month_day, novas::julian_date>(
        {{2024y / std::chrono::January / 1d, 0_jdn}}))};
    WHEN("the Julian clock converts to the Julian date")
    {
      const auto actual{novas::julian_clock::to_julian_date(gregorian)};
      THEN("the Julian date is correct")
      {
        CHECK(actual == expected);
      }
    }
  }
}

SCENARIO("The Julian clock can convert Julian to Gregorian", "[unit]")
{
  GIVEN("a Julian date")
  {
    const auto [julian, expected]{
      GENERATE(table<novas::julian_date, std::chrono::year_month_day>(
        {{novas::julian_date{novas::julian_clock::duration{0}},
          0y / std::chrono::January / 1d}}))};
    WHEN("the Julian clock converts to the Gregorian date")
    {
      const auto actual{novas::julian_clock::to_gregorian_date(julian)};
      THEN("the Gregorian date is correct")
      {
        CHECK(actual == expected);
      }
    }
  }
}
#endif
