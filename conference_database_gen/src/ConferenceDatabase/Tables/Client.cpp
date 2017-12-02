#include "ConferenceDatabase/Tables/Customer.h"

#include "Csv/CsvRecord.h"

Customer::PrimaryKeyType Customer::primaryKey() const
{
    return m_customerId;
}

Record::IdType Customer::customerId() const
{
    return m_customerId;
}

std::optional<ForeignKey<Person>> Customer::person() const
{
    return m_person;
}

std::optional<ForeignKey<Company>> Customer::company() const
{
    return m_company;
}

CsvRecord Customer::toCsvRecord() const
{
    return CsvRecord(
        std::to_string(m_customerId),
        m_person.has_value() ? std::to_string(m_person.value().primaryKey()) : "NULL",
        m_company.has_value() ? std::to_string(m_company.value().primaryKey()) : "NULL"
    );
}
