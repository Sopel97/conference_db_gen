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
ForeignKey<ConferenceDayReservation> WorkshopReservation::conferenceDayReservation() const
{
    return m_conferenceDayReservation;
}

std::optional<ForeignKey<WorkshopEarlyReservation>> WorkshopReservation::workshopEarlyReservation() const
{
    return m_workshopEarlyReservation;
}

ForeignKey<Participant> WorkshopReservation::participant() const
{
    return m_participant;
}

ForeignKey<Workshop> WorkshopReservation::workshop() const
{
    return m_workshop;
}

const DateTime& WorkshopReservation::date() const
{
    return m_date;
}

Price WorkshopReservation::charge() const
{
    return m_charge;
}

Price WorkshopReservation::paidAmount() const
{
    return m_paidAmount;
}

CsvRecord WorkshopReservation::toCsvRecord() const
{
    return CsvRecord(
        std::to_string(m_workshopReservationId),
        std::to_string(m_conferenceDayReservation->primaryKey()),
        m_workshopEarlyReservation.has_value() ? std::to_string(m_workshopEarlyReservation.value()->primaryKey()) : "NULL",
        std::to_string(m_participant.primaryKey()),
        std::to_string(m_workshop.primaryKey()),
        m_date.toString(),
        m_charge.toString(),
        m_paidAmount.toString()
    );
}
