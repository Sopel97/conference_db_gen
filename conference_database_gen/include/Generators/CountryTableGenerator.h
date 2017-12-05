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

    struct Params
    {
        NameGenerator countryGenerator;
        int numCountries;
    };

    TableGenerator(const Params& params);

    template <class TRng>
    Table<Country> operator()(TRng& rng) const
    {
        Table<Country> countries;
        countries.reserve(m_params.numCountries);

        Record::IdType id = 0;
        for (int i = 0; i < m_params.numCountries; ++i)
        {
            countries.add(Country(id++, m_params.countryGenerator(rng)));
        }

        return countries;
    }

private:
    Params m_params;
};
