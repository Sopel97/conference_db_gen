#pragma once

#include <vector>
#include <algorithm>

#include "ConferenceDatabase/Tables/ConferenceDayEarlyReservation.h"
#include "ConferenceDatabase/Tables/WorkshopEarlyReservation.h"
#include "ConferenceDatabase/Tables/Workshop.h"
#include "ConferenceDatabase/Tables/ConferenceDay.h"
#include "ConferenceDatabase/Tables/PriceRange.h"
#include "ConferenceDatabase/Tables/Student.h"
#include "Database/Table.h"

#include "Common.h"
#include "TableGenerator.h"

#include "DurationGenerator.h"

template <>
class TableGenerator<WorkshopEarlyReservation>
{
public:
    using RecordType = Company;
    using ResultType = Table<WorkshopEarlyReservation>;

    struct Params
    {
        const Table<ConferenceDayEarlyReservation>* earlyConferenceDayReservations;
        const Table<Workshop>* workshops;
        int avgNumWorkshopsPerPerson;
    };

    TableGenerator(const Params& params);

    template <class TRng>
    Table<WorkshopEarlyReservation> operator()(TRng& rng) const
    {
        Table<WorkshopEarlyReservation> workshopReservations;

        Record::IdType id = 0;
        for (auto confDayEarlyResIter = m_params.earlyConferenceDayReservations->begin(); confDayEarlyResIter != m_params.earlyConferenceDayReservations->end();)
        {
            const auto& confDayEarlyRes = *confDayEarlyResIter;
            const auto conferenceDay = confDayEarlyRes.conferenceDay();

            Table<Workshop> workshops = m_params.workshops->queryOrdered(&Workshop::conferenceDay, conferenceDay);

            std::vector<int> workshopSpotsLeft;
            for (const auto& workshop : workshops)
            {
                workshopSpotsLeft.emplace_back(workshop.numSpots());
            }

            for (; confDayEarlyResIter != m_params.earlyConferenceDayReservations->end() && confDayEarlyResIter->conferenceDay() == conferenceDay; ++confDayEarlyResIter)
            {
                const auto& confDayEarlyRes = *confDayEarlyResIter;

                const int numReservedDaySpots = confDayEarlyRes.numReservedSpots();

                std::vector<int> numReservedSpotsPerWorkshop = splitSpotsRandomly(numReservedDaySpots * m_params.avgNumWorkshopsPerPerson, workshops.size(), rng);
                for (int n : numReservedSpotsPerWorkshop)
                {
                    if (n == 0) continue;
                    if (n > numReservedDaySpots) n = numReservedDaySpots;

                    const int workshopIdx = Common::chooseSqrIndex(workshops.size(), rng);
                    const auto& workshop = workshops[workshopIdx];

                    if (workshopSpotsLeft[workshopIdx] == 0) continue;
                    
                    n = std::min(n, workshopSpotsLeft[workshopIdx]);
                    workshopSpotsLeft[workshopIdx] -= n;

                    workshopReservations.add(
                        WorkshopEarlyReservation(
                            id++,
                            confDayEarlyRes,
                            (*m_params.workshops)[workshop.primaryKey()], // since out workshop array is only a temporary
                            confDayEarlyRes.company(),
                            n
                        )
                    );
                }
            }
        }

        return workshopReservations;
    }

private:
    Params m_params;

    template <class TRng>
    std::vector<int> splitSpotsRandomly(int numSpots, int numBuckets, TRng& rng) const
    {
        std::vector<int> buckets(numBuckets, 0);

        std::uniform_int_distribution<int> dBucket(0, numBuckets - 1);

        for (int i = 0; i < numSpots; ++i)
        {
            ++buckets[dBucket(rng)];
        }

        return buckets;
    }
};
