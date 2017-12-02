#include "DateTime.h"

Months::Months(int64_t c) :
    m_count(c)
{

}

int64_t Months::count() const
{
    return m_count;
}

Years::Years(int64_t c) :
    m_count(c)
{

}

int64_t Years::count() const
{
    return m_count;
}

DateTime::DateTime(Years year, Months month, Days day, Milliseconds milliseconds) :
    m_milliseconds(toMilliseconds(year, month, day, milliseconds))
{

}

DateTime::DateTime(
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

DateTime::DateTime(Milliseconds duration) :
    m_milliseconds(duration)
{

}

DateTime DateTime::operator+(Milliseconds millis) const
{
    return DateTime(m_milliseconds + millis);
}

DateTime DateTime::operator-(Milliseconds millis) const
{
    return DateTime(m_milliseconds - millis);
}

DateTime& DateTime::operator+=(Milliseconds millis)
{
    m_milliseconds = m_milliseconds + millis;
    return *this;
}

DateTime& DateTime::operator-=(Milliseconds millis)
{
    m_milliseconds = m_milliseconds - millis;
    return *this;
}

Milliseconds DateTime::milliseconds() const
{
    return m_milliseconds;
}

// must be an even number of milliseconds
DateTime DateTime::rounded(Milliseconds roundTo) const
{
    const int64_t r = roundTo.count();
    const int64_t m = m_milliseconds.count();

    return Milliseconds((m + r / 2) / r * r);
}

std::string DateTime::toString() const
{
    const int64_t millis = m_milliseconds.count();

    int64_t leftMillis = millis % Days::numUnits;
    if (leftMillis < 0)
    {
        leftMillis += Days::numUnits;
    }
    const int64_t days = (millis - leftMillis) / Days::numUnits + 719468; // 719468 = days from 0000-03-01 to 1970-01-01

    const int64_t era = (days >= 0 ? days : days - 146096) / 146097;
    const int64_t doe = days - era * 146097;          // [0, 146096]
    const int64_t yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;  // [0, 399]
    int64_t y = yoe + era * 400;
    const int64_t doy = doe - (365 * yoe + yoe / 4 - yoe / 100);                // [0, 365]
    const int64_t mp = (5 * doy + 2) / 153;                                   // [0, 11]
    const int64_t d = doy - (153 * mp + 2) / 5 + 1;                             // [1, 31]
    const int64_t m = mp + (mp < 10 ? 3 : -9);   // [1, 12]
    y += m <= 2;

    const int64_t millisecond = leftMillis % 1000;
    leftMillis /= 1000;
    const int64_t second = leftMillis % 60;
    leftMillis /= 60;
    const int64_t minute = leftMillis % 60;
    leftMillis /= 60;
    const int64_t hour = leftMillis % 24;

    char str[24] = { '\0' };
    snprintf(str, 24, "%04lld-%02lld-%02lld %02lld:%02lld:%02lld.%03lld", y, m, d, hour, minute, second, millisecond);
    return std::string(str);
}
Milliseconds DateTime::toMilliseconds(Years year, Months month, Days day, Milliseconds milliseconds)
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

Milliseconds DateTime::toMilliseconds(Years year, Months month, Days day, Hours hour, Minutes minute, Seconds second, Milliseconds millisecond)
{
    return toMilliseconds(year, month, day, hour + minute + second + millisecond);
}