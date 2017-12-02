#pragma once

#include <vector>
#include <optional>

#include "ConferenceDatabase/Tables/Workshop.h"
#include "ConferenceDatabase/Tables/ConferenceDay.h"
#include "Database/Table.h"

#include "TableGenerator.h"

#include "MarkovChainsDictionary.h"
#include "NameGenerator.h"
#include "DateTimeGenerator.h"
#include "DurationGenerator.h"
#include "PriceGenerator.h"
#include "Common.h"

#include "DateTime.h"

template <>
class TableGenerator<Workshop>
{
public:
    using RecordType = Workshop;
    using ResultType = Table<Workshop>;
    using DictionaryType = MarkovChainsDictionary<std::string>;

    TableGenerator(
        const Table<ConferenceDay>& conferenceDays,
        const DictionaryType& nameDictionary,
        const DurationGenerator& offsetFromStartGenerator,
        const DurationGenerator& durationGenerator,
        const PriceGenerator& priceGenerator,
        float priceSaturation,
        int avgNumWorkshopsPerDay
    ) :
        m_conferenceDays(&conferenceDays),
        m_nameDictionary(&nameDictionary),
        m_offsetFromStartGenerator(offsetFromStartGenerator),
        m_durationGenerator(durationGenerator),
        m_priceGenerator(priceGenerator),
        m_priceSaturation(priceSaturation),
        m_avgNumWorkshopsPerDay(avgNumWorkshopsPerDay)
    {

    }

    template <class TRng>
    Table<Workshop> operator()(TRng& rng) const
    {
        static constexpr float minNumSpotsRelative = 0.4f;
        static constexpr float maxNumSpotsRelative = 1.0f;

        NameGenerator nameGenerator(*m_nameDictionary, 20, 35, 45);

        std::bernoulli_distribution dHasPrice(m_priceSaturation);
        std::uniform_int_distribution<int> dNumWorkshops(1, m_avgNumWorkshopsPerDay * 2 - 1);
        std::uniform_real_distribution<float> dNumSpotsRelative(minNumSpotsRelative, maxNumSpotsRelative);

        Table<Workshop> workshops;

        Record::IdType id = 0;
        for (const auto& conferenceDay : *m_conferenceDays)
        {
            const DateTime& conferenceStartDate = conferenceDay.date();
            const int numWorkshops = dNumWorkshops(rng);

            for (int i = 0; i < numWorkshops; ++i)
            {
                const DateTime workshopStartDate = (conferenceStartDate + m_offsetFromStartGenerator(rng)).rounded(Minutes{ 5 });
                const DateTime workshopEndDate = (workshopStartDate + m_durationGenerator(rng)).rounded(Minutes{ 5 });
                const Price price = dHasPrice(rng) ? m_priceGenerator(rng) : Price(0);

                workshops.add(
                    Workshop(
                        id++,
                        conferenceDay,
                        nameGenerator(rng),
                        workshopStartDate,
                        workshopEndDate,
                        static_cast<int>(dNumSpotsRelative(rng) * conferenceDay.numSpots()),
                        price.rounded(Price::centsPerUnit)
                    )
                );
            }
        }

        return workshops;
    }

private:
    const Table<ConferenceDay>* m_conferenceDays;
    const DictionaryType* m_nameDictionary;
    DurationGenerator m_offsetFromStartGenerator;
    DurationGenerator m_durationGenerator;
    PriceGenerator m_priceGenerator;
    float m_priceSaturation;
    int m_avgNumWorkshopsPerDay;
};