#pragma once

#include <string>

#include "Database/Record.h"
#include "Database/ForeignKey.h"

#include "DateTime.h"
#include "Price.h"

#include "Participant.h"
#include "ConferenceDay.h"

class ConferenceDayReservation : public Record
{
public:
    using PrimaryKeyType = IdType;

    template <
        class TConferenceDayReservationId, 
        class TParticipant, 
        class TConferenceDay,
        class TDate,
        class TCharge,
        class TPaidAmount
    >
    ConferenceDayReservation(
        TConferenceDayReservationId&& conferenceDayReservationId,
        TParticipant&& participant,
        TConferenceDay&& conferenceDay, 
        TDate&& date,
        TCharge&& charge, 
        TPaidAmount&& paidAmount
    ) :
        m_conferenceDayReservationId(std::forward<TConferenceDayReservationId>(conferenceDayReservationId)),
        m_participant(std::forward<TParticipant>(participant)),
        m_conferenceDay(std::forward<TConferenceDay>(conferenceDay)),
        m_date(std::forward<TDate>(date)),
        m_charge(std::forward<TCharge>(charge)),
        m_paidAmount(std::forward<TPaidAmount>(paidAmount))
    {

    }

    ConferenceDayReservation(const ConferenceDayReservation&) = default;
    ConferenceDayReservation(ConferenceDayReservation&&) noexcept = default;

    ConferenceDayReservation& operator=(const ConferenceDayReservation&) = default;
    ConferenceDayReservation& operator=(ConferenceDayReservation&&) noexcept = default;

    PrimaryKeyType primaryKey() const;

    IdType conferenceDayReservationId() const;
    ForeignKey<Participant> participant() const;
    ForeignKey<ConferenceDay> conferenceDay() const;
    const DateTime& date() const;
    Price charge() const;
    Price paidAmount() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_conferenceDayReservationId;
    ForeignKey<Participant> m_participant;
    ForeignKey<ConferenceDay> m_conferenceDay;
    DateTime m_date;
    Price m_charge;
    Price m_paidAmount;
};
