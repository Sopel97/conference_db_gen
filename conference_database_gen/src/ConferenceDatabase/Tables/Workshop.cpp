#include "ConferenceDatabase/Tables/Workshop.h"

#include <string>

#include "Csv/CsvRecord.h"
#include "Database/Record.h"
#include "DateTime.h"
#include "Price.h"

Record::IdType Workshop::workshopId() const
{
    return m_workshopId;
}

Record::IdType Workshop::conferenceDayId() const
{
    return m_conferenceDayId;
}

const DateTime& Workshop::startDate() const
{
    return m_startDate;
}

const DateTime& Workshop::endDate() const
{
    return m_endDate;
}

int Workshop::numSpots() const
{
    return m_numSpots;
}

Price Workshop::entryPrice() const
{
    return m_entryPrice;
}

CsvRecord Workshop::toCsvRecord() const
{
    return CsvRecord(
        std::to_string(m_workshopId),
        std::to_string(m_conferenceDayId),
        m_startDate.toString(),
        m_endDate.toString(),
        std::to_string(m_numSpots),
        m_entryPrice.toString()
    );
}
