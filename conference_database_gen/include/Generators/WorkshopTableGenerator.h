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

    TableGenerator(
        const Table<ConferenceDay>& conferenceDays,
        const DictionaryType& nameDictionary,
        const DurationGenerator& offsetFromStartGenerator,
        const DurationGenerator& durationGenerator,
        const PriceGenerator& priceGenerator,
        float priceSaturation,
        int avgNumWorkshopsPerDay
    );

    template <class TRng>
    Table<Workshop> operator()(TRng& rng) const
    {
        static constexpr float minNumSpotsRelative = 0.4f;
        static constexpr float maxNumSpotsRelative = 1.0f;
        static constexpr int minWorkshopNameLength = 20;
        static constexpr int maxOptimalWorkshopNameLength = 35;
        static constexpr int maxWorkshopNameLength = 45;
        static constexpr Milliseconds dateRounding = Minutes{ 5 };
        static constexpr float percentSpotsReserved = 0.2f;

        NameGenerator nameGenerator(*m_nameDictionary, minWorkshopNameLength, maxOptimalWorkshopNameLength, maxWorkshopNameLength);

        std::bernoulli_distribution dHasPrice(m_priceSaturation);
        const int maxWorkshopsPerDay = m_avgNumWorkshopsPerDay * 2 - 1;
        std::uniform_int_distribution<int> dNumWorkshops(1, maxWorkshopsPerDay);
        std::uniform_real_distribution<float> dNumSpotsRelative(minNumSpotsRelative, maxNumSpotsRelative);

        Table<Workshop> workshops;
        workshops.reserve(m_conferenceDays->size() * maxWorkshopsPerDay);

        Record::IdType id = 0;
        for (const auto& conferenceDay : *m_conferenceDays)
        {
            const DateTime& conferenceStartDate = conferenceDay.date();
            const int numWorkshops = dNumWorkshops(rng);

            for (int i = 0; i < numWorkshops; ++i)
            {
                const DateTime workshopStartDate = (conferenceStartDate + m_offsetFromStartGenerator(rng)).rounded(dateRounding);
                const DateTime workshopEndDate = (workshopStartDate + m_durationGenerator(rng)).rounded(dateRounding);
                const Price price = dHasPrice(rng) ? m_priceGenerator(rng) : Price(0);

                const int numSpots = static_cast<int>(dNumSpotsRelative(rng) * conferenceDay.numSpots());
                const int numSpotsReserved = static_cast<int>(numSpots * percentSpotsReserved);

                workshops.add(
                    Workshop(
                        id++,
                        conferenceDay,
                        nameGenerator(rng),
                        workshopStartDate,
                        workshopEndDate,
                        numSpots,
                        numSpotsReserved,
                        true, // all reservations are assumed to be filled
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
