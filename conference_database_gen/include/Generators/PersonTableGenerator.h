#pragma once

#include <vector>

#include "ConferenceDatabase/Tables/Person.h"
#include "ConferenceDatabase/Tables/Country.h"
#include "Database/Table.h"

#include "TableGenerator.h"

#include "DateTimeGenerator.h"
#include "NameGenerator.h"
#include "PhoneNumberGenerator.h"
#include "PhoneNumberFormatGenerator.h"
#include "Common.h"

template <>
class TableGenerator<Person>
{
public:
    using RecordType = Person;
    using ResultType = Table<Person>;
    using DictionaryType = MarkovChainsDictionary<std::string>;

    struct Params
    {
        const std::vector<std::string>* firstNames;
        const std::vector<std::string>* lastNames;
        DateTimeGenerator birthDateGenerator;
        NameGenerator addressGenerator;
        NameGenerator postalCodeGenerator;
        const Table<Country>* countries;
        const std::vector<std::vector<std::string>>* cityNamesByCountry;
        const std::vector<PhoneNumberGenerator>* phoneNumberGenerators;
        Milliseconds dateRounding;
        int numRecords;
    };

    TableGenerator(const Params& params);

    template <class TRng>
    Table<Person> operator()(TRng& rng) const
    {
        Table<Person> people;
        people.reserve(m_params.numRecords);

        for (int i = 0; i < m_params.numRecords; ++i)
        {
            const std::string& firstName = Common::chooseSqr(*m_params.firstNames, rng);
            const std::string& lastName = Common::chooseSqr(*m_params.lastNames, rng);
            const int countryIndex = Common::chooseSqrIndex(m_params.countries->size(), rng);
            const auto& country = (*m_params.countries)[countryIndex];
            const std::string& cityName = Common::chooseSqr((*m_params.cityNamesByCountry)[countryIndex], rng);
            std::string email = Common::assembleEmailAddress(firstName, lastName, rng);

            Person person(
                i,
                firstName,
                lastName,
                m_params.birthDateGenerator(rng).rounded(m_params.dateRounding),
                m_params.addressGenerator(rng),
                m_params.postalCodeGenerator(rng),
                cityName,
                country,
                (*m_params.phoneNumberGenerators)[countryIndex](rng),
                std::move(email)
            );

            people.add(std::move(person));
        }

        return people;
    }

private:
    Params m_params;
};
