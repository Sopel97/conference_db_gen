#pragma once

#include <string>

class Price
{
public:
    static constexpr int centsPerUnit = 100;

    Price() = default;
    Price(float units);
    Price(int cents);

    std::string toString() const;

    int cents() const
    {
        return m_cents;
    }

    Price rounded(int cents) const
    {
        return (m_cents + cents / 2) / cents * cents;
    }

    float units() const
    {
        return m_cents / static_cast<float>(centsPerUnit);
    }

private:
    int m_cents;
};
