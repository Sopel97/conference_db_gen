#pragma once

#include <string>

#include "Database/Record.h"
#include "Database/ForeignKey.h"

#include "DateTime.h"
#include "Price.h"

#include "Participant.h"
#include "Workshop.h"
#include "ConferenceDayEarlyReservation.h"

class WorkshopEarlyReservation : public Record
{
public:
    using PrimaryKeyType = IdType;

    template <
        class TWorkshopEarlyReservationId,
        class TConferenceDayEarlyReservation,
        class TWorkshop,
        class TCompany,
        class TNumReservedSpots
    >
        WorkshopEarlyReservation(
            TWorkshopEarlyReservationId&& workshopEarlyReservationId,
            TConferenceDayEarlyReservation&& conferenceDayEarlyReservation,
            TWorkshop&& workshop,
            TCompany&& company,
            TNumReservedSpots&& numReservedSpots
        ) :
        m_workshopEarlyReservationId(std::forward<TWorkshopEarlyReservationId>(workshopEarlyReservationId)),
        m_conferenceDayEarlyReservation(std::forward<TConferenceDayEarlyReservation>(conferenceDayEarlyReservation)),
        m_workshop(std::forward<TWorkshop>(workshop)),
        m_company(std::forward<TCompany>(company)),
        m_numReservedSpots(std::forward<TNumReservedSpots>(numReservedSpots))
    {

    }

    WorkshopEarlyReservation(const WorkshopEarlyReservation&) = default;
    WorkshopEarlyReservation(WorkshopEarlyReservation&&) noexcept = default;

    WorkshopEarlyReservation& operator=(const WorkshopEarlyReservation&) = default;
    WorkshopEarlyReservation& operator=(WorkshopEarlyReservation&&) noexcept = default;

    PrimaryKeyType primaryKey() const;

    IdType workshopEarlyReservationId() const;
    ForeignKey<ConferenceDayEarlyReservation> conferenceDayEarlyReservation() const;
    ForeignKey<Workshop> workshop() const;
    ForeignKey<Company> company() const;
    int numReservedSpots() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_workshopEarlyReservationId;
    ForeignKey<ConferenceDayEarlyReservation> m_conferenceDayEarlyReservation;
    ForeignKey<Workshop> m_workshop;
    ForeignKey<Company> m_company;
    int m_numReservedSpots;
};
