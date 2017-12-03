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

    TableGenerator(
        const Table<ConferenceDayReservation>& conferenceDayReservations,
        const Table<ConferenceDay>& conferenceDays,
        const Table<Workshop>& workshops,
        float paymentSaturation
    );

    template <class TRng>
    Table<WorkshopReservation> operator()(TRng& rng) const
    {
        static constexpr float spotSaturation = 0.9f;
        static constexpr Milliseconds minOffsetFromDayReservation = Minutes{ 1 };
        static constexpr Milliseconds maxOffsetFromDayReservation = Days{ 1 };

        Table<WorkshopReservation> workshopReservations;
        {
            int maxWorkshopReservations = 0;
            for (const auto& workshop : *m_workshops)
            {
                maxWorkshopReservations += workshop.numSpots();
            }
            workshopReservations.reserve(maxWorkshopReservations);
        }

        DurationGenerator offsetFromDayReservationGenerator(minOffsetFromDayReservation, maxOffsetFromDayReservation);
        std::bernoulli_distribution dIsPaid(m_paymentSaturation);

        Record::IdType id = 0;
        for (const auto& conferenceDay : *m_conferenceDays)
        {
            const int firstReservationIndex = m_conferenceDayReservations->findFirstIndexOrdered(&ConferenceDayReservation::conferenceDay, conferenceDay);
            const int firstWorkshopIndex = m_workshops->findFirstIndexOrdered(&Workshop::conferenceDay, conferenceDay);

            // reservations and workshops are ordered by conference day id so we can loop through them
            // until we hit a res/workshop from next day
            int reservationIndex = firstReservationIndex;
            int workshopIndex = firstWorkshopIndex;
            for (;;)
            {
                if (workshopIndex >= m_workshops->size()) break;

                const Workshop& workshop = (*m_workshops)[workshopIndex];
                if (workshop.conferenceDay() != conferenceDay) break;

                // fill each workshop
                int spotsLeftInWorkshop = static_cast<int>(workshop.numSpots() * spotSaturation);
                for (int i = 0; i < spotsLeftInWorkshop; ++i)
                {
                    // one participant may be in many workshops
                    // here we loop on the participants if there is too few
                    if (reservationIndex >= m_conferenceDayReservations->size() || (*m_conferenceDayReservations)[reservationIndex].conferenceDay() != conferenceDay)
                    {
                        reservationIndex = firstReservationIndex;
                    }

                    const ConferenceDayReservation& reservation = (*m_conferenceDayReservations)[reservationIndex];

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
    const Table<ConferenceDayReservation>* m_conferenceDayReservations;
    const Table<ConferenceDay>* m_conferenceDays;
    const Table<Workshop>* m_workshops;
    float m_paymentSaturation;
};
