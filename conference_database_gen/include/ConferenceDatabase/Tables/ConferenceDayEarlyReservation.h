#pragma once

#include <string>

#include "Database/Record.h"
#include "Database/ForeignKey.h"

#include "DateTime.h"
#include "Price.h"

#include "Participant.h"
#include "ConferenceDay.h"

class ConferenceDayEarlyReservation : public Record
{
public:
    using PrimaryKeyType = IdType;

    template <
        class TConferenceDayEarlyReservationId,
        class TConferenceDay,
        class TCompany,
        class TNumReservedSpots
    >
        ConferenceDayEarlyReservation(
            TConferenceDayEarlyReservationId&& conferenceDayEarlyReservationId,
            TConferenceDay&& conferenceDay,
            TCompany&& company,
            TNumReservedSpots&& numReservedSpots
        ) :
        m_conferenceDayEarlyReservationId(std::forward<TConferenceDayEarlyReservationId>(conferenceDayEarlyReservationId)),
        m_conferenceDay(std::forward<TConferenceDay>(conferenceDay)),
        m_company(std::forward<TCompany>(company)),
        m_numReservedSpots(std::forward<TNumReservedSpots>(numReservedSpots))
    {

    }

    ConferenceDayEarlyReservation(const ConferenceDayEarlyReservation&) = default;
    ConferenceDayEarlyReservation(ConferenceDayEarlyReservation&&) noexcept = default;

    ConferenceDayEarlyReservation& operator=(const ConferenceDayEarlyReservation&) = default;
    ConferenceDayEarlyReservation& operator=(ConferenceDayEarlyReservation&&) noexcept = default;

    PrimaryKeyType primaryKey() const;

    IdType conferenceDayEarlyReservationId() const;
    ForeignKey<ConferenceDay> conferenceDay() const;
    ForeignKey<Company> company() const;
    int numReservedSpots() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_conferenceDayEarlyReservationId;
    ForeignKey<ConferenceDay> m_conferenceDay;
    ForeignKey<Company> m_company;
    int m_numReservedSpots;
};
