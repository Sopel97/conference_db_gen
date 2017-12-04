#include "ConferenceDatabase/Tables/Company.h"

#include <string>

#include "Csv/CsvRecord.h"
#include "Database/Record.h"

Company::PrimaryKeyType Company::primaryKey() const
{
    return m_companyId;
}

Record::IdType Company::companyId() const
{
    return m_companyId;
}

const std::string& Company::companyName() const
{
    return m_companyName;
}

const std::string& Company::contactPersonName() const
{
    return m_contactPersonName;
}

const std::optional<std::string>& Company::vatin() const
{
    return m_vatin;
}

const std::string& Company::address() const
{
    return m_address;
}
const std::string& Company::postalCode() const
{
    return m_postalCode;
}
const std::string& Company::city() const
{
    return m_city;
}
ForeignKey<Country> Company::country() const
{
    return m_country;
}
const std::string& Company::phone() const
{
    return m_phone;
}
const std::string& Company::email() const
{
    return m_email;
}

CsvRecord Company::toCsvRecord() const
{
    return CsvRecord(
        std::to_string(m_companyId),
        m_companyName,
        m_contactPersonName,
        m_vatin.has_value() ? m_vatin.value() : "NULL",
        m_address, 
        m_postalCode, 
        m_city, 
        std::to_string(m_country.primaryKey()), 
        m_phone, 
        m_email
    );
}
