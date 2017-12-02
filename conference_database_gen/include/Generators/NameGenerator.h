#pragma once

#include <string>
#include <cctype>

#include "MarkovChainsDictionary.h"

class NameGenerator
{
public:
    using ResultType = std::string;
    using DictionaryType = MarkovChainsDictionary<std::string>;

    NameGenerator(const DictionaryType& dictionary, int minLength, int maxOptimalLength, int maxLength);

    template <class TRng>
    ResultType operator()(TRng& rng) const
    {
        std::string name;
        std::uniform_int_distribution<int> dOptimal(m_minLength, m_maxOptimalLength);
        std::uniform_int_distribution<int> dMax(m_maxOptimalLength, m_maxLength);
        const int optimalLen = dOptimal(rng);
        const int maxLen = dMax(rng);
        name.reserve(maxLen);
        for (int i = 0; i < maxLen; ++i)
        {
            const auto& step = m_dictionary->nextStep(name);
            const int j = step.distribution(rng);
            name += step.options[j];

            if (i >= optimalLen && !std::isalpha(name.back()) && !std::isdigit(name.back())) break;
        }

        while (!name.empty() && !std::isalpha(name.back()) && !std::isdigit(name.back())) name.pop_back();

        return name;
    }

private:
    const DictionaryType* m_dictionary;
    int m_minLength;
    int m_maxOptimalLength;
    int m_maxLength;
};
