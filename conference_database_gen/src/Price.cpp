#include "Price.h"

#include <string>

Price::Price(float units) :
    m_cents(static_cast<int>(std::round(units * centsPerUnit)))
{

}

Price::Price(int cents) :
    m_cents(cents)
{

}

std::string Price::toString() const
{
    return std::to_string(m_cents / centsPerUnit) + '.' + std::to_string(m_cents % centsPerUnit);
}

int Price::cents() const
{
    return m_cents;
}

Price Price::rounded(int cents) const
{
    return (m_cents + cents / 2) / cents * cents;
}

float Price::units() const
{
    return m_cents / static_cast<float>(centsPerUnit);
}
