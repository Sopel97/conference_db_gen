#pragma once

#include <vector>
#include <algorithm>

#include "ConferenceDatabase/Tables/ConferenceDayEarlyReservation.h"
#include "ConferenceDatabase/Tables/ConferenceDay.h"
#include "ConferenceDatabase/Tables/PriceRange.h"
#include "ConferenceDatabase/Tables/Student.h"
#include "Database/Table.h"

#include "Common.h"
#include "TableGenerator.h"

#include "DurationGenerator.h"

template <>
class TableGenerator<ConferenceDayEarlyReservation>
{
public:
    using RecordType = Company;
    using ResultType = Table<ConferenceDayEarlyReservation>;

    struct Params
    {
        const Table<ConferenceDay>* conferenceDays;
        const Table<Company>* companies;
        float spotSaturation;
        int avgReservationsPerDay;
    };

    TableGenerator(const Params& params);

    template <class TRng>
    Table<ConferenceDayEarlyReservation> operator()(TRng& rng) const
    {
        Table<ConferenceDayEarlyReservation> conferenceDayReservations;
        {
            int maxReservations = 0;
            for (const auto& conferenceDay : *m_params.conferenceDays)
            {
                maxReservations += conferenceDay.numSpots();
            }
            conferenceDayReservations.reserve(maxReservations);
        }

        std::uniform_int_distribution<int> dNumReservations(1, m_params.avgReservationsPerDay * 2 - 1);

        Record::IdType id = 0;
        for (const auto& conferenceDay : *m_params.conferenceDays)
        {
            const int numReservations = dNumReservations(rng);
            const int numTotalSpotsForReservations = static_cast<int>(conferenceDay.numSpots() * m_params.spotSaturation);
            const std::vector<int> reservations = splitSpotsRandomly(numTotalSpotsForReservations, numReservations, rng);

            for (const int reservation : reservations)
            {
                ForeignKey<Company> company = Common::chooseSqr(m_params.companies->records(), rng);

                conferenceDayReservations.add(
                    ConferenceDayEarlyReservation(
                        id++,
                        conferenceDay,
                        company,
                        reservation
                    )
                );
            }
        }

        return conferenceDayReservations;
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
