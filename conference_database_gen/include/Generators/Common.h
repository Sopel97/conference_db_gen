#pragma once

#include <string>
#include <random>
#include <cctype>

class Common
{
public:
    template <class TRng>
    static std::string assembleEmailAddress(const std::string& firstName, const std::string& lastName, TRng& rng)
    {
        std::uniform_int_distribution<int> dNumber(0, 9999);

        std::string email = firstName + '.' + lastName + std::to_string(dNumber(rng)) + "@mail.com";
        email[0] = std::tolower(email[0]);
        email[firstName.size() + 1] = std::tolower(email[firstName.size() + 1]);

        return email;
    }

    template <class TRng>
    static int chooseSqrIndex(int size, TRng& rng)
    {
        double t = std::generate_canonical<double, std::numeric_limits<double>::digits>(rng);
        t *= t;
        return static_cast<int>(t * size);
    }

    template <class TRng, class TResult>
    static const TResult& chooseSqr(const std::vector<TResult>& possibleResults, TRng& rng)
    {
        return possibleResults[chooseSqrIndex(possibleResults.size(), rng)];
    }

    template <class TGenerator, class TRng, class... Args, class TResult = typename TGenerator::ResultType>
    static std::vector<TResult> generate(TRng rng, int count, Args&&... args)
    {
        TGenerator generator(std::forward<Args>(args)...);
        std::vector<std::string> result;
        result.reserve(count);
        for (int i = 0; i < count; ++i)
        {
            result.emplace_back(generator(rng));
        }

        return result;
    }
};
