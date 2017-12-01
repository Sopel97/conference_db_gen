#include "ConferenceDatabase/Tables/WorkshopReservation.h"

#include <string>

#include "Csv/CsvRecord.h"
#include "Database/Record.h"
#include "DateTime.h"
#include "Price.h"

Record::IdType WorkshopReservation::workshopReservationId() const
{
    return m_workshopReservationId;
}

Record::IdType WorkshopReservation::participantId() const
{
    return m_participantId;
}

Record::IdType WorkshopReservation::workshopId() const
{
    return m_workshopId;
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
        std::to_string(m_participantId),
        std::to_string(m_workshopId),
        m_charge.toString(),
        std::to_string(m_isPaid)
    );
}
