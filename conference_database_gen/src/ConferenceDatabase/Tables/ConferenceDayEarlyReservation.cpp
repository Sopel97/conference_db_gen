#include "ConferenceDatabase/Tables/ConferenceDayEarlyReservation.h"

#include <string>

#include "Csv/CsvRecord.h"
#include "Database/Record.h"

ConferenceDayEarlyReservation::PrimaryKeyType ConferenceDayEarlyReservation::primaryKey() const
{
    return m_conferenceDayEarlyReservationId;
}

Record::IdType ConferenceDayEarlyReservation::conferenceDayEarlyReservationId() const
{
    return m_conferenceDayEarlyReservationId;
}

ForeignKey<ConferenceDay> ConferenceDayEarlyReservation::conferenceDay() const
{
    return m_conferenceDay;
}

ForeignKey<Company> ConferenceDayEarlyReservation::company() const
{
    return m_company;
}

int ConferenceDayEarlyReservation::numReservedSpots() const
{
    return m_numReservedSpots;
}

CsvRecord ConferenceDayEarlyReservation::toCsvRecord() const
{
    return CsvRecord(
        std::to_string(m_conferenceDayEarlyReservationId),
        std::to_string(m_conferenceDay.primaryKey()),
        std::to_string(m_company.primaryKey()),
        std::to_string(m_numReservedSpots)
    );
}
