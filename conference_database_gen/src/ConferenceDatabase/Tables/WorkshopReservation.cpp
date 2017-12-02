#include "ConferenceDatabase/Tables/WorkshopReservation.h"

#include <string>

#include "Csv/CsvRecord.h"
#include "Database/Record.h"
#include "DateTime.h"
#include "Price.h"

WorkshopReservation::PrimaryKeyType WorkshopReservation::primaryKey() const
{
    return m_workshopReservationId;
}

Record::IdType WorkshopReservation::workshopReservationId() const
{
    return m_workshopReservationId;
}

ForeignKey<Participant> WorkshopReservation::participant() const
{
    return m_participant;
}

ForeignKey<Workshop> WorkshopReservation::workshop() const
{
    return m_workshop;
}

Price WorkshopReservation::charge() const
{
    return m_charge;
}

bool WorkshopReservation::isPaid() const
{
    return m_isPaid;
}

CsvRecord WorkshopReservation::toCsvRecord() const
{
    return CsvRecord(
        std::to_string(m_workshopReservationId),
        std::to_string(m_participant.primaryKey()),
        std::to_string(m_workshop.primaryKey()),
        m_charge.toString(),
        std::to_string(m_isPaid)
    );
}
