#include "ConferenceDatabase/Tables/Conference.h"

#include <string>

#include "Csv/CsvRecord.h"
#include "Database/Record.h"
#include "DateTime.h"

Record::IdType Conference::conferenceId() const
{
    return m_conferenceId;
}

Record::IdType Conference::clientId() const
{
    return m_clientId;
}

const DateTime& Conference::startDate() const
{
    return m_startDate;
}

CsvRecord Conference::toCsvRecord() const
{
    return CsvRecord(std::to_string(m_conferenceId), std::to_string(m_clientId), m_startDate.toString());
}