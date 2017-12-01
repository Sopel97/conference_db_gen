#pragma once

#include <map>
#include <random>
#include <vector>
#include <iterator>

template <class TSequence>
class MarkovChainsDictionary
{
    using DistributionType = std::discrete_distribution<>;
    using ContainerType = TSequence;
    using ValueType = typename TSequence::value_type;

    struct Step
    {
        DistributionType distribution; //if the result is options.size() then it means the end of the sequence
        std::vector<ValueType> options;
    };

public:
    template <class InputIt>
    MarkovChainsDictionary(InputIt first, InputIt last, int depth) :
        m_depth(depth)
    {
        train(first, last, depth);
    }

    const Step& nextStep(const ContainerType& sequence) const
    {
        if (sequence.empty())
        {
            return m_firstStep;
        }

        int d = std::min(m_depth, static_cast<int>(sequence.size()));
        auto last = sequence.end();
        auto first = std::prev(last, d);

        while (first != last)
        {
            ContainerType subsequence(first, last);
            auto iter = m_steps.find(subsequence);
            if (iter != m_steps.end())
            {
                return iter->second;
            }

            ++first;
        }

        return m_fallback;
    }

private:
    int m_depth;
    Step m_firstStep;
    std::map<ContainerType, Step> m_steps;
    Step m_fallback;

    template <class InputIt>
    void train(InputIt first, InputIt last, int depth)
    {
        std::map<ValueType, int> firstElementHistogram = generateFirstElementHistogram(first, last);

        m_firstStep = stepFromHistogram(std::move(firstElementHistogram));
        for (int d = 1; d <= depth; ++d)
        {
            addSteps(first, last, d);
        }

        generateFallback(first, last);
    }

    template <class InputIt>
    void generateFallback(InputIt first, InputIt last)
    {
        m_fallback = stepFromHistogram(singleValueHistogram(first, last));
    }

    template <class InputIt>
    std::map<ValueType, int> singleValueHistogram(InputIt first, InputIt last)
    {
        std::map<ValueType, int> histogram;

        for (; first != last; ++first)
        {
            const ContainerType& el = *first;
            auto innerLast = el.end();
            for (auto innerFirst = el.begin(); innerFirst != innerLast; ++innerFirst)
            {
                ++histogram[*innerFirst];
            }
        }

        return histogram;
    }

    Step stepFromHistogram(std::map<ValueType, int>&& histogram) const
    {
        const int numElements = histogram.size();

        std::vector<ValueType> options;
        options.reserve(numElements);
        std::vector<int> counts;
        counts.reserve(numElements);

        for (auto&& el : std::move(histogram))
        {
            options.emplace_back(std::move(el.first));
            counts.emplace_back(el.second);
        }

        return Step{
            std::discrete_distribution<>(counts.begin(), counts.end()),
            std::move(options)
        };
    }

    template <class InputIt>
    std::map<ValueType, int> generateFirstElementHistogram(InputIt first, InputIt last)
    {
        std::map<ValueType, int> histogram;

        for (; first != last; ++first)
        {
            const auto& el = *first;
            ++histogram[*(el.begin())];
        }

        return histogram;
    }

    template <class InputIt>
    void addSteps(InputIt first, InputIt last, int depth)
    {
        std::map<ContainerType, std::map<ValueType, int>> histograms;

        for (; first != last; ++first)
        {
            const ContainerType& el = *first;
            auto innerFirst = el.begin();
            auto innerLast = el.end();
            for (;; ++innerFirst)
            {
                const int size = std::distance(innerFirst, innerLast);
                if (size <= depth) break;

                auto innerAtDepth = std::next(innerFirst, depth);
                ++histograms[ContainerType(innerFirst, innerAtDepth)][*innerAtDepth];
            }
        }

        for (auto&& histogram : std::move(histograms))
        {
            m_steps[std::move(histogram.first)] = stepFromHistogram(std::move(histogram.second));
        }
    }
};
