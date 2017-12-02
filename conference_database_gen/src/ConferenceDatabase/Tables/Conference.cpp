#include "ConferenceDatabase/Tables/Conference.h"

#include <string>

#include "Csv/CsvRecord.h"
#include "Database/Record.h"
#include "DateTime.h"

Conference::PrimaryKeyType Conference::primaryKey() const
{
    return m_conferenceId;
}

Record::IdType Conference::conferenceId() const
{
    return m_conferenceId;
}

ForeignKey<Client> Conference::client() const
{
    return m_client;
}

const DateTime& Conference::startDate() const
{
    return m_startDate;
}

CsvRecord Conference::toCsvRecord() const
{
    return CsvRecord(std::to_string(m_conferenceId), std::to_string(m_client.primaryKey()), m_startDate.toString());
}