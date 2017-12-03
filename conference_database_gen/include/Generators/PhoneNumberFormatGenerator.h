#pragma once

#include <string>
#include <random>

class PhoneNumberFormatGenerator
{
public:
    using ResultType = std::string;

    static constexpr char digitWildcard = 'd';

    PhoneNumberFormatGenerator(
        int minSegmentLength,
        int maxSegmentLength,
        int minAreaCodeLength,
        int maxAreaCodeLength,
        int minNumberLength,
        int maxNumberLength,
        float separatorBias
    );

    template <class TRng>
    ResultType operator()(TRng& rng) const
    {
        std::string format;
        std::uniform_int_distribution<int> dPreNumberLen(m_minAreaCodeLength, m_maxAreaCodeLength);
        std::uniform_int_distribution<int> dNumberLen(m_minNumberLength, m_maxNumberLength);
        std::uniform_int_distribution<int> dDigit('0', '9');
        std::uniform_int_distribution<int> dSeparatorAfter(m_minSegmentLength, m_maxSegmentLength);
        std::bernoulli_distribution dSep(m_separatorBias);
        const int preLen = dPreNumberLen(rng);
        const int numberLen = dNumberLen(rng);
        const char separator = dSep(rng) ? ' ' : '-';
        if (preLen > 0)
        {
            format += '(';
            for (int i = 0; i < preLen; ++i)
            {
                format += digitWildcard;
            }
            format += ") ";
        }

        int withoutSeparator = 0;
        int nextSeparator = dSeparatorAfter(rng);
        for (int i = 0; i < numberLen; ++i)
        {
            format += digitWildcard;
            ++withoutSeparator;

            if (i < numberLen - 1 && withoutSeparator == nextSeparator)
            {
                format += separator;
                withoutSeparator = 0;
                nextSeparator = dSeparatorAfter(rng);
            }
        }

        return format;
    }

private:
    int m_minSegmentLength;
    int m_maxSegmentLength;
    int m_minAreaCodeLength;
    int m_maxAreaCodeLength;
    int m_minNumberLength;
    int m_maxNumberLength;
    float m_separatorBias;
};
