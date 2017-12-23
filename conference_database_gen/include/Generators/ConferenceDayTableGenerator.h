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

    struct Params
    {
        const Table<Conference>* conferences;
        int avgNumDays;
        int minNumSpots;
        int maxNumSpots;
        float percentSpotsVariationWithinConference;
        Milliseconds minStartOffset;
        Milliseconds maxStartOffset;
        Milliseconds dateRounding;
    };

    TableGenerator(const Params& params);

    template <class TRng>
    Table<ConferenceDay> operator()(TRng& rng) const
    {
        DurationGenerator startOffsetGenerator(m_params.minStartOffset, m_params.maxStartOffset);

        const int maxNumDays = m_params.avgNumDays * 2 - 1;

        Table<ConferenceDay> conferenceDays;
        conferenceDays.reserve(m_params.conferences->size() * maxNumDays);

        std::uniform_int_distribution<int> dNumDays(1, maxNumDays);
        std::uniform_int_distribution<int> dNumSpots(m_params.minNumSpots, m_params.maxNumSpots);

        Record::IdType id = 0;
        for(const auto& conference : *m_params.conferences)
        {
            const int numSpotsApproximate = dNumSpots(rng);
            std::uniform_int_distribution<int> dNumSpotsActual(
                std::max(m_params.minNumSpots, static_cast<int>(numSpotsApproximate * (1.0f - m_params.percentSpotsVariationWithinConference))),
                std::min(m_params.maxNumSpots, static_cast<int>(numSpotsApproximate * (1.0f + m_params.percentSpotsVariationWithinConference)))
            );

            const int numDays = dNumDays(rng);
            for (int d = 0; d < numDays; ++d)
            {
                const int numActualSpots = dNumSpotsActual(rng);

                conferenceDays.add(
                    ConferenceDay(
                        id++,
                        conference,
                        (conference.startDate() + Days{ d } +startOffsetGenerator(rng)).rounded(m_params.dateRounding),
                        numActualSpots
                    )
                );
            }
        }

        return conferenceDays;
    }

private:
    Params m_params;
};
