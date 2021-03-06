#include <iostream>
#include <random>
#include <filesystem>

#include "ConferenceDatabase/Tables/Person.h"
#include "Database/Table.h"
#include "ConferenceDatabase/ConferenceDatabase.h"

#include "Generators/ConferenceDatabaseGenerator.h"

#include "Generators/MarkovChainsDictionary.h"
#include "Generators/DateTimeGenerator.h"

#include "Rng/xoroshiro.h"

#include "Json/Document.h"
#include "Json/Parser.h"
#include "Json/Value.h"

void saveCsv(const std::string& path, const CsvDocument& csv)
{
    std::ofstream file(path);

    file << csv;
}

Milliseconds readDuration(const ls::json::Value& value)
{
    Milliseconds millis{ 0 };

    millis += Milliseconds{ value["milliseconds"].getIntOr(0) };
    millis += Seconds{ value["seconds"].getIntOr(0) };
    millis += Minutes{ value["minutes"].getIntOr(0) };
    millis += Hours{ value["hours"].getIntOr(0) };
    millis += Days{ value["days"].getIntOr(0) };

    return millis;
}

DateTime readDateTime(const ls::json::Value& value)
{
    DateTime dt(
        value["year"].getInt(),
        value["month"].getIntOr(0),
        value["day"].getIntOr(0)
        );

    return dt + readDuration(value);
}

ConferenceDatabaseGenerator::Params loadSettings(const ls::json::Document& doc)
{
    ConferenceDatabaseGenerator::Params params;

    params.numFirstNames = static_cast<int>(doc["numFirstNames"].getInt());
    params.numLastNames = static_cast<int>(doc["numLastNames"].getInt());
    params.numCountries = static_cast<int>(doc["numCountries"].getInt());
    params.numCitiesPerCountry = static_cast<int>(doc["numCitiesPerCountry"].getInt());

    params.numPeople = static_cast<int>(doc["numPeople"].getInt());
    params.numCompanies = static_cast<int>(doc["numCompanies"].getInt());

    params.minConferenceNameLength = static_cast<int>(doc["minConferenceNameLength"].getInt());
    params.maxOptimalConferenceNameLength = static_cast<int>(doc["maxOptimalConferenceNameLength"].getInt());
    params.maxConferenceNameLength = static_cast<int>(doc["maxConferenceNameLength"].getInt());

    params.minConferenceYear = Years{ doc["minConferenceYear"].getInt() };
    params.maxConferenceYear = Years{ doc["maxConferenceYear"].getInt() };
    params.avgNumConferencesPerMonth = static_cast<int>(doc["avgNumConferencesPerMonth"].getInt());

    params.minFirstNameLength = static_cast<int>(doc["minFirstNameLength"].getInt());
    params.maxFirstNameLength = static_cast<int>(doc["maxFirstNameLength"].getInt());
    params.minLastNameLength = static_cast<int>(doc["minLastNameLength"].getInt());
    params.maxLastNameLength = static_cast<int>(doc["maxLastNameLength"].getInt());

    params.minCityNameLength = static_cast<int>(doc["minCityNameLength"].getInt());
    params.maxOptimalCityNameLength = static_cast<int>(doc["maxOptimalCityNameLength"].getInt());
    params.maxCityNameLength = static_cast<int>(doc["maxCityNameLength"].getInt());

    params.minAddressLength = static_cast<int>(doc["minAddressLength"].getInt());
    params.maxOptimalAddressLength = static_cast<int>(doc["maxOptimalAddressLength"].getInt());
    params.maxAddressLength = static_cast<int>(doc["maxAddressLength"].getInt());

    params.minPostalCodeLength = static_cast<int>(doc["minPostalCodeLength"].getInt());
    params.maxOptimalPostalCodeLength = static_cast<int>(doc["maxOptimalPostalCodeLength"].getInt());
    params.maxPostalCodeLength = static_cast<int>(doc["maxPostalCodeLength"].getInt());

    params.minPhoneSegmentLength = static_cast<int>(doc["minPhoneSegmentLength"].getInt());
    params.maxPhoneSegmentLength = static_cast<int>(doc["maxPhoneSegmentLength"].getInt());
    params.minAreaCodeLength = static_cast<int>(doc["minAreaCodeLength"].getInt());
    params.maxAreaCodeLength = static_cast<int>(doc["maxAreaCodeLength"].getInt());
    params.minPhoneNumberLength = static_cast<int>(doc["minPhoneNumberLength"].getInt());
    params.maxPhoneNumberLength = static_cast<int>(doc["maxPhoneNumberLength"].getInt());
    params.phoneNumberSeparatorBias = static_cast<float>(doc["phoneNumberSeparatorBias"].getDouble());

    params.minBirthDate = readDateTime(doc["minBirthDate"]);
    params.maxBirthDate = readDateTime(doc["maxBirthDate"]);

    params.minCountryLength = static_cast<int>(doc["minCountryLength"].getInt());
    params.maxOptimalCountryLength = static_cast<int>(doc["maxOptimalCountryLength"].getInt());
    params.maxCountryLength = static_cast<int>(doc["maxCountryLength"].getInt());

    params.personBirthDateRounding = readDuration(doc["personBirthDateRounding"]);

    params.minCompanyNameLength = static_cast<int>(doc["minCompanyNameLength"].getInt());
    params.maxOptimalCompanyNameLength = static_cast<int>(doc["maxOptimalCompanyNameLength"].getInt());
    params.maxCompanyNameLength = static_cast<int>(doc["maxCompanyNameLength"].getInt());

    params.minVatin = doc["minVatin"].getInt();
    params.maxVatin = doc["maxVatin"].getInt();

    params.studentSaturation = static_cast<float>(doc["studentSaturation"].getDouble());

    params.minStudentCardNumber = static_cast<int>(doc["minStudentCardNumber"].getInt());
    params.maxStudentCardNumber = static_cast<int>(doc["maxStudentCardNumber"].getInt());

    params.individualCustomersSaturation = static_cast<float>(doc["individualCustomersSaturation"].getDouble());

    params.participantSaturation = static_cast<float>(doc["participantSaturation"].getDouble());
    params.participantsWithCompanySaturation = static_cast<float>(doc["participantsWithCompanySaturation"].getDouble());

    params.conferenceStartDateRounding = readDuration(doc["conferenceStartDateRounding"]);

    params.avgNumConferenceDays = static_cast<int>(doc["avgNumConferenceDays"].getInt());
    params.minConferenceDaySpots = static_cast<int>(doc["minConferenceDaySpots"].getInt());
    params.maxConferenceDaySpots = static_cast<int>(doc["maxConferenceDaySpots"].getInt());

    params.percentConferenceDaySpotsVariation = static_cast<float>(doc["percentConferenceDaySpotsVariation"].getDouble());
    params.minConferenceDayStartOffset = readDuration(doc["minConferenceDayStartOffset"]);
    params.maxConferenceDayStartOffset = readDuration(doc["maxConferenceDayStartOffset"]);
    params.conferenceDayStartDateRounding = readDuration(doc["conferenceDayStartDateRounding"]);

    params.minConferenceDayPrice = Price(doc["minConferenceDayPrice"].getDouble());
    params.maxConferenceDayPrice = Price(doc["maxConferenceDayPrice"].getDouble());
    params.availableBefore = readDuration(doc["availableBefore"]);
    params.paymentIncreaseAfter = readDuration(doc["paymentIncreaseAfter"]);
    params.percentPaymentIncrease = static_cast<float>(doc["percentPaymentIncrease"].getDouble());
    params.numPaymentRangesPerDay = static_cast<int>(doc["numPaymentRangesPerDay"].getInt());

    params.minWorkshopNameLength = static_cast<int>(doc["minWorkshopNameLength"].getInt());
    params.maxOptimalWorkshopNameLength = static_cast<int>(doc["maxOptimalWorkshopNameLength"].getInt());
    params.maxWorkshopNameLength = static_cast<int>(doc["maxWorkshopNameLength"].getInt());

    params.minWorkshopOffsetFromDayStart = readDuration(doc["minWorkshopOffsetFromDayStart"]);
    params.maxWorkshopOffsetFromDayStart = readDuration(doc["maxWorkshopOffsetFromDayStart"]);
    params.minWorkshopDuration = readDuration(doc["minWorkshopDuration"]);
    params.maxWorkshopDuration = readDuration(doc["maxWorkshopDuration"]);
    params.minWorkshopPrice = Price(doc["minWorkshopPrice"].getDouble());
    params.maxWorkshopPrice = Price(doc["maxWorkshopPrice"].getDouble());
    params.workshopPriceSaturation = static_cast<float>(doc["workshopPriceSaturation"].getDouble());
    params.avgNumWorkshopsPerDay = static_cast<int>(doc["avgNumWorkshopsPerDay"].getInt());

    params.minNumWorkshopSpotsRelativeToConferenceDay = static_cast<float>(doc["minNumWorkshopSpotsRelativeToConferenceDay"].getDouble());
    params.maxNumWorkshopSpotsRelativeToConferenceDay = static_cast<float>(doc["maxNumWorkshopSpotsRelativeToConferenceDay"].getDouble());
    params.workshopStartDateRounding = readDuration(doc["workshopStartDateRounding"]);

    params.percentConferenceDayFillFromEarlyReservations = static_cast<float>(doc["percentConferenceDayFillFromEarlyReservations"].getDouble());
    params.avgEarlyReservationsPerConferenceDay = static_cast<int>(doc["avgEarlyReservationsPerConferenceDay"].getInt());

    params.conferenceDayPaymentSaturation = static_cast<float>(doc["conferenceDayPaymentSaturation"].getDouble());
    params.percentFillPerConferenceDayPriceRange = static_cast<float>(doc["percentFillPerConferenceDayPriceRange"].getDouble());
    params.studentConferenceDayDiscount = static_cast<float>(doc["studentConferenceDayDiscount"].getDouble());
    params.maxConferenceDayReservationTimeOffset = readDuration(doc["maxConferenceDayReservationTimeOffset"]);

    params.avgNumWorkshopsPerPerson = static_cast<int>(doc["avgNumWorkshopsPerPerson"].getInt());

    params.workshopPaymentSaturation = static_cast<float>(doc["workshopPaymentSaturation"].getDouble());
    params.minOffsetFromConferenceDayReservation = readDuration(doc["minOffsetFromConferenceDayReservation"]);
    params.maxOffsetFromConferenceDayReservation = readDuration(doc["maxOffsetFromConferenceDayReservation"]);

    return params;
}

int main()
{
    static constexpr int numFirstNames = 300;
    static constexpr int numLastNames = 2000;
    static constexpr int numCountriesNames = 10;
    static constexpr int numCitiesPerCountryNames = 100;

    static constexpr int seed = 123;
    xoroshiro64plus32 rng(seed);

    ls::json::Document doc = ls::json::Document::fromFile("settings/generation_parameters.json");
    auto params = loadSettings(doc);

    ConferenceDatabase database = ConferenceDatabaseGenerator(params)(rng);

    std::experimental::filesystem::v1::create_directories("conference_database_content/tables/");
    for (auto&& csv : database.toCsvArray())
    {
        saveCsv("conference_database_content/tables/" + csv.first + ".csv", csv.second);
        //std::cout << csv.first << '\n' << csv.second << "\n\n";
    }

    std::vector<std::string> strings{ "123", "2345", "34567" };
    MarkovChainsDictionary<std::string> gen(strings.begin(), strings.end(), 2);
    gen.nextStep("12");
    

    /*
    std::cout << DateTime{ Years{ 1904 }, Months{ 1 }, Days{ 28 }, Hours{23} +Minutes{ 59 } +Seconds{ 59 } +Milliseconds{999} }.toString() << '\n';
    std::cout << DateTime{ Years{1905}, Months{1}, Days{28}, Hours{ 23 } +Minutes{5} + Seconds{14} }.toString() << '\n';
    std::cout << DateTime{ Years{ 1975 }, Months{ 1 }, Days{ 28 }, Hours{ 23 } +Minutes{ 5 } + Seconds{ 14 } }.toString() << '\n';
    std::cout << DateTime{ Years{ 1875 }, Months{ 2 }, Days{ 0 }, Hours{ 0 } }.toString() << '\n';
    std::cout << DateTime{ Years{ 1875 }, Months{ 2 }, Days{ 0 }, Hours{ 23 } +Minutes{ 5 } +Seconds{ 14 } }.toString() << '\n';
    std::cout << DateTime{ Years{ 1700 }, Months{ 1 }, Days{ 28 }, Hours{ 23 } +Minutes{ 5 } +Seconds{ 14 } }.toString() << '\n';
    std::cout << DateTime{ Hours{ 23 } +Minutes{ 5 } +Seconds{ 14 } }.toString() << '\n';

    std::cout << DateTimeGenerator(DateTime{ Years{ 1700 }, Months{ 1 }, Days{ 28 }, Hours{ 23 } +Minutes{ 5 } +Seconds{ 14 } }, DateTime{ Years{ 1904 }, Months{ 1 }, Days{ 28 }, Hours{ 23 } +Minutes{ 59 } +Seconds{ 59 } +Milliseconds{ 999 } })(rng).rounded(Hours{1}).toString() << '\n';
    */

}

