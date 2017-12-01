#include "ConferenceDatabase/Tables/ConferenceDayReservation.h"

#include <string>

#include "Csv/CsvRecord.h"
#include "Database/Record.h"
#include "DateTime.h"
#include "Price.h"

Record::IdType ConferenceDayReservation::conferenceDayReservationId() const
{
    return m_conferenceDayReservationId;
}

Record::IdType ConferenceDayReservation::participantId() const
{
    return m_participantId;
}

Record::IdType ConferenceDayReservation::conferenceDayId() const
{
    return m_conferenceDayId;
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
        std::to_string(m_participantId),
        std::to_string(m_conferenceDayId),
        m_charge.toString(),
        std::to_string(m_isPaid)
    );
}
