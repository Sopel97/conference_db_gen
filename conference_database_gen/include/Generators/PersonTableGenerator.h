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

    TableGenerator(
        const std::vector<std::string>& firstNames,
        const std::vector<std::string>& lastNames,
        const DateTimeGenerator& birthDateGenerator,
        const DictionaryType& addressDictionary,
        const DictionaryType& postalCodeDictionary,
        const Table<Country>& countries,
        const std::vector<std::vector<std::string>>& cityNamesByCountry,
        const std::vector<PhoneNumberGenerator>& phoneNumberGenerators,
        int numRecords
    );

    template <class TRng>
    Table<Person> operator()(TRng& rng) const
    {
        NameGenerator addressGenerator(*m_addressDictionary, 8, 20, 30);
        NameGenerator postalCodeGenerator(*m_postalCodeDictionary, 5, 6, 8);

        Table<Person> people;

        for (int i = 0; i < m_numRecords; ++i)
        {
            const std::string& firstName = Common::chooseSqr(*m_firstNames, rng);
            const std::string& lastName = Common::chooseSqr(*m_lastNames, rng);
            const int countryIndex = Common::chooseSqrIndex(m_countries->size(), rng);
            const auto& country = (*m_countries)[countryIndex];
            const std::string& cityName = Common::chooseSqr((*m_cityNamesByCountry)[countryIndex], rng);
            std::string email = Common::assembleEmailAddress(firstName, lastName, rng);

            Person person(
                i,
                firstName,
                lastName,
                m_birthDateGenerator(rng).rounded(Days{ 1 }),
                addressGenerator(rng),
                postalCodeGenerator(rng),
                cityName,
                country,
                (*m_phoneNumberGenerators)[countryIndex](rng),
                std::move(email)
            );

            people.add(std::move(person));
        }

        return people;
    }

private:
    const std::vector<std::string>* m_firstNames;
    const std::vector<std::string>* m_lastNames;
    DateTimeGenerator m_birthDateGenerator;
    const DictionaryType* m_addressDictionary;
    const DictionaryType* m_postalCodeDictionary;
    const Table<Country>* m_countries;
    const std::vector<std::vector<std::string>>* m_cityNamesByCountry;
    const std::vector<PhoneNumberGenerator>* m_phoneNumberGenerators;
    int m_numRecords;
};
