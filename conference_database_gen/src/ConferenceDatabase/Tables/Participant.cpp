#include "ConferenceDatabase/Tables/Participant.h"

#include <string>

#include "Csv/CsvRecord.h"
#include "Database/Record.h"

Participant::PrimaryKeyType Participant::primaryKey() const
{
    return m_participantId;
}

Record::IdType Participant::participantId() const
{
    return m_participantId;
}

ForeignKey<Person> Participant::person() const
{
    return m_person;
}

std::optional<ForeignKey<Company>> Participant::company() const
{
    return m_company;
}

CsvRecord Participant::toCsvRecord() const
{
    return CsvRecord(
        std::to_string(m_participantId),
        std::to_string(m_person.primaryKey()),
        m_company.has_value() ? std::to_string(m_company.value().primaryKey()) : "NULL"
    );
}