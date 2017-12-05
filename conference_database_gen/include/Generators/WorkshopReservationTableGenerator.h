#pragma once

#include <vector>
#include <algorithm>

#include "ConferenceDatabase/Tables/ConferenceDayReservation.h"
#include "ConferenceDatabase/Tables/ConferenceDay.h"
#include "ConferenceDatabase/Tables/WorkshopReservation.h"
#include "ConferenceDatabase/Tables/Workshop.h"
#include "ConferenceDatabase/Tables/Participant.h"
#include "Database/Table.h"

#include "TableGenerator.h"

#include "DurationGenerator.h"

template <>
class TableGenerator<WorkshopReservation>
{
public:
    using RecordType = Company;
    using ResultType = Table<WorkshopReservation>;

    struct Params
    {
        const Table<ConferenceDayReservation>* conferenceDayReservations;
        const Table<ConferenceDay>* conferenceDays;
        const Table<Workshop>* workshops;
        float paymentSaturation;
        float spotSaturation;
        Milliseconds minOffsetFromDayReservation;
        Milliseconds maxOffsetFromDayReservation;
    };

    TableGenerator(const Params& params);

    template <class TRng>
    Table<WorkshopReservation> operator()(TRng& rng) const
    {
        Table<WorkshopReservation> workshopReservations;
        {
            int maxWorkshopReservations = 0;
            for (const auto& workshop : *m_params.workshops)
            {
                maxWorkshopReservations += workshop.numSpots();
            }
            workshopReservations.reserve(maxWorkshopReservations);
        }

        DurationGenerator offsetFromDayReservationGenerator(m_params.minOffsetFromDayReservation, m_params.maxOffsetFromDayReservation);
        std::bernoulli_distribution dIsPaid(m_params.paymentSaturation);

        Record::IdType id = 0;
        for (const auto& conferenceDay : *m_params.conferenceDays)
        {
            const int firstReservationIndex = m_params.conferenceDayReservations->findFirstIndexOrdered(&ConferenceDayReservation::conferenceDay, conferenceDay);
            const int firstWorkshopIndex = m_params.workshops->findFirstIndexOrdered(&Workshop::conferenceDay, conferenceDay);

            // reservations and workshops are ordered by conference day id so we can loop through them
            // until we hit a res/workshop from next day
            int reservationIndex = firstReservationIndex;
            int workshopIndex = firstWorkshopIndex;
            for (;;)
            {
                if (workshopIndex >= m_params.workshops->size()) break;

                const Workshop& workshop = (*m_params.workshops)[workshopIndex];
                if (workshop.conferenceDay() != conferenceDay) break;

                // fill each workshop
                int spotsLeftInWorkshop = static_cast<int>(workshop.numSpots() * m_params.spotSaturation);
                for (int i = 0; i < spotsLeftInWorkshop; ++i)
                {
                    // one participant may be in many workshops
                    // here we loop on the participants if there is too few
                    if (reservationIndex >= m_params.conferenceDayReservations->size() || (*m_params.conferenceDayReservations)[reservationIndex].conferenceDay() != conferenceDay)
                    {
                        reservationIndex = firstReservationIndex;
                    }

                    const ConferenceDayReservation& reservation = (*m_params.conferenceDayReservations)[reservationIndex];

                    //add reservation to current workshop

                    const Price charge = workshop.entryPrice();
                    workshopReservations.add(
                        WorkshopReservation(
                            id++,
                            reservation.participant(),
                            workshop,
                            reservation.date() + offsetFromDayReservationGenerator(rng),
                            charge,
                            dIsPaid(rng) ? charge : Price(0)
                        )
                    );

                    ++reservationIndex;
                }

                ++workshopIndex;
            }
        }

        return workshopReservations;
    }

private:
    Params m_params;
};
