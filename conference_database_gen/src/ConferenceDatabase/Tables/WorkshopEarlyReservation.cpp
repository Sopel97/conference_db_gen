#include "ConferenceDatabase/Tables/WorkshopEarlyReservation.h"

#include <string>

#include "Csv/CsvRecord.h"
#include "Database/Record.h"

WorkshopEarlyReservation::PrimaryKeyType WorkshopEarlyReservation::primaryKey() const
{
    return m_workshopEarlyReservationId;
}

Record::IdType WorkshopEarlyReservation::workshopEarlyReservationId() const
{
    return m_workshopEarlyReservationId;
}
ForeignKey<ConferenceDayEarlyReservation> WorkshopEarlyReservation::conferenceDayEarlyReservation() const
{
    return m_conferenceDayEarlyReservation;
}

ForeignKey<Workshop> WorkshopEarlyReservation::workshop() const
{
    return m_workshop;
}

ForeignKey<Company> WorkshopEarlyReservation::company() const
{
    return m_company;
}

int WorkshopEarlyReservation::numReservedSpots() const
{
    return m_numReservedSpots;
}

CsvRecord WorkshopEarlyReservation::toCsvRecord() const
{
    return CsvRecord(
        std::to_string(m_workshopEarlyReservationId),
        std::to_string(m_conferenceDayEarlyReservation.primaryKey()),
        std::to_string(m_workshop.primaryKey()),
        std::to_string(m_company.primaryKey()),
        std::to_string(m_numReservedSpots)
    );
}
