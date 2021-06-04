#include <chrono>
#include <ostream>
template <typename Rep, typename Period>
std::ostream& operator<<(std::ostream& s,
                         const std::chrono::duration<Rep, Period> d)
{
  return s << d.count();
}

#include "supernovas/julian_date.h"
#include <catch2/catch.hpp>

using namespace std::literals::chrono_literals;

SCENARIO("A user can construct a Julian date.")
{
  WHEN("A user default constructs a Julian date")
  {
    supernovas::julian_date jd;
    THEN("The day is 0") { CHECK(jd.day() == 0); }
    AND_THEN("The time of day is 0") { CHECK(jd.time_of_day() == 0ns); }
  }

  WHEN("A user constructs a Julian date from a clock time point")
  {
    const supernovas::julian_date jd{
      std::chrono::system_clock::time_point{1615273885865337375s}};
    THEN("The day is correct") { CHECK(jd.day() == 2459282); }
    AND_THEN("The time of day is correct")
    {
      // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
      CHECK(jd.time_of_day() == 6.908458e13ns);
    }
  }

  WHEN("A user constructs a Julian date from a floating point value")
  {
    constexpr supernovas::julian_date jd{45.234f};
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
    THEN("The day is correct") { CHECK(jd.day() == 45); }
    AND_THEN("The time of day is correct")
    {
      // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
      CHECK(jd.time_of_day() == 2.0218e13ns);
    }
  }

  WHEN("A user constructs a Julian date from a calendar date")
  {
    using namespace date::literals;  // NOLINT(google-build-using-namespace)
    constexpr supernovas::julian_date jd{2021_y / date::June / 3_d};
    THEN("The day is correct") { CHECK(jd.day() == 21'154); }
    AND_THEN("The time of day is correct") { CHECK(jd.time_of_day() == 0ns); }
  }
}

SCENARIO("A user can compare Julian dates.")
{
  // #lizard forgives
  // I can't do much about the length of this scenario. Much of the excess is
  // the data table.
  GIVEN("Two Julian dates")
  {
    struct expected_results
    {
      bool equality;
      bool inequality;
      bool less_than;
      bool less_than_equal;
      bool greater_than;
      bool greater_than_equal;
    };
    const auto [a, b, expected_result]{GENERATE(
      table<supernovas::julian_date, supernovas::julian_date, expected_results>(
        {{{345, 987ns},  // a == b
          {345, 987ns},
          {true, false, false, true, false, true}},
         {{345, 987ns},  // a > b due to time of day
          {345, 897ns},
          {false, true, false, false, true, true}},
         {{345, 987ns},  // a > b due to day
          {334, 987ns},
          {false, true, false, false, true, true}},
         {{345, 897ns},  // a < b due to time of day
          {345, 987ns},
          {false, true, true, true, false, false}},
         {{334, 987ns},  // a < b due to day
          {345, 987ns},
          {false, true, true, true, false, false}}}))};
    WHEN("The two dates are compared for equality")
    {
      THEN("The result is correct.")
      {
        CHECK((a == b) == expected_result.equality);
      }
    }
    AND_WHEN("The two dates are compared for inequality")
    {
      THEN("The result is correct.")
      {
        CHECK((a != b) == expected_result.inequality);
      }
    }
    AND_WHEN("The two dates are compared with less-than")
    {
      THEN("The result is correct")
      {
        CHECK((a < b) == expected_result.less_than);
      }
    }
    AND_WHEN("The dates are compared with less-than-or-equal")
    {
      THEN("The result is correct")
      {
        CHECK((a <= b) == expected_result.less_than_equal);
      }
    }
    AND_WHEN("The dates are compared with greater-than")
    {
      THEN("The result is correct")
      {
        CHECK((a > b) == expected_result.greater_than);
      }
    }
    AND_WHEN("The dates are compared with greater-than-or-equal")
    {
      THEN("The result is correct")
      {
        CHECK((a >= b) == expected_result.greater_than_equal);
      }
    }
  }
}

SCENARIO("A user can add time to a Julian date.")
{
  GIVEN("A Julian date")
  {
    constexpr supernovas::julian_date jd{245, 0ns};
    const auto [time_to_add, expected_date]{
      GENERATE(table<std::chrono::seconds, supernovas::julian_date>(
        {{2s, supernovas::julian_date{245, 2s}},
         {86400s, supernovas::julian_date{246, 0ns}},
         {2 * 86400s, supernovas::julian_date{247, 0ns}}}))};
    WHEN("The time is added to the Julian date")
    {
      const auto actual_date{jd + time_to_add};
      THEN("The new Julian date is correct.")
      {
        CHECK(actual_date == expected_date);
      }
    }
  }
}

SCENARIO("A user can subtract time from a Julian date.")
{
  GIVEN("A Julian date")
  {
    constexpr supernovas::julian_date jd{245, 0ns};
    const auto [time_to_subtract, expected_date]{
      GENERATE(table<std::chrono::seconds, supernovas::julian_date>(
        {{2s, supernovas::julian_date{244, 86398s}},
         {86400s, supernovas::julian_date{244, 0ns}},
         {2 * 86400s, supernovas::julian_date{243, 0ns}}}))};
    WHEN("The time is subtracted from the Julian date")
    {
      const auto actual_date{jd - time_to_subtract};
      THEN("The new Julian date is correct.")
      {
        CHECK(actual_date == expected_date);
      }
    }
  }
}

SCENARIO("A user can increment a Julian date")
{
  GIVEN("A Julian date")
  {
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
    supernovas::julian_date jd{132, 56ns};
    WHEN("The date is post-incremented")
    {
      const auto copy{jd++};
      THEN("The Julian day is correct") { CHECK(jd.day() == 133); }
      AND_THEN("The time of day is unmodified")
      {
        CHECK(jd.time_of_day() == 56ns);
      }
      AND_THEN("The returned Julian date has the original day")
      {
        CHECK(copy.day() == 132);
      }
      AND_THEN("The returned Julian date has the original time of day")
      {
        CHECK(copy.time_of_day() == 56ns);
      }
    }
    AND_WHEN("The date is pre-incremented")
    {
      const auto copy{++jd};
      THEN("The Julian day is correct") { CHECK(jd.day() == 134); }
      AND_THEN("The time of day is unmodified")
      {
        CHECK(jd.time_of_day() == 56ns);
      }
      AND_THEN("The returned Julian date has the new day")
      {
        CHECK(copy.day() == 134);
      }
      AND_THEN("The returned Julian date has the new time of day")
      {
        CHECK(copy.time_of_day() == 56ns);
      }
    }
  }
}

SCENARIO("A user can decrement a Julian date")
{
  GIVEN("A Julian date")
  {
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
    supernovas::julian_date jd{132, 56ns};
    WHEN("The date is post-decremented")
    {
      const auto copy{jd--};
      THEN("The Julian day is correct") { CHECK(jd.day() == 131); }
      AND_THEN("The time of day is unmodified")
      {
        CHECK(jd.time_of_day() == 56ns);
      }
      AND_THEN("The returned Julian date has the original day")
      {
        CHECK(copy.day() == 132);
      }
      AND_THEN("The returned Julian date has the original time of day")
      {
        CHECK(copy.time_of_day() == 56ns);
      }
    }
    AND_WHEN("The date is pre-decremented")
    {
      const auto copy{--jd};
      THEN("The Julian day is correct") { CHECK(jd.day() == 130); }
      AND_THEN("The time of day is unmodified")
      {
        CHECK(jd.time_of_day() == 56ns);
      }
      AND_THEN("The returned Julian date has the new day")
      {
        CHECK(copy.day() == 130);
      }
      AND_THEN("The returned Julian date has the new time of day")
      {
        CHECK(copy.time_of_day() == 56ns);
      }
    }
  }
}
