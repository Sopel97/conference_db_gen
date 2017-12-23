#pragma once

#include <vector>
#include <optional>

#include "ConferenceDatabase/Tables/Workshop.h"
#include "ConferenceDatabase/Tables/ConferenceDay.h"
#include "ConferenceDatabase/Tables/ConferenceDayReservation.h"
#include "Database/Table.h"

#include "TableGenerator.h"

#include "MarkovChainsDictionary.h"
#include "NameGenerator.h"
#include "DurationGenerator.h"
#include "PriceGenerator.h"

#include "DateTime.h"

template <>
class TableGenerator<Workshop>
{
public:
    using RecordType = Workshop;
    using ResultType = Table<Workshop>;
    using DictionaryType = MarkovChainsDictionary<std::string>;

    struct Params
    {
        const Table<ConferenceDay>* conferenceDays;
        NameGenerator nameGenerator;
        DurationGenerator offsetFromStartGenerator;
        DurationGenerator durationGenerator;
        PriceGenerator priceGenerator;
        float priceSaturation;
        int avgNumWorkshopsPerDay;
        float minNumSpotsRelative;
        float maxNumSpotsRelative;
        Milliseconds dateRounding;
    };

    TableGenerator(const Params& params);

    template <class TRng>
    Table<Workshop> operator()(TRng& rng) const
    {
        std::bernoulli_distribution dHasPrice(m_params.priceSaturation);
        const int maxWorkshopsPerDay = m_params.avgNumWorkshopsPerDay * 2 - 1;
        std::uniform_int_distribution<int> dNumWorkshops(1, maxWorkshopsPerDay);
        std::uniform_real_distribution<float> dNumSpotsRelative(m_params.minNumSpotsRelative, m_params.maxNumSpotsRelative);

        Table<Workshop> workshops;
        workshops.reserve(m_params.conferenceDays->size() * maxWorkshopsPerDay);

        Record::IdType id = 0;
        for (const auto& conferenceDay : *m_params.conferenceDays)
        {
            const DateTime& conferenceStartDate = conferenceDay.date();
            const int numWorkshops = dNumWorkshops(rng);

            for (int i = 0; i < numWorkshops; ++i)
            {
                const DateTime workshopStartDate = (conferenceStartDate + m_params.offsetFromStartGenerator(rng)).rounded(m_params.dateRounding);
                const DateTime workshopEndDate = (workshopStartDate + m_params.durationGenerator(rng)).rounded(m_params.dateRounding);
                const Price price = dHasPrice(rng) ? m_params.priceGenerator(rng) : Price(0);

                const int numSpots = static_cast<int>(dNumSpotsRelative(rng) * conferenceDay.numSpots());

                workshops.add(
                    Workshop(
                        id++,
                        conferenceDay,
                        m_params.nameGenerator(rng),
                        workshopStartDate,
                        workshopEndDate,
                        numSpots,
                        price.rounded(Price::centsPerUnit)
                    )
                );
            }
        }

        return workshops;
    }

private:
    Params m_params;
};
