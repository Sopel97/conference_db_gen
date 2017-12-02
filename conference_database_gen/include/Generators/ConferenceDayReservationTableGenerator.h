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

    TableGenerator(
        const Table<Participant>& participants,
        const Table<ConferenceDay>& conferenceDays,
        const Table<PriceRange>& priceRanges,
        const Table<Student>& students,
        float paymentSaturation,
        Milliseconds priceRangeDuration // price ranges only hold starting date, this may need to be changed in the future
    );

    template <class TRng>
    Table<ConferenceDayReservation> operator()(TRng& rng) const
    {
        static constexpr float percentFillPerPriceRange = 0.7f;
        static constexpr float studentDiscount = 0.3f;

        DurationGenerator reservationTimeOffsetGenerator(-m_priceRangeDuration, Days{ 8 }); // some will be after 7 day payment time, they should be cancelled by the DB

        std::bernoulli_distribution dIsPaid(m_paymentSaturation);

        Table<ConferenceDayReservation> conferenceDayReservations;

        std::vector<int> participantIndices; // used for choosing a set of participants for each day
        participantIndices.reserve(m_participants->size());
        for (int i = 0; i < m_participants->size(); ++i)
        {
            participantIndices.emplace_back(i);
        }

        Record::IdType id = 0;
        for (auto iter = m_priceRanges->begin(); iter != m_priceRanges->end();)
        {
            auto currentConferenceDay = iter->conferenceDay();
            auto lastConferenceDay = currentConferenceDay;

            int numFreeSpots = currentConferenceDay->numSpots();

            std::shuffle(participantIndices.begin(), participantIndices.end(), rng);

            int participantIndexIndex = 0;
            while (iter != m_priceRanges->end() && currentConferenceDay == lastConferenceDay)
            {
                const auto& priceRange = *iter;

                // one price range at one the same conference day
                const int numSpotsToFill = static_cast<int>(numFreeSpots * percentFillPerPriceRange);
                numFreeSpots -= numSpotsToFill;

                for (int i = 0; i < numSpotsToFill; ++i)
                {
                    const auto& participant = (*m_participants)[participantIndices[participantIndexIndex++]];

                    const DateTime reservationDate = priceRange.startDate() - reservationTimeOffsetGenerator(rng);
                    const bool isStudent = m_students->existsOrdered(&Student::primaryKey, participant.primaryKey());
                    const Price charge = Price(priceRange.price().units() * 
                        (isStudent ? (1.0f - studentDiscount) : 1.0f)
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
                ++iter;
                currentConferenceDay = iter->conferenceDay();
            }
        }

        return conferenceDayReservations;
    }

private:
    const Table<Participant>* m_participants;
    const Table<ConferenceDay>* m_conferenceDays;
    const Table<PriceRange>* m_priceRanges;
    const Table<Student>* m_students;
    float m_paymentSaturation;
    Milliseconds m_priceRangeDuration;
};
