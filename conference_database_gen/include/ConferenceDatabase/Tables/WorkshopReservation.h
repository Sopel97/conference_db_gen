#pragma once

#include <string>

#include "Database/Record.h"
#include "DateTime.h"
#include "Price.h"

class WorkshopReservation : public Record
{
public:
    template <
        class TWorkshopReservationId, 
        class TParticipantId, 
        class TWorkshopId, 
        class TCharge, 
        class TIsPaid
    >
    WorkshopReservation(
        TWorkshopReservationId&& workshopReservationId, 
        TParticipantId&& participantId, 
        TWorkshopId&& workshopId, 
        TCharge&& charge, 
        TIsPaid&& isPaid
    ) :
        m_workshopReservationId(std::forward<TWorkshopReservationId>(workshopReservationId)),
        m_participantId(std::forward<TParticipantId>(participantId)),
        m_workshopId(std::forward<TWorkshopId>(workshopId)),
        m_charge(std::forward<TCharge>(charge)),
        m_isPaid(std::forward<TIsPaid>(isPaid))
    {

    }

    IdType workshopReservationId() const;
    IdType participantId() const;
    IdType workshopId() const;
    Price charge() const;
    bool isPaid() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_workshopReservationId;
    IdType m_participantId;
    IdType m_workshopId;
    Price m_charge;
    bool m_isPaid;
};
