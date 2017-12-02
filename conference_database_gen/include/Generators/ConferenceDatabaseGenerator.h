#pragma once

#include <vector>
#include <fstream>

#include "ConferenceDatabase/ConferenceDatabase.h"

#include "CountryTableGenerator.h"
#include "PersonTableGenerator.h"
#include "CompanyTableGenerator.h"
#include "StudentTableGenerator.h"
#include "ClientTableGenerator.h"
#include "ParticipantTableGenerator.h"
#include "ConferenceTableGenerator.h"
#include "ConferenceDayTableGenerator.h"
#include "PriceRangeTableGenerator.h"
#include "WorkshopTableGenerator.h"
#include "ConferenceDayReservationTableGenerator.h"
#include "WorkshopReservationTableGenerator.h"
#include "NameGenerator.h"
#include "MarkovChainsDictionary.h"
#include "Common.h"

class ConferenceDatabaseGenerator
{
    static constexpr int markovChainsDepth = 3;
public:
    using ResultType = ConferenceDatabase;
    using DictionaryType = MarkovChainsDictionary<std::string>;

    ConferenceDatabaseGenerator(int numFirstNames, int numLastNames, int numCountries, int numCitiesPerCountry) :
        m_numFirstNames(numFirstNames),
        m_numLastNames(numLastNames),
        m_numCountries(numCountries),
        m_numCitiesPerCountry(numCitiesPerCountry)
    {

    }

    template <class TRng>
    ConferenceDatabase operator()(TRng& rng) const
    {
        static constexpr int numPeople = 10000;
        static constexpr int numCompanies = 100;
        static constexpr int numConferences = 3 * 12 * 2;

        ConferenceDatabase database;

        auto firstNames = Common::generate<NameGenerator>(rng, m_numFirstNames, createFirstNameDictionary(), 3, 3, 9);
        auto lastNames = Common::generate<NameGenerator>(rng, m_numLastNames, createLastNameDictionary(), 4, 4, 11);

        DictionaryType addressDictionary = createAddressDictionary();
        DictionaryType postalCodeDictionary = createPostalCodeDictionary();

        std::vector<std::vector<std::string>> cityNamesByCountry;
        {
            cityNamesByCountry.resize(m_numCountries);
            DictionaryType cityDictionary = createCityDictionary();
            for (int i = 0; i < m_numCountries; ++i)
            {
                cityNamesByCountry[i] = Common::generate<NameGenerator>(rng, m_numCitiesPerCountry, cityDictionary, 4, 18, 25);
            }
        }

        auto phoneGeneratorFormats = Common::generate<PhoneNumberFormatGenerator>(rng, m_numCountries, 2, 4, 0, 4, 8, 10, 0.5f);
        std::vector<PhoneNumberGenerator> phoneGenerators;
        phoneGenerators.reserve(m_numCountries);
        for (int i = 0; i < m_numCountries; ++i)
        {
            phoneGenerators.emplace_back(phoneGeneratorFormats[i]);
        }

        DateTimeGenerator birthDateGenerator(DateTime(Years{ 1950 }), DateTime(Years{ 1998 }));

        DictionaryType countryDictionary = createCountryDictionary();
        const auto& countries = database.table<Country>() = TableGenerator<Country>(
            countryDictionary,
            m_numCountries
            )(rng);

        const auto& people = database.table<Person>() = TableGenerator<Person>(
            firstNames,
            lastNames,
            birthDateGenerator,
            addressDictionary,
            postalCodeDictionary,
            countries,
            cityNamesByCountry,
            phoneGenerators,
            numPeople
            )(rng);

        DictionaryType companyNameDictionary = createCompanyNameDictionary();

        const auto& companies = database.table<Company>() = TableGenerator<Company>(
            companyNameDictionary,
            firstNames,
            lastNames,
            addressDictionary,
            postalCodeDictionary,
            countries,
            cityNamesByCountry,
            phoneGenerators,
            numCompanies
            )(rng);

        const auto& students = database.table<Student>() = TableGenerator<Student>(
            people,
            0.1f
            )(rng);

        const auto& clients = database.table<Client>() = TableGenerator<Client>(
            people,
            companies,
            0.001f
            )(rng);

        const auto& participants = database.table<Participant>() = TableGenerator<Participant>(
            people,
            companies,
            0.95f,
            0.75f
            )(rng);

        DateTimeGenerator conferenceStartDateGenerator(DateTime(Years{ 2014 }), DateTime(Years{ 2017 }));

        const auto& conferences = database.table<Conference>() = TableGenerator<Conference>(
            clients,
            conferenceStartDateGenerator,
            numConferences
            )(rng);

        const auto& conferenceDays = database.table<ConferenceDay>() = TableGenerator<ConferenceDay>(
            conferences,
            3,
            50,
            100
            )(rng);

        const auto& priceRanges = database.table<PriceRange>() = TableGenerator<PriceRange>(
            conferenceDays,
            Price(10.0f),
            Price(50.0f),
            Days{ 70 },
            Days{14},
            0.5f,
            4
            )(rng);

        DictionaryType workshopNameDictionary = createWorkshopNameDictionary();

        const auto& workshops = database.table<Workshop>() = TableGenerator<Workshop>(
            conferenceDays,
            workshopNameDictionary,
            DurationGenerator(Minutes{30}, Hours{6}),
            DurationGenerator(Minutes{60}, Minutes{180}),
            PriceGenerator(Price(10.0f), Price(30.0f)),
            0.8f,
            4
            )(rng);

        const auto& conferenceDayReservations = database.table<ConferenceDayReservation>() = TableGenerator<ConferenceDayReservation>(
            participants,
            conferenceDays,
            priceRanges,
            students,
            0.9f,
            Days{ 14 } // same as in price ranges
            )(rng);

        const auto& workshopReservations = database.table<WorkshopReservation>() = TableGenerator<WorkshopReservation>(
            conferenceDayReservations,
            conferenceDays,
            workshops,
            0.9f
            )(rng);

        return database;
    }

private:
    int m_numFirstNames;
    int m_numLastNames;
    int m_numCountries;
    int m_numCitiesPerCountry;

    static std::vector<std::string> readTrainingData(const std::string& path)
    {
        std::vector<std::string> lines;

        std::ifstream file(path);
        for (;;)
        {
            std::string line;
            std::getline(file, line);
            if (file.eof()) break;

            lines.emplace_back(std::move(line));
        }

        return lines;
    }

    static void sortByLength(std::vector<std::string>& strings)
    {
        std::sort(strings.begin(), strings.end(), [](const std::string& lhs, const std::string& rhs) {return lhs.size() < rhs.size(); });
    }

    static DictionaryType createDictionary(const std::string& path)
    {
        std::vector<std::string> lines = readTrainingData(path);
        return DictionaryType(lines.begin(), lines.end(), markovChainsDepth);
    }
    static DictionaryType createAddressDictionary()
    {
        return createDictionary("training_data/addresses.txt");
    }
    static DictionaryType createPostalCodeDictionary()
    {
        return createDictionary("training_data/postal_codes.txt");
    }
    static DictionaryType createCompanyNameDictionary()
    {
        return createDictionary("training_data/company_names.txt");
    }
    static DictionaryType createFirstNameDictionary()
    {
        return createDictionary("training_data/first.txt");
    }
    static DictionaryType createLastNameDictionary()
    {
        return createDictionary("training_data/last.txt");
    }
    static DictionaryType createCityDictionary()
    {
        return createDictionary("training_data/cities.txt");
    }
    static DictionaryType createCountryDictionary()
    {
        return createDictionary("training_data/countries.txt");
    }
    static DictionaryType createWorkshopNameDictionary()
    {
        return createDictionary("training_data/workshop_names.txt");
    }
};
