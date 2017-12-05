#pragma once

#include <string>

struct Millisecond;

template <class TBaseUnit, int64_t NumUnits>
class Duration
{
public:
    using BaseUnitType = TBaseUnit;
    static constexpr int64_t numUnits = NumUnits;

    Duration() = default;

    constexpr Duration(int64_t c) :
        m_count(c)
    {

    }

    constexpr int64_t count() const
    {
        return m_count;
    }

    constexpr operator Duration<BaseUnitType, 1>()
    {
        return { m_count * numUnits };
    }

    constexpr Duration<TBaseUnit, NumUnits> operator-() const
    {
        return Duration<TBaseUnit, NumUnits>(-m_count);
    }

    template <int64_t NumUnitsRhs>
    constexpr Duration& operator+=(Duration<TBaseUnit, NumUnitsRhs> rhs)
    {
        m_count += rhs.count() * NumUnitsRhs;
        return *this;
    }
    template <int64_t NumUnitsRhs>
    constexpr Duration& operator-=(Duration<TBaseUnit, NumUnitsRhs> rhs)
    {
        m_count -= rhs.count() * NumUnitsRhs;
        return *this;
    }

private:
    int64_t m_count;
};

template <class TBaseUnit, int64_t NumUnitsLhs, int64_t NumUnitsRhs>
constexpr Duration<TBaseUnit, 1> operator+(Duration<TBaseUnit, NumUnitsLhs> lhs, Duration<TBaseUnit, NumUnitsRhs> rhs)
{
    return { lhs.count() * NumUnitsLhs + rhs.count() * NumUnitsRhs };
}
template <class TBaseUnit, int64_t NumUnitsLhs, int64_t NumUnitsRhs>
constexpr Duration<TBaseUnit, 1> operator-(Duration<TBaseUnit, NumUnitsLhs> lhs, Duration<TBaseUnit, NumUnitsRhs> rhs)
{
    return { lhs.count() * NumUnitsLhs - rhs.count() * NumUnitsRhs };
}

using Milliseconds = Duration<Millisecond, 1>;
using Seconds = Duration<Millisecond, Milliseconds::numUnits * 1000>;
using Minutes = Duration<Millisecond, Seconds::numUnits * 60>;
using Hours = Duration<Millisecond, Minutes::numUnits * 60>;
using Days = Duration<Millisecond, Hours::numUnits * 24>;
using CommonYears = Duration<Millisecond, Days::numUnits * 365>;
using LeapYears = Duration<Millisecond, Days::numUnits * 366>;

class Months
{
public:
    Months() = default;

    constexpr Months(int64_t c);

    constexpr int64_t count() const;

private:
    int64_t m_count;
};

class Years
{
public:
    Years() = default;

    constexpr Years(int64_t c);

    constexpr int64_t count() const;

private:
    int64_t m_count;
};

class DateTime
{
public:
    DateTime() = default;

    constexpr DateTime(Years year, Months month, Days day, Milliseconds milliseconds);

    constexpr DateTime(
        Years year, 
        Months month = Months{ 0 }, 
        Days day = Days{ 0 }, 
        Hours hour = Hours{ 0 }, 
        Minutes minute = Minutes{ 0 }, 
        Seconds second = Seconds{ 0 }, 
        Milliseconds millisecond = Milliseconds{ 0 }
    );

    constexpr DateTime(Milliseconds duration);

    constexpr DateTime operator+(Milliseconds millis) const;
    constexpr DateTime operator-(Milliseconds millis) const;

    constexpr DateTime& operator+=(Milliseconds millis);
    constexpr DateTime& operator-=(Milliseconds millis);

    constexpr Milliseconds milliseconds() const;

    // must be an even number of milliseconds
    constexpr DateTime rounded(Milliseconds roundTo) const;

    std::string toString() const;

private:
    Milliseconds m_milliseconds;

    constexpr static Milliseconds toMilliseconds(Years year, Months month, Days day, Milliseconds milliseconds);
    constexpr static Milliseconds toMilliseconds(Years year, Months month, Days day, Hours hour, Minutes minute, Seconds second, Milliseconds millisecond);
};


constexpr Months::Months(int64_t c) :
    m_count(c)
{

}

constexpr int64_t Months::count() const
{
    return m_count;
}

constexpr Years::Years(int64_t c) :
    m_count(c)
{

}

constexpr int64_t Years::count() const
{
    return m_count;
}

constexpr DateTime::DateTime(Years year, Months month, Days day, Milliseconds milliseconds) :
    m_milliseconds(toMilliseconds(year, month, day, milliseconds))
{

}

constexpr DateTime::DateTime(
    Years year,
    Months month,
    Days day,
    Hours hour,
    Minutes minute,
    Seconds second,
    Milliseconds millisecond
) :
    m_milliseconds(toMilliseconds(year, month, day, hour, minute, second, millisecond))
{

}

constexpr DateTime::DateTime(Milliseconds duration) :
    m_milliseconds(duration)
{

}

constexpr DateTime DateTime::operator+(Milliseconds millis) const
{
    return DateTime(m_milliseconds + millis);
}

constexpr DateTime DateTime::operator-(Milliseconds millis) const
{
    return DateTime(m_milliseconds - millis);
}

constexpr DateTime& DateTime::operator+=(Milliseconds millis)
{
    m_milliseconds = m_milliseconds + millis;
    return *this;
}

constexpr DateTime& DateTime::operator-=(Milliseconds millis)
{
    m_milliseconds = m_milliseconds - millis;
    return *this;
}

constexpr Milliseconds DateTime::milliseconds() const
{
    return m_milliseconds;
}

// must be an even number of milliseconds
constexpr DateTime DateTime::rounded(Milliseconds roundTo) const
{
    const int64_t r = roundTo.count();
    const int64_t m = m_milliseconds.count();

    return Milliseconds((m + r / 2) / r * r);
}

constexpr Milliseconds DateTime::toMilliseconds(Years year, Months month, Days day, Milliseconds milliseconds)
{
    int64_t y = year.count();
    int64_t m = month.count() + 1;
    int64_t d = day.count() + 1;

    y -= m <= 2;
    const int64_t era = (y >= 0 ? y : y - 399) / 400;
    const int64_t yoe = y - era * 400;      // [0, 399]
    const int64_t doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;  // [0, 365]
    const int64_t doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;         // [0, 146096]
    return Milliseconds{ (era * 146097 + doe - 719468) * Days::numUnits } +milliseconds; // 719468 = days from 0000-03-01 to 1970-01-01
}

constexpr Milliseconds DateTime::toMilliseconds(Years year, Months month, Days day, Hours hour, Minutes minute, Seconds second, Milliseconds millisecond)
{
    return toMilliseconds(year, month, day, hour + minute + second + millisecond);
}