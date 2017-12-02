#pragma once

#include <string>

struct Millisecond;

template <class TBaseUnit, int64_t NumUnits>
class Duration
{
public:
    using BaseUnitType = TBaseUnit;
    static constexpr int64_t numUnits = NumUnits;

    Duration(int64_t c) :
        m_count(c)
    {

    }

    int64_t count() const
    {
        return m_count;
    }

    operator Duration<BaseUnitType, 1>()
    {
        return { m_count * numUnits };
    }

    Duration<TBaseUnit, NumUnits> operator-() const
    {
        return Duration<TBaseUnit, NumUnits>(-m_count);
    }

private:
    int64_t m_count;
};

template <class TBaseUnit, int64_t NumUnitsLhs, int64_t NumUnitsRhs>
Duration<TBaseUnit, 1> operator+(Duration<TBaseUnit, NumUnitsLhs> lhs, Duration<TBaseUnit, NumUnitsRhs> rhs)
{
    return { lhs.count() * NumUnitsLhs + rhs.count() * NumUnitsRhs };
}
template <class TBaseUnit, int64_t NumUnitsLhs, int64_t NumUnitsRhs>
Duration<TBaseUnit, 1> operator-(Duration<TBaseUnit, NumUnitsLhs> lhs, Duration<TBaseUnit, NumUnitsRhs> rhs)
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
    Months(int64_t c);

    int64_t count() const;

private:
    int64_t m_count;
};

class Years
{
public:
    Years(int64_t c);

    int64_t count() const;

private:
    int64_t m_count;
};

class DateTime
{
public:
    DateTime(Years year, Months month, Days day, Milliseconds milliseconds);

    DateTime(
        Years year = Years{ 1900 }, 
        Months month = Months{ 0 }, 
        Days day = Days{ 0 }, 
        Hours hour = Hours{ 0 }, 
        Minutes minute = Minutes{ 0 }, 
        Seconds second = Seconds{ 0 }, 
        Milliseconds millisecond = Milliseconds{ 0 }
    );

    DateTime(Milliseconds duration);

    DateTime operator+(Milliseconds millis) const;
    DateTime operator-(Milliseconds millis) const;

    DateTime& operator+=(Milliseconds millis);
    DateTime& operator-=(Milliseconds millis);

    Milliseconds milliseconds() const;

    // must be an even number of milliseconds
    DateTime rounded(Milliseconds roundTo) const;

    std::string toString() const;

private:
    Milliseconds m_milliseconds;

    static Milliseconds toMilliseconds(Years year, Months month, Days day, Milliseconds milliseconds);
    static Milliseconds toMilliseconds(Years year, Months month, Days day, Hours hour, Minutes minute, Seconds second, Milliseconds millisecond);
};
