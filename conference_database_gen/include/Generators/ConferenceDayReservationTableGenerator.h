#pragma once

#include <vector>
#include <algorithm>

#include "ConferenceDatabase/Tables/ConferenceDayReservation.h"
#include "ConferenceDatabase/Tables/ConferenceDayEarlyReservation.h"
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
        const Table<PriceRange>* priceRanges;
        const Table<Student>* students;
        const Table<ConferenceDayEarlyReservation>* earlyReservations;
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

        std::vector<int> participantIndices; // used for choosing a set of participants for each day
        participantIndices.reserve(m_params.participants->size());
        for (int i = 0; i < m_params.participants->size(); ++i)
        {
            participantIndices.emplace_back(i);
        }

        Record::IdType id = 0;
        for (auto resIter = m_params.earlyReservations->begin(); resIter != m_params.earlyReservations->end();)
        {
            const auto& earlyReservation = *resIter;
            const ForeignKey<ConferenceDay> conferenceDay = earlyReservation.conferenceDay();
            const int numConferenceDaySpots = conferenceDay->numSpots();

            shuffleFirstN(participantIndices, rng, numConferenceDaySpots); // we only know an upper bound

            Table<PriceRange> priceRanges = m_params.priceRanges->queryOrdered(&PriceRange::conferenceDay, conferenceDay);

            int participantIndexIndex = 0;
            for (; resIter != m_params.earlyReservations->end() && resIter->conferenceDay() == conferenceDay; ++resIter)
            {
                const auto& earlyReservation = *resIter;

                const int numSpots = earlyReservation.numReservedSpots();

                int numFreeSpots = numSpots;
                for(auto priceRangeIter = priceRanges.begin(); priceRangeIter != priceRanges.end(); ++priceRangeIter)
                {
                    const auto& priceRange = *priceRangeIter;

                    // one price range at one the same conference day
                    int numSpotsToFill = static_cast<int>(numFreeSpots * m_params.percentFillPerPriceRange);
                    if (numSpotsToFill <= 0)
                    {
                        if (numFreeSpots > 0)
                        {
                            numSpotsToFill = 1;
                        }
                        else
                        {
                            break;
                        }
                    }

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
                                conferenceDay,
                                reservationDate,
                                earlyReservation,
                                charge,
                                dIsPaid(rng) ? charge : Price(0)
                            )
                        );
                    }
                }
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

        for (int i = 0; i < n; ++i)
        {
            const int i2 = dIndex(rng);
            if (i == i2) continue;

            std::swap(values[i], values[i2]);
        }
    }
};
