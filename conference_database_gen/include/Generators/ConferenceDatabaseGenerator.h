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
#include "ConferenceDayEarlyReservationTableGenerator.h"
#include "ConferenceDayReservationTableGenerator.h"
#include "WorkshopEarlyReservationTableGenerator.h"
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

    struct Params
    {
        int numFirstNames;
        int numLastNames;
        int numCountries;
        int numCitiesPerCountry;

        int numPeople;
        int numCompanies;

        int minConferenceNameLength;
        int maxOptimalConferenceNameLength;
        int maxConferenceNameLength;

        Years minConferenceYear;
        Years maxConferenceYear;
        int avgNumConferencesPerMonth;

        int minFirstNameLength;
        int maxFirstNameLength;
        int minLastNameLength;
        int maxLastNameLength;

        int minCityNameLength;
        int maxOptimalCityNameLength;
        int maxCityNameLength;

        int minAddressLength;
        int maxOptimalAddressLength;
        int maxAddressLength;

        int minPostalCodeLength;
        int maxOptimalPostalCodeLength;
        int maxPostalCodeLength;

        int minPhoneSegmentLength;
        int maxPhoneSegmentLength;
        int minAreaCodeLength;
        int maxAreaCodeLength;
        int minPhoneNumberLength;
        int maxPhoneNumberLength;
        float phoneNumberSeparatorBias;

        DateTime minBirthDate;
        DateTime maxBirthDate;

        int minCountryLength;
        int maxOptimalCountryLength;
        int maxCountryLength;

        Milliseconds personBirthDateRounding;

        int minCompanyNameLength;
        int maxOptimalCompanyNameLength;
        int maxCompanyNameLength;

        int64_t minVatin;
        int64_t maxVatin;

        float studentSaturation;

        int minStudentCardNumber;
        int maxStudentCardNumber;

        float individualCustomersSaturation;

        float participantSaturation;
        float participantsWithCompanySaturation;

        Milliseconds conferenceStartDateRounding;

        int avgNumConferenceDays;
        int minConferenceDaySpots;
        int maxConferenceDaySpots;

        float percentConferenceDaySpotsVariation;
        Milliseconds minConferenceDayStartOffset;
        Milliseconds maxConferenceDayStartOffset;
        Milliseconds conferenceDayStartDateRounding;

        Price minConferenceDayPrice;
        Price maxConferenceDayPrice;
        Milliseconds availableBefore;
        Milliseconds paymentIncreaseAfter;
        float percentPaymentIncrease;
        int numPaymentRangesPerDay;

        int minWorkshopNameLength;
        int maxOptimalWorkshopNameLength;
        int maxWorkshopNameLength;

        Milliseconds minWorkshopOffsetFromDayStart;
        Milliseconds maxWorkshopOffsetFromDayStart;
        Milliseconds minWorkshopDuration;
        Milliseconds maxWorkshopDuration;
        Price minWorkshopPrice;
        Price maxWorkshopPrice;
        float workshopPriceSaturation;
        int avgNumWorkshopsPerDay;

        float minNumWorkshopSpotsRelativeToConferenceDay;
        float maxNumWorkshopSpotsRelativeToConferenceDay;
        Milliseconds workshopStartDateRounding;

        float percentConferenceDayFillFromEarlyReservations;
        int avgEarlyReservationsPerConferenceDay;

        float conferenceDayPaymentSaturation;
        float percentFillPerConferenceDayPriceRange;
        float studentConferenceDayDiscount;
        Milliseconds maxConferenceDayReservationTimeOffset;

        int avgNumWorkshopsPerPerson;

        float workshopPaymentSaturation;
        Milliseconds minOffsetFromConferenceDayReservation;
        Milliseconds maxOffsetFromConferenceDayReservation;
    };

    ConferenceDatabaseGenerator(const Params& params);

    template <class TRng>
    ConferenceDatabase operator()(TRng& rng) const
    {
        const int conferenceYears = static_cast<int>(m_params.maxConferenceYear.count() - m_params.minConferenceYear.count());
        const int numConferences = m_params.avgNumConferencesPerMonth * conferenceYears * 12;

        ConferenceDatabase database;

        auto firstNames = Common::generate<NameGenerator>(
            rng, 
            m_params.numFirstNames, 
            createFirstNameDictionary(), 
            m_params.minFirstNameLength, 
            m_params.minFirstNameLength,
            m_params.maxFirstNameLength
            );

        auto lastNames = Common::generate<NameGenerator>(
            rng, 
            m_params.numLastNames, 
            createLastNameDictionary(), 
            m_params.minLastNameLength, 
            m_params.minLastNameLength, 
            m_params.maxLastNameLength
            );

        DictionaryType addressDictionary = createAddressDictionary();
        NameGenerator addressGenerator(addressDictionary, m_params.minAddressLength, m_params.maxOptimalAddressLength, m_params.maxAddressLength);

        DictionaryType postalCodeDictionary = createPostalCodeDictionary();
        NameGenerator postalCodeGenerator(postalCodeDictionary, m_params.minPostalCodeLength, m_params.maxOptimalPostalCodeLength, m_params.maxPostalCodeLength);

        std::vector<std::vector<std::string>> cityNamesByCountry;
        {
            cityNamesByCountry.resize(m_params.numCountries);
            DictionaryType cityDictionary = createCityDictionary();
            for (int i = 0; i < m_params.numCountries; ++i)
            {
                cityNamesByCountry[i] = Common::generate<NameGenerator>(
                    rng, 
                    m_params.numCitiesPerCountry, 
                    cityDictionary, 
                    m_params.minCityNameLength, 
                    m_params.maxOptimalCityNameLength, 
                    m_params.maxCityNameLength
                    );
            }
        }

        auto phoneGeneratorFormats = Common::generate<PhoneNumberFormatGenerator>(
            rng,
            m_params.numCountries,
            m_params.minPhoneSegmentLength,
            m_params.maxPhoneSegmentLength,
            m_params.minAreaCodeLength,
            m_params.maxAreaCodeLength,
            m_params.minPhoneNumberLength,
            m_params.maxPhoneNumberLength,
            m_params.phoneNumberSeparatorBias
            );

        std::vector<PhoneNumberGenerator> phoneGenerators;
        phoneGenerators.reserve(m_params.numCountries);
        for (int i = 0; i < m_params.numCountries; ++i)
        {
            phoneGenerators.emplace_back(phoneGeneratorFormats[i]);
        }

        DateTimeGenerator birthDateGenerator(m_params.minBirthDate, m_params.maxBirthDate);

        DictionaryType countryDictionary = createCountryDictionary();
        NameGenerator countryGenerator(countryDictionary, m_params.minCountryLength, m_params.maxOptimalCountryLength, m_params.maxCountryLength);

        const auto& countries = database.table<Country>() = TableGenerator<Country>({
            countryGenerator,
            m_params.numCountries
        })(rng);

        const auto& people = database.table<Person>() = TableGenerator<Person>({
            &firstNames,
            &lastNames,
            birthDateGenerator,
            addressGenerator,
            postalCodeGenerator,
            &countries,
            &cityNamesByCountry,
            &phoneGenerators,
            m_params.personBirthDateRounding,
            m_params.numPeople
        })(rng);

        DictionaryType companyNameDictionary = createCompanyNameDictionary();
        NameGenerator companyNameGenerator(companyNameDictionary, m_params.minCompanyNameLength, m_params.maxOptimalCompanyNameLength, m_params.maxCompanyNameLength);

        const auto& companies = database.table<Company>() = TableGenerator<Company>({
            companyNameGenerator,
            &firstNames,
            &lastNames,
            addressGenerator,
            postalCodeGenerator,
            &countries,
            &cityNamesByCountry,
            &phoneGenerators,
            m_params.minVatin,
            m_params.maxVatin,
            m_params.numCompanies
        })(rng);

        const auto& students = database.table<Student>() = TableGenerator<Student>({
            &people,
            m_params.studentSaturation,
            m_params.minStudentCardNumber,
            m_params.maxStudentCardNumber
        })(rng);

        const auto& customers = database.table<Customer>() = TableGenerator<Customer>({
            &people,
            &companies,
            m_params.individualCustomersSaturation
        })(rng);

        const auto& participants = database.table<Participant>() = TableGenerator<Participant>({
            &people,
            &companies,
            m_params.participantSaturation,
            m_params.participantsWithCompanySaturation
        })(rng);

        DictionaryType conferenceNameDictionary = createConferenceNameDictionary();
        NameGenerator conferenceNameGenerator(conferenceNameDictionary, m_params.minConferenceNameLength, m_params.maxOptimalConferenceNameLength, m_params.maxConferenceNameLength);

        DateTimeGenerator conferenceStartDateGenerator(DateTime(m_params.minConferenceYear), DateTime(m_params.maxConferenceYear));

        const auto& conferences = database.table<Conference>() = TableGenerator<Conference>({
            &customers,
            conferenceNameGenerator,
            conferenceStartDateGenerator,
            numConferences,
            m_params.conferenceStartDateRounding
        })(rng);

        const auto& conferenceDays = database.table<ConferenceDay>() = TableGenerator<ConferenceDay>({
            &conferences,
            m_params.avgNumConferenceDays,
            m_params.minConferenceDaySpots,
            m_params.maxConferenceDaySpots,
            m_params.percentConferenceDaySpotsVariation,
            m_params.minConferenceDayStartOffset,
            m_params.maxConferenceDayStartOffset,
            m_params.conferenceDayStartDateRounding
        })(rng);

        const auto& priceRanges = database.table<PriceRange>() = TableGenerator<PriceRange>({
            &conferenceDays,
            m_params.minConferenceDayPrice,
            m_params.maxConferenceDayPrice,
            m_params.availableBefore,
            m_params.paymentIncreaseAfter,
            m_params.percentPaymentIncrease,
            m_params.numPaymentRangesPerDay
        })(rng);

        DictionaryType workshopNameDictionary = createWorkshopNameDictionary();
        NameGenerator workshopNameGenerator(workshopNameDictionary, m_params.minWorkshopNameLength, m_params.maxOptimalWorkshopNameLength, m_params.maxWorkshopNameLength);

        const auto& workshops = database.table<Workshop>() = TableGenerator<Workshop>({
            &conferenceDays,
            workshopNameGenerator,
            DurationGenerator(m_params.minWorkshopOffsetFromDayStart, m_params.maxWorkshopOffsetFromDayStart),
            DurationGenerator(m_params.minWorkshopDuration, m_params.maxWorkshopDuration),
            PriceGenerator(m_params.minWorkshopPrice, m_params.maxWorkshopPrice),
            m_params.workshopPriceSaturation,
            m_params.avgNumWorkshopsPerDay,
            m_params.minNumWorkshopSpotsRelativeToConferenceDay,
            m_params.maxNumWorkshopSpotsRelativeToConferenceDay,
            m_params.workshopStartDateRounding
        })(rng);

        const auto& conferenceDayEarlyReservations = database.table<ConferenceDayEarlyReservation>() = TableGenerator<ConferenceDayEarlyReservation>({
            &conferenceDays,
            &companies,
            m_params.percentConferenceDayFillFromEarlyReservations,
            m_params.avgEarlyReservationsPerConferenceDay
        })(rng);

        const auto& conferenceDayReservations = database.table<ConferenceDayReservation>() = TableGenerator<ConferenceDayReservation>({
            &participants,
            &priceRanges,
            &students,
            &conferenceDayEarlyReservations,
            m_params.conferenceDayPaymentSaturation,
            m_params.paymentIncreaseAfter, // same as in price ranges
            m_params.percentFillPerConferenceDayPriceRange,
            m_params.studentConferenceDayDiscount,
            m_params.maxConferenceDayReservationTimeOffset
        })(rng);

        const auto& workshopEarlyReservations = database.table<WorkshopEarlyReservation>() = TableGenerator<WorkshopEarlyReservation>({
            &conferenceDayEarlyReservations,
            &workshops,
            m_params.avgNumWorkshopsPerPerson
        })(rng);

        const auto& workshopReservations = database.table<WorkshopReservation>() = TableGenerator<WorkshopReservation>({
            &conferenceDayReservations,
            &workshopEarlyReservations,
            m_params.workshopPaymentSaturation,
            m_params.minOffsetFromConferenceDayReservation,
            m_params.maxOffsetFromConferenceDayReservation
        })(rng);

        return database;
    }

private:
    Params m_params;

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
    static DictionaryType createConferenceNameDictionary()
    {
        return createDictionary("training_data/conference_names.txt");
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
