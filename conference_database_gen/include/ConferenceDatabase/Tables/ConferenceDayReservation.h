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
        class TCharge, 
        class TIsPaid
    >
    ConferenceDayReservation(
        TConferenceDayReservationId&& conferenceDayReservationId,
        TParticipant&& participant,
        TConferenceDay&& conferenceDay, 
        TCharge&& charge, 
        TIsPaid&& isPaid
    ) :
        m_conferenceDayReservationId(std::forward<TConferenceDayReservationId>(conferenceDayReservationId)),
        m_participant(std::forward<TParticipant>(participant)),
        m_conferenceDay(std::forward<TConferenceDay>(conferenceDay)),
        m_charge(std::forward<TCharge>(charge)),
        m_isPaid(std::forward<TIsPaid>(isPaid))
    {

    }

    PrimaryKeyType primaryKey() const;

    IdType conferenceDayReservationId() const;
    ForeignKey<Participant> participant() const;
    ForeignKey<ConferenceDay> conferenceDay() const;
    Price charge() const;
    bool isPaid() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_conferenceDayReservationId;
    ForeignKey<Participant> m_participant;
    ForeignKey<ConferenceDay> m_conferenceDay;
    Price m_charge;
    bool m_isPaid;
};
