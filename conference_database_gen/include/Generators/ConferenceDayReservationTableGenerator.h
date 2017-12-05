#pragma once

#include <vector>
#include <algorithm>

#include "ConferenceDatabase/Tables/ConferenceDayReservation.h"
#include "ConferenceDatabase/Tables/ConferenceDay.h"
#include "ConferenceDatabase/Tables/PriceRange.h"
#include "ConferenceDatabase/Tables/Student.h"
#include "Database/Table.h"

#include "TableGenerator.h"

#include "DurationGenerator.h"

template <>
class TableGenerator<ConferenceDayReservation>
{
public:
    using RecordType = Company;
    using ResultType = Table<ConferenceDayReservation>;

    struct Params
    {
        const Table<Participant>* participants;
        const Table<ConferenceDay>* conferenceDays;
        const Table<PriceRange>* priceRanges;
        const Table<Student>* students;
        float paymentSaturation;
        Milliseconds priceRangeDuration;
        float percentFillPerPriceRange;
        float studentDiscount;
        Milliseconds maxReservationTimeOffset;
    };

    TableGenerator(const Params& params);

    template <class TRng>
    Table<ConferenceDayReservation> operator()(TRng& rng) const
    {
        DurationGenerator reservationTimeOffsetGenerator(-m_params.priceRangeDuration, m_params.maxReservationTimeOffset);

        std::bernoulli_distribution dIsPaid(m_params.paymentSaturation);

        Table<ConferenceDayReservation> conferenceDayReservations;
        {
            int maxReservations = 0;
            for (const auto& conferenceDay : *m_params.conferenceDays)
            {
                maxReservations += conferenceDay.numSpots();
            }
            conferenceDayReservations.reserve(maxReservations);
        }

        std::vector<int> participantIndices; // used for choosing a set of participants for each day
        participantIndices.reserve(m_params.participants->size());
        for (int i = 0; i < m_params.participants->size(); ++i)
        {
            participantIndices.emplace_back(i);
        }

        Record::IdType id = 0;
        for (auto priceRangeIter = m_params.priceRanges->begin(); priceRangeIter != m_params.priceRanges->end();)
        {
            auto currentConferenceDay = priceRangeIter->conferenceDay();
            auto lastConferenceDay = currentConferenceDay;

            int numFreeSpots = currentConferenceDay->numSpots();

            shuffleFirstN(participantIndices, rng, numFreeSpots);

            int participantIndexIndex = 0;
            while (priceRangeIter != m_params.priceRanges->end() && currentConferenceDay == lastConferenceDay)
            {
                const auto& priceRange = *priceRangeIter;

                // one price range at one the same conference day
                const int numSpotsToFill = static_cast<int>(numFreeSpots * m_params.percentFillPerPriceRange);
                numFreeSpots -= numSpotsToFill;

                for (int i = 0; i < numSpotsToFill; ++i)
                {
                    const auto& participant = (*m_params.participants)[participantIndices[participantIndexIndex++]];

                    const DateTime reservationDate = priceRange.startDate() - reservationTimeOffsetGenerator(rng);
                    const bool isStudent = m_params.students->existsOrdered(&Student::primaryKey, participant.primaryKey());
                    const Price charge = Price(priceRange.price().units() * 
                        (isStudent ? (1.0f - m_params.studentDiscount) : 1.0f)
                    );

                    conferenceDayReservations.add(
                        ConferenceDayReservation(
                            id++,
                            participant,
                            currentConferenceDay,
                            reservationDate,
                            charge,
                            dIsPaid(rng) ? charge : Price(0)
                        )
                    );
                }

                lastConferenceDay = currentConferenceDay;
                ++priceRangeIter;
                currentConferenceDay = priceRangeIter->conferenceDay();
            }
        }

        return conferenceDayReservations;
    }

private:
    Params m_params;

    template <class T, class TRng>
    static void shuffleFirstN(std::vector<T>& values, TRng&& rng, int n)
    {
        std::uniform_int_distribution<int> dIndex(0, values.size() - 1);

        for(int i = 0; i < n; ++i)
        {
            const int i2 = dIndex(rng);
            if (i == i2) continue;

            std::swap(values[i], values[i2]);
        }
    }
};
