#include "ConferenceDatabase/Tables/Student.h"

#include <string>

#include "Csv/CsvRecord.h"
#include "Database/Record.h"

Student::PrimaryKeyType Student::primaryKey() const
{
    return m_studentId;
}

Record::IdType Student::studentId() const
{
    return m_studentId;
}

ForeignKey<Person> Student::person() const
{
    return m_person;
}

const std::string& Student::cardNumber() const
{
    return m_cardNumber;
}

CsvRecord Student::toCsvRecord() const
{
    return CsvRecord(std::to_string(m_studentId), std::to_string(m_person.primaryKey()), m_cardNumber);
}