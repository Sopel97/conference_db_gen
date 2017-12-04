#include "ConferenceDatabase/Tables/Person.h"

#include <string>

#include "Database/Record.h"
#include "Csv/CsvRecord.h"

Person::PrimaryKeyType Person::primaryKey() const
{
    return m_personId;
}

Record::IdType Person::personId() const
{
    return m_personId;
}

const std::string& Person::firstName() const
{
    return m_firstName;
}

const std::string& Person::lastName() const
{
    return m_lastName;
}

const DateTime& Person::birthDate() const
{
    return m_birthDate;
}

const std::string& Person::address() const
{
    return m_address;
}

const std::string& Person::postalCode() const
{
    return m_postalCode;
}

const std::string& Person::city() const
{
    return m_city;
}

ForeignKey<Country> Person::country() const
{
    return m_country;
}

const std::string& Person::phone() const
{
    return m_phone;
}

const std::string& Person::email() const
{
    return m_email;
}

CsvRecord Person::toCsvRecord() const
{
    return CsvRecord(
        std::to_string(m_personId), 
        m_firstName, 
        m_lastName, 
        m_birthDate.toString(), 
        m_address, 
        m_postalCode, 
        m_city, 
        std::to_string(m_country.primaryKey()), 
        m_phone, 
        m_email
    );
}
