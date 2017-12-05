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

    struct Params
    {
        NameGenerator companyNameGenerator;
        const std::vector<std::string>* firstNames;
        const std::vector<std::string>* lastNames;
        NameGenerator addressGenerator;
        NameGenerator postalCodeGenerator;
        const Table<Country>* countries;
        const std::vector<std::vector<std::string>>* cityNamesByCountry;
        const std::vector<PhoneNumberGenerator>* phoneNumberGenerators;
        int64_t minVatin;
        int64_t maxVatin;
        int numCompanies;
    };

    TableGenerator(const Params& params);

    template <class TRng>
    Table<Company> operator()(TRng& rng) const
    {
        std::uniform_int_distribution<int64_t> dVatin(m_params.minVatin, m_params.maxVatin);

        Table<Company> companies;
        companies.reserve(m_params.numCompanies);

        for (int i = 0; i < m_params.numCompanies; ++i)
        {
            const std::string& firstName = Common::chooseSqr(*m_params.firstNames, rng);
            const std::string& lastName = Common::chooseSqr(*m_params.lastNames, rng);
            const int countryIndex = Common::chooseSqrIndex(m_params.countries->size(), rng);
            const auto& country = (*m_params.countries)[countryIndex];
            const std::string& cityName = Common::chooseSqr((*m_params.cityNamesByCountry)[countryIndex], rng);
            std::string email = Common::assembleEmailAddress(firstName, lastName, rng);

            Company company(
                i,
                m_params.companyNameGenerator(rng),
                firstName + ' ' + lastName,
                std::to_string(dVatin(rng)),
                m_params.addressGenerator(rng),
                m_params.postalCodeGenerator(rng),
                cityName,
                country,
                (*m_params.phoneNumberGenerators)[countryIndex](rng),
                std::move(email)
            );

            companies.add(std::move(company));
        }

        return companies;
    }

private:
    Params m_params;
};
