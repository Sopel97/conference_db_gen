#include "ConferenceDatabase/Tables/ConferenceDayPrice.h"

#include <string>

#include "Csv/CsvRecord.h"
#include "Database/Record.h"

Record::IdType ConferenceDayPrice::conferenceId() const
{
    return m_conferenceId;
}

Record::IdType ConferenceDayPrice::priceRangeId() const
{
    return m_priceRangeId;
}

CsvRecord ConferenceDayPrice::toCsvRecord() const
{
    return CsvRecord(std::to_string(m_conferenceId), std::to_string(m_priceRangeId));
}
