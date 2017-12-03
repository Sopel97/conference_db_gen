#pragma once

#include <vector>

#include "ConferenceDatabase/Tables/Country.h"
#include "Database/Table.h"

#include "TableGenerator.h"

#include "MarkovChainsDictionary.h"
#include "NameGenerator.h"
#include "Common.h"

template <>
class TableGenerator<Country>
{
public:
    using RecordType = Country;
    using ResultType = Table<Country>;
    using DictionaryType = MarkovChainsDictionary<std::string>;

    TableGenerator(
        DictionaryType& countryDictionary,
        int numCountries
    );

    template <class TRng>
    Table<Country> operator()(TRng& rng) const
    {
        static constexpr int minCountryLength = 5;
        static constexpr int maxOptimalCountryLength = 7;
        static constexpr int maxCountryLength = 15;

        auto countryNames = Common::generate<NameGenerator>(rng, m_numCountries, *m_countryDictionary, minCountryLength, maxOptimalCountryLength, maxCountryLength);

        Table<Country> countries;
        countries.reserve(m_numCountries);

        Record::IdType id = 0;
        for (auto&& countryName : std::move(countryNames))
        {
            countries.add(Country(id++, std::move(countryName)));
        }

        return countries;
    }

private:
    const DictionaryType* m_countryDictionary;
    int m_numCountries;
};
