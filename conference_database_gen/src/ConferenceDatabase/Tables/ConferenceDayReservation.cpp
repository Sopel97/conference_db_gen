#include "ConferenceDatabase/Tables/ConferenceDayReservation.h"

#include <string>

#include "Csv/CsvRecord.h"
#include "Database/Record.h"
#include "DateTime.h"
#include "Price.h"

ConferenceDayReservation::PrimaryKeyType ConferenceDayReservation::primaryKey() const
{
    return m_conferenceDayReservationId;
}

Record::IdType ConferenceDayReservation::conferenceDayReservationId() const
{
    return m_conferenceDayReservationId;
}

ForeignKey<Participant> ConferenceDayReservation::participant() const
{
    return m_participant;
}

ForeignKey<ConferenceDay> ConferenceDayReservation::conferenceDay() const
{
    return m_conferenceDay;
}

const DateTime& ConferenceDayReservation::date() const
{
    return m_date;
}

std::optional<ForeignKey<ConferenceDayEarlyReservation>> ConferenceDayReservation::conferenceDayEarlyReservation() const
{
    return m_conferenceDayEarlyReservation;
}

Price ConferenceDayReservation::charge() const
{
    return m_charge;
}

Price ConferenceDayReservation::paidAmount() const
{
    return m_paidAmount;
}

CsvRecord ConferenceDayReservation::toCsvRecord() const
{
    return CsvRecord(
        std::to_string(m_conferenceDayReservationId),
        std::to_string(m_participant.primaryKey()),
        std::to_string(m_conferenceDay.primaryKey()),
        m_date.toString(),
        m_conferenceDayEarlyReservation.has_value() ? std::to_string(m_conferenceDayEarlyReservation.value().primaryKey()) : "NULL",
        m_charge.toString(),
        m_paidAmount.toString()
    );
}
