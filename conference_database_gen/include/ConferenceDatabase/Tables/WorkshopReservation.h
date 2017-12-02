#pragma once

#include <string>

#include "Database/Record.h"
#include "Database/ForeignKey.h"

#include "DateTime.h"
#include "Price.h"

#include "Workshop.h"
#include "Participant.h"

class WorkshopReservation : public Record
{
public:
    using PrimaryKeyType = IdType;

    template <
        class TWorkshopReservationId, 
        class TParticipant, 
        class TWorkshop, 
        class TCharge, 
        class TIsPaid
    >
    WorkshopReservation(
        TWorkshopReservationId&& workshopReservationId, 
        TParticipant&& participant, 
        TWorkshop&& workshop, 
        TCharge&& charge, 
        TIsPaid&& isPaid
    ) :
        m_workshopReservationId(std::forward<TWorkshopReservationId>(workshopReservationId)),
        m_participant(std::forward<TParticipant>(participant)),
        m_workshop(std::forward<TWorkshop>(workshop)),
        m_charge(std::forward<TCharge>(charge)),
        m_isPaid(std::forward<TIsPaid>(isPaid))
    {

    }

    PrimaryKeyType primaryKey() const;

    IdType workshopReservationId() const;
    ForeignKey<Participant> participant() const;
    ForeignKey<Workshop> workshop() const;
    Price charge() const;
    bool isPaid() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_workshopReservationId;
    ForeignKey<Participant> m_participant;
    ForeignKey<Workshop> m_workshop;
    Price m_charge;
    bool m_isPaid;
};
