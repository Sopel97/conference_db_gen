#pragma once

#include <string>

#include "Database/Record.h"
#include "DateTime.h"

class ConferenceDay : public Record
{
public:
    template <class TConferenceDayId, class TConferenceId, class TDate, class TNumSpots>
    ConferenceDay(TConferenceDayId&& conferenceDayId, TConferenceId&& conferenceId, TDate&& date, TNumSpots&& numSpots) :
        m_conferenceDayId(std::forward<TConferenceDayId>(conferenceDayId)),
        m_conferenceId(std::forward<TConferenceId>(conferenceId)),
        m_date(std::forward<TDate>(date)),
        m_numSpots(std::forward<TNumSpots>(numSpots))
    {

    }

    IdType conferenceDayId() const;
    IdType conferenceId() const;
    DateTime date() const;
    int numSpots() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_conferenceDayId;
    IdType m_conferenceId;
    DateTime m_date;
    int m_numSpots;
};
