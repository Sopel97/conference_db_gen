#include "ConferenceDatabase/Tables/ConferenceDay.h"

#include <string>

#include "Csv/CsvRecord.h"
#include "Database/Record.h"
#include "DateTime.h"

ConferenceDay::PrimaryKeyType ConferenceDay::primaryKey() const
{
    return m_conferenceDayId;
}

Record::IdType ConferenceDay::conferenceDayId() const
{
    return m_conferenceDayId;
}

ForeignKey<Conference> ConferenceDay::conference() const
{
    return m_conference;
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
    return CsvRecord(std::to_string(m_conferenceDayId), std::to_string(m_conference.primaryKey()), m_date.toString(), std::to_string(m_numSpots));
}
