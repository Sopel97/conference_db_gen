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

private:
    int m_cents;
};
