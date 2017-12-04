#pragma once

#include <vector>
#include <optional>

#include "ConferenceDatabase/Tables/Conference.h"
#include "ConferenceDatabase/Tables/ConferenceDay.h"
#include "Database/Table.h"

#include "TableGenerator.h"

#include "DurationGenerator.h"
#include "Common.h"

#include "DateTime.h"

template <>
class TableGenerator<ConferenceDay>
{
public:
    using RecordType = ConferenceDay;
    using ResultType = Table<ConferenceDay>;

    TableGenerator(
        const Table<Conference>& conferences,
        int avgNumDays,
        int minNumSpots,
        int maxNumSpots
    );

    template <class TRng>
    Table<ConferenceDay> operator()(TRng& rng) const
    {
        static constexpr float percentSpotsVariationWithinConference = 0.1f;
        static constexpr Milliseconds minStartOffsetDuration = Hours{ 8 };
        static constexpr Milliseconds maxStartOffsetDuration = Hours{ 12 };
        static constexpr Milliseconds dateRounding = Hours{ 1 };
        static constexpr float percentSpotsReserved = 0.2f;
        
        DurationGenerator startOffsetGenerator(minStartOffsetDuration, maxStartOffsetDuration);

        const int maxNumDays = m_avgNumDays * 2 - 1;

        Table<ConferenceDay> conferenceDays;
        conferenceDays.reserve(m_conferences->size() * maxNumDays);

        std::uniform_int_distribution<int> dNumDays(1, maxNumDays);
        std::uniform_int_distribution<int> dNumSpots(m_minNumSpots, m_maxNumSpots);

        Record::IdType id = 0;
        for(const auto& conference : *m_conferences)
        {
            const int numSpotsApproximate = dNumSpots(rng);
            std::uniform_int_distribution<int> dNumSpotsActual(
                std::max(m_minNumSpots, static_cast<int>(numSpotsApproximate * (1.0f - percentSpotsVariationWithinConference))),
                std::min(m_maxNumSpots, static_cast<int>(numSpotsApproximate * (1.0f + percentSpotsVariationWithinConference)))
            );

            const int numDays = dNumDays(rng);
            for (int d = 0; d < numDays; ++d)
            {
                const int numActualSpots = dNumSpotsActual(rng);
                const int numSpotsReserved = static_cast<int>(numActualSpots * percentSpotsReserved);

                conferenceDays.add(
                    ConferenceDay(
                        id++,
                        conference,
                        (conference.startDate() + Days{ d } +startOffsetGenerator(rng)).rounded(dateRounding),
                        numActualSpots,
                        numSpotsReserved,
                        true // all reservations are assumed to be filled
                    )
                );
            }
        }

        return conferenceDays;
    }

private:
    const Table<Conference>* m_conferences;
    int m_avgNumDays;
    int m_minNumSpots;
    int m_maxNumSpots;
};
