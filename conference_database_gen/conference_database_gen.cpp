#include <iostream>
#include <random>
#include <filesystem>

#include "ConferenceDatabase/Tables/Person.h"
#include "Database/Table.h"
#include "ConferenceDatabase/ConferenceDatabase.h"

#include "Generators/ConferenceDatabaseGenerator.h"

#include "Generators/MarkovChainsDictionary.h"
#include "Generators/DateTimeGenerator.h"

void saveCsv(const std::string& path, const CsvDocument& csv)
{
    std::ofstream file(path);

    file << csv;
}

int main()
{
    static constexpr int numFirstNames = 300;
    static constexpr int numLastNames = 2000;
    static constexpr int numCountriesNames = 10;
    static constexpr int numCitiesPerCountryNames = 100;

    std::ranlux48 rng;

    
    ConferenceDatabase database = ConferenceDatabaseGenerator(numFirstNames, numLastNames, numCountriesNames, numCitiesPerCountryNames)(rng);

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

