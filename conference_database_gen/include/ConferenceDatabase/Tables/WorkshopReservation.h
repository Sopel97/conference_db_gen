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
        class TDate,
        class TCharge, 
        class TPaidAmount
    >
    WorkshopReservation(
        TWorkshopReservationId&& workshopReservationId, 
        TParticipant&& participant, 
        TWorkshop&& workshop, 
        TDate&& date,
        TCharge&& charge, 
        TPaidAmount&& paidAmount
    ) :
        m_workshopReservationId(std::forward<TWorkshopReservationId>(workshopReservationId)),
        m_participant(std::forward<TParticipant>(participant)),
        m_workshop(std::forward<TWorkshop>(workshop)),
        m_date(std::forward<TDate>(date)),
        m_charge(std::forward<TCharge>(charge)),
        m_paidAmount(std::forward<TPaidAmount>(paidAmount))
    {

    }

    PrimaryKeyType primaryKey() const;

    IdType workshopReservationId() const;
    ForeignKey<Participant> participant() const;
    ForeignKey<Workshop> workshop() const;
    const DateTime& date() const;
    Price charge() const;
    Price paidAmount() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_workshopReservationId;
    ForeignKey<Participant> m_participant;
    ForeignKey<Workshop> m_workshop;
    DateTime m_date;
    Price m_charge;
    Price m_paidAmount;
};
