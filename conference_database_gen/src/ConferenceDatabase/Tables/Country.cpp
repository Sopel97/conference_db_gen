#include "ConferenceDatabase/Tables/Country.h"

#include "Csv/CsvRecord.h"

Country::PrimaryKeyType Country::primaryKey() const
{
    return m_countryId;
}

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
