#pragma once

#include <vector>
#include <algorithm>

#include "ConferenceDatabase/Tables/ConferenceDayReservation.h"
#include "ConferenceDatabase/Tables/ConferenceDay.h"
#include "ConferenceDatabase/Tables/WorkshopReservation.h"
#include "ConferenceDatabase/Tables/WorkshopEarlyReservation.h"
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
        const Table<WorkshopEarlyReservation>* workshopEarlyReservations;
        float paymentSaturation;
        Milliseconds minOffsetFromDayReservation;
        Milliseconds maxOffsetFromDayReservation;
    };

    TableGenerator(const Params& params);

    // we can't really ensure that exact people being assigned to conference day by company A are the same as those assigned to workshops by company A
    // in other words people from different companies' early day reservations are mixed here
    // it is due to the fact that these follow from logical 
    template <class TRng>
    Table<WorkshopReservation> operator()(TRng& rng) const
    {
        Table<WorkshopReservation> workshopReservations;

        DurationGenerator offsetFromDayReservationGenerator(m_params.minOffsetFromDayReservation, m_params.maxOffsetFromDayReservation);
        std::bernoulli_distribution dIsPaid(m_params.paymentSaturation);

        Record::IdType id = 0;
        for (auto iter = m_params.workshopEarlyReservations->begin(); iter != m_params.workshopEarlyReservations->end();)
        {
            const auto& workshopEarlyReservation = *iter;
            const auto& conferenceDayEarlyReservation = workshopEarlyReservation.conferenceDayEarlyReservation();
            const auto& conferenceDay = conferenceDayEarlyReservation->conferenceDay();
            const auto& company = workshopEarlyReservation.company();

            Table<ConferenceDayReservation> conferenceDayReservations = m_params.conferenceDayReservations->queryOrdered(&ConferenceDayReservation::conferenceDay, conferenceDay);
            Table<ConferenceDayReservation> conferenceDayReservationsFromCompany;
            for (const auto& conferenceDayReservation : conferenceDayReservations)
            {
                if (conferenceDayReservation.conferenceDayEarlyReservation().has_value()
                    && conferenceDayReservation.conferenceDayEarlyReservation().value()->company() != company) continue;

                conferenceDayReservationsFromCompany.add(conferenceDayReservation);
            }

            int conferenceDayReservationIdx = 0;
            for (; iter != m_params.workshopEarlyReservations->end() && iter->conferenceDayEarlyReservation()->conferenceDay() == conferenceDay; ++iter)
            {
                const auto& workshopEarlyReservation = *iter;
                const int numSpots = workshopEarlyReservation.numReservedSpots();
                for (int i = 0; i < numSpots; ++i, ++conferenceDayReservationIdx)
                {
                    conferenceDayReservationIdx %= conferenceDayReservationsFromCompany.size();
                    const auto& conferenceDayReservationLocal = conferenceDayReservationsFromCompany[conferenceDayReservationIdx];
                    const auto& conferenceDayReservation = (*m_params.conferenceDayReservations)[conferenceDayReservationLocal.primaryKey()];

                    const auto& workshop = workshopEarlyReservation.workshop();
                    const Price charge = workshop->entryPrice();
                    workshopReservations.add(
                        WorkshopReservation(
                            id++,
                            conferenceDayReservation,
                            workshopEarlyReservation,
                            conferenceDayReservation.participant(),
                            workshop,
                            conferenceDayReservation.date() + offsetFromDayReservationGenerator(rng),
                            charge,
                            dIsPaid(rng) ? charge : Price(0)
                        )
                    );
                }
            }
        }

        return workshopReservations;
    }

private:
    Params m_params;
};
