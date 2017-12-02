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
        static constexpr float numSpotsVariationWithinConference = 0.1f;
        
        DurationGenerator startOffsetGenerator(Hours{ 8 }, Hours{ 12 });

        Table<ConferenceDay> conferenceDays;

        std::uniform_int_distribution<int> dNumDays(1, m_avgNumDays * 2 - 1);
        std::uniform_int_distribution<int> dNumSpots(m_minNumSpots, m_maxNumSpots);

        Record::IdType id = 0;
        for(const auto& conference : *m_conferences)
        {
            const int numSpotsAround = dNumSpots(rng);
            std::uniform_int_distribution<int> dNumSpotsActual(
                std::max(m_minNumSpots, static_cast<int>(numSpotsAround * (1.0f - numSpotsVariationWithinConference))),
                std::min(m_maxNumSpots, static_cast<int>(numSpotsAround * (1.0f + numSpotsVariationWithinConference)))
            );

            const int numDays = dNumDays(rng);
            for (int d = 0; d < numDays; ++d)
            {
                conferenceDays.add(
                    ConferenceDay(
                        id++,
                        conference,
                        (conference.startDate() + Days{ d } +startOffsetGenerator(rng)).rounded(Hours{ 1 }),
                        dNumSpotsActual(rng)
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
