#pragma once

#include <string>
#include <random>

#include "PhoneNumberFormatGenerator.h"

class PhoneNumberGenerator
{
public:
    using ResultType = std::string;

    PhoneNumberGenerator(const std::string& format);
    PhoneNumberGenerator(std::string&& format);

    template <class TRng>
    ResultType operator()(TRng& rng) const
    {
        std::string phone = m_format;
        std::uniform_int_distribution<int> dDigit('0', '9');

        for (char& ch : phone)
        {
            if (ch == PhoneNumberFormatGenerator::digitWildcard)
            {
                ch = dDigit(rng);
            }
        }

        return phone;
    }

private:
    std::string m_format;
};
