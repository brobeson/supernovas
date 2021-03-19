#ifndef SUPERNOVAS_JULIAN_DATE_H
#define SUPERNOVAS_JULIAN_DATE_H

#include <chrono>
#include <type_traits>

namespace supernovas
{
  template <typename ValueType>
  class basic_julian_date final
  {
    static_assert(std::is_floating_point_v<ValueType>,
                  "A julian date must use a floating point type.");

  public:
    /// The data type of stored Julian date.
    using value_type = ValueType;

    /// \brief Construct Julian date 0.0.
    constexpr basic_julian_date() = default;

    /**
     * \brief Construct a Julian date from a clock time point.
     * \tparam Clock The type of clock for the time point.
     * \param[in] time The time point to convert to a Julian date.
     */
    template <typename Clock>
    constexpr explicit basic_julian_date(
      std::chrono::time_point<Clock> /*time*/) noexcept
      : m_date{0.0}
    {}

    /**
     * \brief Construct a Julian date from a raw value.
     * \param[in] date The raw value to use as the Julian date.
     */
    constexpr explicit basic_julian_date(value_type date) noexcept
      : m_date{date}
    {}

    /// The Julian date as a raw floating point value.
    [[nodiscard]] constexpr auto date() const noexcept { return m_date; }

  private:
    value_type m_date;
  };

  namespace detail
  {
    constexpr long double nanoseconds_per_day{8.64e13};

    // This is the Julian date of 00:00:00 on 1970 January 1.
    constexpr long double unix_epoch_julian_date{2440587.5};
  }  // namespace detail

  /**
   * \brief Advance a Julian date by some time duration.
   * \tparam ValueType The type of the Julian date.
   * \tparam Rep See https://en.cppreference.com/w/cpp/chrono/duration.
   * \tparam Period See https://en.cppreference.com/w/cpp/chrono/duration.
   * \param[in] date The Julian date to advance.
   * \param[in] duration The amount of time by which to advance the \a date.
   * \return The new Julian date.
   * \related basic_julian_date
   */
  template <typename ValueType, typename Rep, typename Period>
  constexpr basic_julian_date<ValueType> operator+(
    const basic_julian_date<ValueType> date,
    const std::chrono::duration<Rep, Period> duration) noexcept
  {
    // TODO Change static_cast to gsl::narrow.
    return basic_julian_date<ValueType>{
      date.date()
      + static_cast<ValueType>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()
        / detail::nanoseconds_per_day)};
  }

  /**
   * \brief Advance a Julian date by some time duration.
   * \tparam ValueType The type of the Julian date.
   * \tparam Rep See https://en.cppreference.com/w/cpp/chrono/duration.
   * \tparam Period See https://en.cppreference.com/w/cpp/chrono/duration.
   * \param[in] duration The amount of time by which to advance the \a date.
   * \param[in] date The Julian date to advance.
   * \return The new Julian date.
   * \related basic_julian_date
   */
  template <typename ValueType, typename Rep, typename Period>
  constexpr basic_julian_date<ValueType> operator+(
    const std::chrono::duration<Rep, Period> duration,
    const basic_julian_date<ValueType> date)
  {
    return date + duration;
  }

  /**
   * \brief Back up a Julian date by some time duration.
   * \tparam ValueType The type of the Julian date.
   * \tparam Rep See https://en.cppreference.com/w/cpp/chrono/duration.
   * \tparam Period See https://en.cppreference.com/w/cpp/chrono/duration.
   * \param[in] date The Julian date to back up.
   * \param[in] duration The amount of time by which to back up the \a date.
   * \return The new Julian date.
   * \related basic_julian_date
   */
  template <typename ValueType, typename Rep, typename Period>
  constexpr basic_julian_date<ValueType> operator-(
    const basic_julian_date<ValueType> date,
    const std::chrono::duration<Rep, Period> duration)
  {
    // TODO Change static_cast to gsl::narrow.
    return basic_julian_date<ValueType>{
      date.date()
      - static_cast<ValueType>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()
        / detail::nanoseconds_per_day)};
  }

  using julian_date_f = basic_julian_date<float>;
  using julian_date_d = basic_julian_date<double>;
  using julian_date_ld = basic_julian_date<long double>;
}  // namespace supernovas

#endif