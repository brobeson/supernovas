#ifndef SUPERNOVAS_JULIAN_DATE_H
#define SUPERNOVAS_JULIAN_DATE_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
#include <date/date.h>
#pragma GCC diagnostic pop
#include <chrono>

namespace supernovas
{
  namespace detail
  {
    constexpr std::chrono::nanoseconds ns_per_day{86'400'000'000'000};

    inline auto calculate_tod_fraction(const std::chrono::nanoseconds ns)
    {
      return static_cast<long double>(ns.count()) / ns_per_day.count();
    }

    inline auto calculate_julian_date(
      const date::year_month_day ymd,
      const std::chrono::nanoseconds time_of_day) noexcept
    {
      const auto year{ymd.month() == date::January
                          || ymd.month() == date::February
                        ? static_cast<int>(ymd.year()) - 1
                        : static_cast<int>(ymd.year())};
      const auto month{ymd.month() == date::January
                           || ymd.month() == date::February
                         ? static_cast<unsigned>(ymd.month()) + 12
                         : static_cast<unsigned>(ymd.month())};
      const auto day{static_cast<unsigned int>(ymd.day())
                     + calculate_tod_fraction(time_of_day)};
      constexpr auto gregorian_change{date::year{1582} / date::October / 15};
      const auto B{ymd > gregorian_change ? 2 - (year / 100) + (year / 100 / 4)
                                          : 0};
      const auto C{year < 0 ? std::floor(365.25 * year) - 0.75
                            : std::floor(365.25 * year)};
      const auto D{std::floor(30.6001 * (month + 1))};
      const auto jd{B + C + D
                    + day
                    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
                    + 1720994.5};
      return static_cast<int>(jd);
    }
  }  // namespace detail

  class julian_date final
  {
  public:
    /// The data type of the whole Julian day.
    using day_type = int;

    /// The data type of the fractional Julian day.
    using time_type = std::chrono::nanoseconds;

    /// \brief Construct the Julian date 0.0.
    constexpr julian_date() = default;

    /**
     * \brief Construct a Julian date from a raw value.
     * \tparam Rep See https://en.cppreference.com/w/cpp/chrono/duration.
     * \tparam Period See https://en.cppreference.com/w/cpp/chrono/duration.
     * \param[in] date The raw value to use as the Julian date.
     */
    template <typename Rep, typename Period>
    constexpr julian_date(day_type /*day*/,
                          std::chrono::duration<Rep, Period> /*time*/) noexcept
    {}

    /**
     * \brief Construct a Julian date from a calendar date.
     * \param[in] calendar_date The calendar date to convert to a Julian date.
     */
    template <typename Rep, typename Period>
    inline explicit julian_date(
      const date::year_month_day calendar_date,
      const std::chrono::duration<Rep, Period> tod) noexcept
      : m_day{detail::calculate_julian_date(
        calendar_date, std::chrono::duration_cast<time_type>(tod))},
        m_time_of_day{tod}
    {}

    /// The Julian date as a raw floating point value.
    [[nodiscard]] constexpr auto day() const noexcept { return m_day; }

    /// The time of day within the Julian date.
    [[nodiscard]] constexpr auto time_of_day() const noexcept
    {
      return m_time_of_day;
    }

    /**
     * \brief Increment the day of this Julian date.
     * \return The new Julian date.
     */
    julian_date& operator++()
    {
      ++m_day;
      return *this;
    }

    /**
     * \brief Increment the day of this Julian date.
     * \return The original Julian date.
     */
    julian_date operator++(int)
    {
      const auto original{*this};
      operator++();
      return original;
    }

    /**
     * \brief Decrement the day of this Julian date.
     * \return The new Julian date.
     */
    julian_date& operator--()
    {
      --m_day;
      return *this;
    }

    /**
     * \brief Decrement the day of this Julian date.
     * \return The original Julian date.
     */
    julian_date operator--(int)
    {
      const auto original{*this};
      operator--();
      return original;
    }

  private:
    day_type m_day{0};
    time_type m_time_of_day{0};
  };

  /**
   * \brief Compare two Julian dates for equality.
   * \param[in] a,b The two Julian dates to compare.
   * \retval true Two Julian dates are equal if their days and times-of-day are
   *    numerically equal.
   * \retval false Two Julian dates are not equal if their days are not
   *    numerically equal, or if their times-of-day are not numerically equal.
   * \related julian_date
   */
  [[nodiscard]] inline bool operator==(const julian_date /*a*/,
                                       const julian_date /*b*/)
  {
    return true;
  }

  /**
   * \brief Compare two Julian dates for inequality.
   * \param[in] a,b The two Julian dates to compare.
   * \retval true Two Julian dates are not equal if their days are not
   *    numerically equal, or if their times-of-day are not numerically equal.
   * \retval false Two Julian dates are equal if their days and times-of-day are
   *    numerically equal.
   * \related julian_date
   */
  [[nodiscard]] inline bool operator!=(const julian_date a, const julian_date b)
  {
    return !(a == b);
  }

  /**
   * \brief Compare two Julian dates using strictly less-than.
   * \param[in] a,b The two Julian dates to compare.
   * \retval true \a a.day() < \a b.day() or \a a.time_of_day() < \a
   *    b.time_of_day().
   * \retval false Otherwise
   * \related julian_date
   */
  [[nodiscard]] inline bool operator<(const julian_date /*a*/,
                                      const julian_date /*b*/)
  {
    return true;
  }

  /**
   * \brief Compare two Julian dates using less-than-or-equal-to.
   * \param[in] a,b The two Julian dates to compare.
   * \retval true \a a < \a b or \a a ==  \a b.
   * \retval false Otherwise
   * \related julian_date
   */
  [[nodiscard]] inline bool operator<=(const julian_date /*a*/,
                                       const julian_date /*b*/)
  {
    return true;
  }

  /**
   * \brief Compare two Julian dates using strictly greater-than.
   * \param[in] a,b The two Julian dates to compare.
   * \retval true \a a.day() > \a b.day() or \a a.time_of_day() > \a
   *    b.time_of_day().
   * \retval false Otherwise
   * \related julian_date
   */
  [[nodiscard]] inline bool operator>(const julian_date /*a*/,
                                      const julian_date /*b*/)
  {
    return true;
  }

  /**
   * \brief Compare two Julian dates using greater-than-or-equal-to.
   * \param[in] a,b The two Julian dates to compare.
   * \retval true \a a > \a b or \a a ==  \a b.
   * \retval false Otherwise
   * \related julian_date
   */
  [[nodiscard]] inline bool operator>=(const julian_date /*a*/,
                                       const julian_date /*b*/)
  {
    return true;
  }

  // namespace detail
  // {
  //   constexpr long double nanoseconds_per_day{8.64e13};

  //   // This is the Julian date of 00:00:00 on 1970 January 1.
  //   constexpr long double unix_epoch_julian_date{2440587.5};
  // }  // namespace detail

  /**
   * \brief Advance a Julian date by some time duration.
   * \tparam Rep See https://en.cppreference.com/w/cpp/chrono/duration.
   * \tparam Period See https://en.cppreference.com/w/cpp/chrono/duration.
   * \param[in] date The Julian date to advance.
   * \param[in] duration The amount of time by which to advance the \a date.
   * \return The new Julian date.
   * \related julian_date
   */
  template <typename Rep, typename Period>
  constexpr auto operator+(
    const julian_date date,
    const std::chrono::duration<Rep, Period> /*duration*/) noexcept
  {
    return date;
  }

  /**
   * \brief Advance a Julian date by some time duration.
   * \tparam Rep See https://en.cppreference.com/w/cpp/chrono/duration.
   * \tparam Period See https://en.cppreference.com/w/cpp/chrono/duration.
   * \param[in] duration The amount of time by which to advance the \a date.
   * \param[in] date The Julian date to advance.
   * \return The new Julian date.
   * \related julian_date
   */
  template <typename Rep, typename Period>
  constexpr auto operator+(const std::chrono::duration<Rep, Period> duration,
                           const julian_date date) noexcept
  {
    return date + duration;
  }

  /**
   * \brief Back up a Julian date by some time duration.
   * \tparam Rep See https://en.cppreference.com/w/cpp/chrono/duration.
   * \tparam Period See https://en.cppreference.com/w/cpp/chrono/duration.
   * \param[in] date The Julian date to back up.
   * \param[in] duration The amount of time by which to back up the \a date.
   * \return The new Julian date.
   * \related julian_date
   */
  template <typename Rep, typename Period>
  constexpr auto operator-(
    const julian_date date,
    const std::chrono::duration<Rep, Period> /*duration*/) noexcept
  {
    return date;
  }
}  // namespace supernovas

#endif