#pragma once

#include <string>

#include "Database/Record.h"
#include "Database/ForeignKey.h"
#include "DateTime.h"

#include "Conference.h"

class ConferenceDay : public Record
{
public:
    using PrimaryKeyType = IdType;

    template <
        class TConferenceDayId, 
        class TConference, 
        class TDate,
        class TNumSpots, 
        class TNumReservedSpots, 
        class TIsReservationFilled
    >
    ConferenceDay(
        TConferenceDayId&& conferenceDayId, 
        TConference&& conference, 
        TDate&& date, 
        TNumSpots&& numSpots,
        TNumReservedSpots&& numReservedSpots,
        TIsReservationFilled&& isReservationFilled
    ) :
        m_conferenceDayId(std::forward<TConferenceDayId>(conferenceDayId)),
        m_conference(std::forward<TConference>(conference)),
        m_date(std::forward<TDate>(date)),
        m_numSpots(std::forward<TNumSpots>(numSpots)),
        m_numReservedSpots(std::forward<TNumReservedSpots>(numReservedSpots)),
        m_isReservationFilled(std::forward<TIsReservationFilled>(isReservationFilled))
    {

    }

    ConferenceDay(const ConferenceDay&) = default;
    ConferenceDay(ConferenceDay&&) noexcept = default;

    ConferenceDay& operator=(const ConferenceDay&) = default;
    ConferenceDay& operator=(ConferenceDay&&) noexcept = default;

    PrimaryKeyType primaryKey() const;

    IdType conferenceDayId() const;
    ForeignKey<Conference> conference() const;
    DateTime date() const;
    int numSpots() const;
    int numReservedSpots() const;
    bool isReservationFilled() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_conferenceDayId;
    ForeignKey<Conference> m_conference;
    DateTime m_date;
    int m_numSpots;
    int m_numReservedSpots;
    bool m_isReservationFilled;
};
