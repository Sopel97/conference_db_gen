#include "ConferenceDatabase/Tables/ConferenceDay.h"

#include <string>

#include "Csv/CsvRecord.h"
#include "Database/Record.h"
#include "DateTime.h"

Record::IdType ConferenceDay::conferenceDayId() const
{
    return m_conferenceDayId;
}

Record::IdType ConferenceDay::conferenceId() const
{
    return m_conferenceId;
}

DateTime ConferenceDay::date() const
{
    return m_date;
}

int ConferenceDay::numSpots() const
{
    return m_numSpots;
}

CsvRecord ConferenceDay::toCsvRecord() const
{
    return CsvRecord(std::to_string(m_conferenceDayId), std::to_string(m_conferenceId), m_date.toString(), std::to_string(m_numSpots));
}
