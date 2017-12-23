#pragma once

#include <tuple>
#include <array>

#include "Database/Database.h"
#include "Database/Table.h"

#include "ConferenceDatabase/Tables/Country.h"
#include "ConferenceDatabase/Tables/Person.h"
#include "ConferenceDatabase/Tables/Student.h"
#include "ConferenceDatabase/Tables/Company.h"
#include "ConferenceDatabase/Tables/Customer.h"
#include "ConferenceDatabase/Tables/Conference.h"
#include "ConferenceDatabase/Tables/ConferenceDay.h"
#include "ConferenceDatabase/Tables/ConferenceDayEarlyReservation.h"
#include "ConferenceDatabase/Tables/ConferenceDayReservation.h"
#include "ConferenceDatabase/Tables/Workshop.h"
#include "ConferenceDatabase/Tables/WorkshopReservation.h"
#include "ConferenceDatabase/Tables/PriceRange.h"
#include "ConferenceDatabase/Tables/Participant.h"

class ConferenceDatabase :
    public Database<
        Country,
        Person,
        Student,
        Company,
        Customer,
        Participant,
        Conference,
        ConferenceDay,
        PriceRange,
        Workshop,
        ConferenceDayEarlyReservation,
        ConferenceDayReservation,
        WorkshopReservation
    >
{
    friend class ConferenceDatabaseGenerator;
public:
    ConferenceDatabase() = default;

    const NameArrayType& tableNames() const override;

    const Table<Country>& countries() const;
    const Table<Person>& people() const;
    const Table<Student>& students() const;
    const Table<Company>& companies() const;
    const Table<Customer>& customers() const;
    const Table<Participant>& participants() const;
    const Table<Conference>& conferences() const;
    const Table<ConferenceDay>& conferenceDays() const;
    const Table<PriceRange>& priceRanges() const;
    const Table<Workshop>& workshops() const;
    const Table<ConferenceDayEarlyReservation>& conferenceDayEarlyReservations() const;
    const Table<ConferenceDayReservation>& conferenceDayReservations() const;
    const Table<WorkshopReservation>& workshopReservations() const;

private:
    ConferenceDatabase(StorageType&& tables);
};
