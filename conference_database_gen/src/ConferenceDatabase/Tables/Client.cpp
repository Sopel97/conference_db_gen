#include "ConferenceDatabase/Tables/Client.h"

#include "Csv/CsvRecord.h"

Client::PrimaryKeyType Client::primaryKey() const
{
    return m_clientId;
}

Record::IdType Client::clientId() const
{
    return m_clientId;
}

std::optional<ForeignKey<Person>> Client::person() const
{
    return m_person;
}

std::optional<ForeignKey<Company>> Client::company() const
{
    return m_company;
}

CsvRecord Client::toCsvRecord() const
{
    return CsvRecord(
        std::to_string(m_clientId),
        m_person.has_value() ? std::to_string(m_person.value().primaryKey()) : "NULL",
        m_company.has_value() ? std::to_string(m_company.value().primaryKey()) : "NULL"
    );
}
