#ifndef SUPERNOVAS_JULIAN_DATE_H
#define SUPERNOVAS_JULIAN_DATE_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
#include <date/date.h>
#pragma GCC diagnostic pop
#include <chrono>
#include <cmath>
#include <type_traits>

namespace supernovas
{
  // namespace detail
  // {
  //   constexpr auto time_of_day(const long double t)
  //   {
  //     return t - std::floor(t);
  //     // const auto fraction{t - std::floor(t)};
  //     // constexpr std::chrono::nanoseconds ns_per_day{86'400'000'000'000};
  //     // // return std::chrono::nanoseconds{
  //     // //   static_cast<std::chrono::nanoseconds::rep>(fraction
  //     // //                                              //  *
  //     // // ns_per_day.count())};
  //     // //                                              *
  //     86'400'000'000'000)};
  //     // using day = std::chrono::duration<long double, std::ratio<86400>>;
  //     // return day{fraction};
  //   }
  // }  // namespace detail

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
     * \brief Construct a Julian date from a clock time point.
     * \tparam Clock The type of clock for the time point.
     * \param[in] time The time point to convert to a Julian date.
     */
    template <typename Clock>
    constexpr explicit julian_date(
      std::chrono::time_point<Clock> /*time*/) noexcept
    {}

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
     * \brief Construct a Julian date from a floating point value.
     * \tparam Float The floating point type. This must be a floating point
     *    type, or an integer type. Note that boolean and character types are
     *    not allowed. If Float is an integer type, the time of day is
     *    initialized to 0.
     * \param[in] jd The Julian date number to convert to a julian_date object.
     */
    template <typename Float>
    constexpr explicit julian_date(Float jd) noexcept
      : m_day{static_cast<day_type>(jd)}
    {}

    /**
     * \brief Construct a Julian date from a calendar date.
     * \param[in] calendar_date The calendar date to convert to a Julian date.
     */
    constexpr explicit julian_date(
      date::year_month_day /*calendar_date*/) noexcept
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