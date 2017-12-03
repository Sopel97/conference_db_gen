#include "DateTime.h"

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