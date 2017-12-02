#pragma once

#include <random>

#include "DateTime.h"

class DateTimeGenerator
{
public:
    using ResultType = DateTime;

    DateTimeGenerator(DateTime min, DateTime max);

    template <class TRng>
    ResultType operator()(TRng& rng) const
    {
        return DateTime(Milliseconds(m_distr(rng)));
    }

private:
    std::uniform_int_distribution<int64_t> m_distr;
};
