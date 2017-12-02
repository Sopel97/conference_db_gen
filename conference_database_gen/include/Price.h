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

    int cents() const;

    Price rounded(int cents) const;

    float units() const;

private:
    int m_cents;
};
