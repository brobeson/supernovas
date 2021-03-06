#include "supernovas/julian_date.h"
#include <catch2/catch.hpp>

using namespace std::literals::chrono_literals;

TEMPLATE_TEST_CASE(  // NOLINT
  "A user can construct a Julian date.",
  "",
  float,
  double,
  long double)
{
  using jdt = supernovas::basic_julian_date<TestType>;
  SECTION("default construction")
  {
    constexpr jdt jd{};
    CHECK(jd.date() == Approx{0.0});
  }

  SECTION("constructing from a clock time point")
  {
    const jdt jd{std::chrono::system_clock::now()};
    CHECK(jd.date() == Approx{0.0});
  }

  SECTION("constructing from a floating point value")
  {
    constexpr jdt jd{45.234f};
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
    CHECK(jd.date() == Approx{45.234});
  }
}

TEMPLATE_TEST_CASE(  // NOLINT
  "A user can add time to a Julian date.",
  "",
  float,
  double,
  long double)
{
  using jdt = supernovas::basic_julian_date<TestType>;
  SECTION("adding milliseconds")
  {
    jdt jd{1.0};
    jd = jd + 2ms;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
    CHECK(jd.date() == Approx{1.00000002315});
  }

  SECTION("adding seconds")
  {
    jdt jd{1.0};
    jd = jd + 2s;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
    CHECK(jd.date() == Approx{1.00002314815});
  }

  SECTION("adding minutes")
  {
    jdt jd{1.0};
    jd = jd + 2min;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
    CHECK(jd.date() == Approx{1.00138888889});
  }

  SECTION("adding hours")
  {
    jdt jd{1.0};
    jd = jd + 2h;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
    CHECK(jd.date() == Approx{1.08333333333});
  }
}

TEMPLATE_TEST_CASE(  // NOLINT
  "A user can subtract time from a Julian date.",
  "",
  float,
  double,
  long double)
{
  using jdt = supernovas::basic_julian_date<TestType>;
  SECTION("subtracting milliseconds")
  {
    // TODO Why can't the compiler skip the move assignment if jd is constexpr?
    jdt jd{1.0};
    jd = jd - 2ms;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
    CHECK(jd.date() == Approx{0.999999976852});
  }

  SECTION("subtracting seconds")
  {
    jdt jd{1.0};
    jd = jd - 2s;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
    CHECK(jd.date() == Approx{0.999976851852});
  }

  SECTION("subtracting minutes")
  {
    jdt jd{1.0};
    jd = jd - 2min;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
    CHECK(jd.date() == Approx{0.998611111111});
  }

  SECTION("subtracting hours")
  {
    jdt jd{1.0};
    jd = jd - 2h;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
    CHECK(jd.date() == Approx{0.916666666667});
  }
}
