#pragma once

#include <random>

#include "DateTime.h"

class DurationGenerator
{
public:
    using ResultType = Milliseconds;

    DurationGenerator(Milliseconds min, Milliseconds max) :
        m_distr(min.count(), max.count())
    {

    }

    template <class TRng>
    ResultType operator()(TRng& rng) const
    {
        return m_distr(rng);
    }

private:
    std::uniform_int_distribution<int64_t> m_distr;
};
