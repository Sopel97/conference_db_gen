#include "ConferenceDatabase/Tables/PriceRange.h"

#include <string>

#include "Csv/CsvRecord.h"
#include "Database/Record.h"
#include "DateTime.h"
#include "Price.h"

PriceRange::PrimaryKeyType PriceRange::primaryKey() const
{
    return m_priceRangeId;
}

Record::IdType PriceRange::priceRangeId() const
{
    return m_priceRangeId;
}

ForeignKey<ConferenceDay> PriceRange::conferenceDay() const
{
    return m_conferenceDay;
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
    return CsvRecord(std::to_string(m_priceRangeId), std::to_string(m_conferenceDay.primaryKey()), m_startDate.toString(), m_price.toString());
}
