#pragma once

#include <string>

class Price
{
public:
    static constexpr int centsPerUnit = 100;

    Price() = default;
    Price(float units);
    Price(double units);
    constexpr Price(int cents);

    std::string toString() const;

    constexpr int cents() const;

    constexpr Price rounded(int cents) const;

    constexpr float units() const;

private:
    int m_cents;
};

constexpr Price::Price(int cents) :
    m_cents(cents)
{

}

constexpr int Price::cents() const
{
    return m_cents;
}

constexpr Price Price::rounded(int cents) const
{
    return (m_cents + cents / 2) / cents * cents;
}

constexpr float Price::units() const
{
    return m_cents / static_cast<float>(centsPerUnit);
}
