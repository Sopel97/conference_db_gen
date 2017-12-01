#include "ConferenceDatabase/Tables/Student.h"

#include <string>

#include "Csv/CsvRecord.h"
#include "Database/Record.h"

Record::IdType Student::studentId() const
{
    return m_studentId;
}

Record::IdType Student::personId() const
{
    return m_personId;
}

const std::string& Student::cardNumber() const
{
    return m_cardNumber;
}

CsvRecord Student::toCsvRecord() const
{
    return CsvRecord(std::to_string(m_studentId), std::to_string(m_personId), m_cardNumber);
}