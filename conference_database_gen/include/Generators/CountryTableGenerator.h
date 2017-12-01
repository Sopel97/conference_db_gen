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
        auto countryNames = Common::generate<NameGenerator>(rng, m_numCountries, *m_countryDictionary, 5, 7, 15);

        Table<Country> countries;

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
