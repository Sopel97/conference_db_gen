#pragma once

#include <random>

#include "Price.h"

class PriceGenerator
{
public:
    using ResultType = Price;

    PriceGenerator(Price min, Price max) :
        m_distr(min.cents(), max.cents())
    {

    }

    template <class TRng>
    ResultType operator()(TRng& rng) const
    {
        return Price(m_distr(rng));
    }

private:
    std::uniform_int_distribution<int> m_distr;
};
