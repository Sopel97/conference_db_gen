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

Price ConferenceDayReservation::charge() const
{
    return m_charge;
}

bool ConferenceDayReservation::isPaid() const
{
    return m_isPaid;
}

CsvRecord ConferenceDayReservation::toCsvRecord() const
{
    return CsvRecord(
        std::to_string(m_conferenceDayReservationId),
        std::to_string(m_participant.primaryKey()),
        std::to_string(m_conferenceDay.primaryKey()),
        m_charge.toString(),
        std::to_string(m_isPaid)
    );
}
