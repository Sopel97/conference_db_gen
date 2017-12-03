#pragma once

#include <vector>
#include <fstream>

#include "ConferenceDatabase/ConferenceDatabase.h"

#include "CountryTableGenerator.h"
#include "PersonTableGenerator.h"
#include "CompanyTableGenerator.h"
#include "StudentTableGenerator.h"
#include "CustomerTableGenerator.h"
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

    ConferenceDatabaseGenerator(int numFirstNames, int numLastNames, int numCountries, int numCitiesPerCountry);

    template <class TRng>
    ConferenceDatabase operator()(TRng& rng) const
    {
        static constexpr int numPeople = 10000;
        static constexpr int numCompanies = 100;

        static constexpr int minConferenceYear = 2014;
        static constexpr int maxConferenceYear = 2017;
        static constexpr int years = maxConferenceYear - minConferenceYear;
        static constexpr int numConferencesPerMonth = 2;
        static constexpr int numConferences = years * 12 * numConferencesPerMonth;

        static constexpr int minFirstNameLength = 3;
        static constexpr int maxFirstNameLength = 9;
        static constexpr int minLastNameLength = 4;
        static constexpr int maxLastNameLength = 11;

        static constexpr int minCityNameLength = 4;
        static constexpr int maxOptimalCityNameLength = 18;
        static constexpr int maxCityNameLength = 25;

        ConferenceDatabase database;

        auto firstNames = Common::generate<NameGenerator>(rng, m_numFirstNames, createFirstNameDictionary(), minFirstNameLength, minFirstNameLength, maxFirstNameLength);
        auto lastNames = Common::generate<NameGenerator>(rng, m_numLastNames, createLastNameDictionary(), minLastNameLength, minLastNameLength, maxLastNameLength);

        DictionaryType addressDictionary = createAddressDictionary();
        DictionaryType postalCodeDictionary = createPostalCodeDictionary();

        std::vector<std::vector<std::string>> cityNamesByCountry;
        {
            cityNamesByCountry.resize(m_numCountries);
            DictionaryType cityDictionary = createCityDictionary();
            for (int i = 0; i < m_numCountries; ++i)
            {
                cityNamesByCountry[i] = Common::generate<NameGenerator>(rng, m_numCitiesPerCountry, cityDictionary, minCityNameLength, maxOptimalCityNameLength, maxCityNameLength);
            }
        }

        static constexpr int minPhoneSegmentLength = 2;
        static constexpr int maxPhoneSegmentLength = 4;
        static constexpr int minAreaCodeLength = 0;
        static constexpr int maxAreaCodeLength = 4;
        static constexpr int minPhoneNumberLength = 8;
        static constexpr int maxPhoneNumberLength = 10;
        static constexpr float phoneNumberSeparatorBias = 0.5f;

        auto phoneGeneratorFormats = Common::generate<PhoneNumberFormatGenerator>(
            rng, 
            m_numCountries, 
            minPhoneSegmentLength,
            maxPhoneSegmentLength,
            minAreaCodeLength,
            maxAreaCodeLength,
            minPhoneNumberLength,
            maxPhoneNumberLength,
            phoneNumberSeparatorBias
            );
        std::vector<PhoneNumberGenerator> phoneGenerators;
        phoneGenerators.reserve(m_numCountries);
        for (int i = 0; i < m_numCountries; ++i)
        {
            phoneGenerators.emplace_back(phoneGeneratorFormats[i]);
        }

        static constexpr DateTime minBirthDate = DateTime(Years{ 1950 });
        static constexpr DateTime maxBirthDate = DateTime(Years{ 1998 });

        DateTimeGenerator birthDateGenerator(minBirthDate, maxBirthDate);

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

        static constexpr float studentSaturation = 0.1f;

        const auto& students = database.table<Student>() = TableGenerator<Student>(
            people,
            studentSaturation
            )(rng);

        static constexpr float personalCustomersSaturation = 0.001f;

        const auto& customers = database.table<Customer>() = TableGenerator<Customer>(
            people,
            companies,
            personalCustomersSaturation
            )(rng);

        static constexpr float participantSaturation = 0.95f;
        static constexpr float participantsWithCompanySaturation = 0.75f;

        const auto& participants = database.table<Participant>() = TableGenerator<Participant>(
            people,
            companies,
            participantSaturation,
            participantsWithCompanySaturation
            )(rng);

        DateTimeGenerator conferenceStartDateGenerator(DateTime(Years{ minConferenceYear }), DateTime(Years{ maxConferenceYear }));

        const auto& conferences = database.table<Conference>() = TableGenerator<Conference>(
            customers,
            conferenceStartDateGenerator,
            numConferences
            )(rng);

        static constexpr int avgNumConferenceDays = 3;
        static constexpr int minConferenceDaySpots = 50;
        static constexpr int maxConferenceDaySpots = 100;

        const auto& conferenceDays = database.table<ConferenceDay>() = TableGenerator<ConferenceDay>(
            conferences,
            avgNumConferenceDays,
            minConferenceDaySpots,
            maxConferenceDaySpots
            )(rng);

        static constexpr float minConferenceDayPrice = 10.0f;
        static constexpr float maxConferenceDayPrice = 50.0f;
        static constexpr Milliseconds availableBefore = Days{ 70 };
        static constexpr Milliseconds paymentIncreaseAfter = Days{ 14 };
        static constexpr float percentPaymentIncrease = 0.5f;
        static constexpr int numPaymentRangesPerDay = 4;

        const auto& priceRanges = database.table<PriceRange>() = TableGenerator<PriceRange>(
            conferenceDays,
            Price(minConferenceDayPrice),
            Price(maxConferenceDayPrice),
            availableBefore,
            paymentIncreaseAfter,
            percentPaymentIncrease,
            numPaymentRangesPerDay
            )(rng);

        DictionaryType workshopNameDictionary = createWorkshopNameDictionary();

        static constexpr Milliseconds minWorkshopOffsetFromDayStart = Minutes{ 30 };
        static constexpr Milliseconds maxWorkshopOffsetFromDayStart = Hours{ 6 };
        static constexpr Milliseconds minWorkshopDuration = Minutes{ 60 };
        static constexpr Milliseconds maxWorkshopDuration = Minutes{ 180 };
        static constexpr float minWorkshopPrice = 10.0f;
        static constexpr float maxWorkshopPrice = 30.0f;
        static constexpr float workshopPriceSaturation = 0.8f;
        static constexpr int avgNumWorkshopsPerDay = 4;

        const auto& workshops = database.table<Workshop>() = TableGenerator<Workshop>(
            conferenceDays,
            workshopNameDictionary,
            DurationGenerator(minWorkshopOffsetFromDayStart, maxWorkshopOffsetFromDayStart),
            DurationGenerator(minWorkshopDuration, maxWorkshopDuration),
            PriceGenerator(Price(minWorkshopPrice), Price(maxWorkshopPrice)),
            workshopPriceSaturation,
            avgNumWorkshopsPerDay
            )(rng);

        static constexpr float conferenceDayPaymentSaturation = 0.9f;

        const auto& conferenceDayReservations = database.table<ConferenceDayReservation>() = TableGenerator<ConferenceDayReservation>(
            participants,
            conferenceDays,
            priceRanges,
            students,
            conferenceDayPaymentSaturation,
            paymentIncreaseAfter // same as in price ranges
            )(rng);

        static constexpr float workshopPaymentSaturation = 0.9f;

        const auto& workshopReservations = database.table<WorkshopReservation>() = TableGenerator<WorkshopReservation>(
            conferenceDayReservations,
            conferenceDays,
            workshops,
            workshopPaymentSaturation
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
