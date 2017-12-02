#include "ConferenceDatabase/Tables/PriceRange.h"

#include <string>

#include "Csv/CsvRecord.h"
#include "Database/Record.h"
#include "DateTime.h"
#include "Price.h"

Record::IdType PriceRange::priceRangeId() const
{
    return m_priceRangeId;
}

Record::IdType PriceRange::conferenceDayId() const
{
    return m_conferenceDayId;
}

DateTime PriceRange::startDate() const
{
    return m_startDate;
}

Price PriceRange::price() const
{
    return m_price;
}

CsvRecord PriceRange::toCsvRecord() const
{
    return CsvRecord(std::to_string(m_priceRangeId), m_startDate.toString(), m_price.toString());
}
