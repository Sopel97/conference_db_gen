#include "ConferenceDatabase/Tables/Participant.h"

#include <string>

#include "Csv/CsvRecord.h"
#include "Database/Record.h"

Record::IdType Participant::participantId() const
{
    return m_participantId;
}

Record::IdType Participant::personId() const
{
    return m_personId;
}

CsvRecord Participant::toCsvRecord() const
{
    return CsvRecord(std::to_string(m_participantId), std::to_string(m_personId));
}