#include "ConferenceDatabase/Tables/Country.h"

#include "Csv/CsvRecord.h"

Record::IdType Country::countryId() const
{
    return m_countryId;
}

const std::string& Country::countryName() const
{
    return m_countryName;
}

CsvRecord Country::toCsvRecord() const
{
    return CsvRecord(
        std::to_string(m_countryId),
        m_countryName
    );
}
