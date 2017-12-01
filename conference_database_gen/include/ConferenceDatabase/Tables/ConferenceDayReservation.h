#pragma once

#include <string>

#include "Database/Record.h"
#include "DateTime.h"
#include "Price.h"

class ConferenceDayReservation : public Record
{
public:
    template <
        class TConferenceDayReservationId, 
        class TParticipantId, 
        class TConferenceDayId, 
        class TCharge, 
        class TIsPaid
    >
    ConferenceDayReservation(
        TConferenceDayReservationId&& conferenceDayReservationId,
        TParticipantId&& participantId,
        TConferenceDayId&& conferenceDayId, 
        TCharge&& charge, 
        TIsPaid&& isPaid
    ) :
        m_conferenceDayReservationId(std::forward<TConferenceDayReservationId>(conferenceDayReservationId)),
        m_participantId(std::forward<TParticipantId>(participantId)),
        m_conferenceDayId(std::forward<TConferenceDayId>(conferenceDayId)),
        m_charge(std::forward<TCharge>(charge)),
        m_isPaid(std::forward<TIsPaid>(isPaid))
    {

    }

    IdType conferenceDayReservationId() const;
    IdType participantId() const;
    IdType conferenceDayId() const;
    Price charge() const;
    bool isPaid() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_conferenceDayReservationId;
    IdType m_participantId;
    IdType m_conferenceDayId;
    Price m_charge;
    bool m_isPaid;
};
