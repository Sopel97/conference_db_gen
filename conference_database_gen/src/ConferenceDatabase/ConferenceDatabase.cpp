#include "ConferenceDatabase/ConferenceDatabase.h"

const ConferenceDatabase::NameArrayType& ConferenceDatabase::tableNames() const
{
    static const std::array<std::string, numTables> s_tableNames = []()
    {
        return std::array<std::string, numTables>{
            "Countries",
            "People",
            "Students",
            "Companies",
            "Customers",
            "Participants",
            "Conferences",
            "ConferenceDays",
            "PriceRanges",
            "Workshops",
            "ConferenceDayReservations",
            "WorkshopReservations"
        };
    }();

    return s_tableNames;
}

const Table<Country>& ConferenceDatabase::countries() const
{
    return table<Country>();
}
const Table<Person>& ConferenceDatabase::people() const
{
    return table<Person>();
}
const Table<Student>& ConferenceDatabase::students() const
{
    return table<Student>();
}
const Table<Company>& ConferenceDatabase::companies() const
{
    return table<Company>();
}
const Table<Customer>& ConferenceDatabase::customers() const
{
    return table<Customer>();
}
const Table<Participant>& ConferenceDatabase::participants() const
{
    return table<Participant>();
}
const Table<Conference>& ConferenceDatabase::conferences() const
{
    return table<Conference>();
}
const Table<ConferenceDay>& ConferenceDatabase::conferenceDays() const
{
    return table<ConferenceDay>();
}
const Table<PriceRange>& ConferenceDatabase::priceRanges() const
{
    return table<PriceRange>();
}
const Table<Workshop>& ConferenceDatabase::workshops() const
{
    return table<Workshop>();
}
const Table<ConferenceDayReservation>& ConferenceDatabase::conferenceDayReservations() const
{
    return table<ConferenceDayReservation>();
}
const Table<WorkshopReservation>& ConferenceDatabase::workshopReservations() const
{
    return table<WorkshopReservation>();
}

ConferenceDatabase::ConferenceDatabase(StorageType&& tables) :
    Database(std::move(tables))
{

}
