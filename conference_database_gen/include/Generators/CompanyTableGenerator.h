#pragma once

#include <vector>

#include "ConferenceDatabase/Tables/Company.h"
#include "ConferenceDatabase/Tables/Country.h"
#include "Database/Table.h"

#include "TableGenerator.h"

#include "MarkovChainsDictionary.h"
#include "NameGenerator.h"
#include "PhoneNumberGenerator.h"
#include "Common.h"

template <>
class TableGenerator<Company>
{
public:
    using RecordType = Company;
    using ResultType = Table<Company>;
    using DictionaryType = MarkovChainsDictionary<std::string>;

    TableGenerator(
        DictionaryType& companyNameDictionary,
        const std::vector<std::string>& firstNames,
        const std::vector<std::string>& lastNames,
        const DictionaryType& addressDictionary,
        const DictionaryType& postalCodeDictionary,
        const Table<Country>& countries,
        const std::vector<std::vector<std::string>>& cityNamesByCountry,
        const std::vector<PhoneNumberGenerator>& phoneNumberGenerators, 
        int numCompanies
    );

    template <class TRng>
    Table<Company> operator()(TRng& rng) const
    {
        static constexpr int minCompanyNameLength = 4;
        static constexpr int maxOptimalCompanyNameLength = 20;
        static constexpr int maxCompanyNameLength = 30;
        static constexpr int minAddressLength = 8;
        static constexpr int maxOptimalAddressLength = 20;
        static constexpr int maxAddressLength = 30;
        static constexpr int minPostalCodeLength = 5;
        static constexpr int maxOptimalPostalCodeLength = 6;
        static constexpr int maxPostalCodeLength = 8;
        static constexpr int64_t minVatin = 100'000'000'000ll;
        static constexpr int64_t maxVatin = 999'999'999'999ll;

        NameGenerator nameGenerator(*m_companyNameDictionary, minCompanyNameLength, maxOptimalCompanyNameLength, maxCompanyNameLength);

        NameGenerator addressGenerator(*m_addressDictionary, minAddressLength, maxOptimalAddressLength, maxAddressLength);
        NameGenerator postalCodeGenerator(*m_postalCodeDictionary, minPostalCodeLength, maxOptimalPostalCodeLength, maxPostalCodeLength);

        std::uniform_int_distribution<int64_t> dVatin(minVatin, maxVatin);

        Table<Company> companies;
        companies.reserve(m_numCompanies);

        for (int i = 0; i < m_numCompanies; ++i)
        {
            const std::string& firstName = Common::chooseSqr(*m_firstNames, rng);
            const std::string& lastName = Common::chooseSqr(*m_lastNames, rng);
            const int countryIndex = Common::chooseSqrIndex(m_countries->size(), rng);
            const auto& country = (*m_countries)[countryIndex];
            const std::string& cityName = Common::chooseSqr((*m_cityNamesByCountry)[countryIndex], rng);
            std::string email = Common::assembleEmailAddress(firstName, lastName, rng);

            Company company(
                i,
                nameGenerator(rng),
                firstName + ' ' + lastName,
                std::to_string(dVatin(rng)),
                addressGenerator(rng),
                postalCodeGenerator(rng),
                cityName,
                country,
                (*m_phoneNumberGenerators)[countryIndex](rng),
                std::move(email)
            );

            companies.add(std::move(company));
        }

        return companies;
    }

private:
    const DictionaryType* m_companyNameDictionary;
    const std::vector<std::string>* m_firstNames;
    const std::vector<std::string>* m_lastNames;
    const DictionaryType* m_addressDictionary;
    const DictionaryType* m_postalCodeDictionary;
    const Table<Country>* m_countries;
    const std::vector<std::vector<std::string>>* m_cityNamesByCountry;
    const std::vector<PhoneNumberGenerator>* m_phoneNumberGenerators;
    int m_numCompanies;
};
