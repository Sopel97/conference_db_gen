#pragma once

#include <vector>
#include <optional>

#include "ConferenceDatabase/Tables/PriceRange.h"
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
class TableGenerator<PriceRange>
{
public:
    using RecordType = PriceRange;
    using ResultType = Table<PriceRange>;

    TableGenerator(
        const Table<ConferenceDay>& conferenceDays,
        Price minInitialPrice,
        Price maxInitialPrice,
        Milliseconds firstAvailableBefore,
        Milliseconds priceIncreaseAfter,
        float percentIcrease,
        int numIncreases
    ) :
        m_conferenceDays(&conferenceDays),
        m_minInitialPrice(minInitialPrice),
        m_maxInitialPrice(maxInitialPrice),
        m_firstAvailableBefore(firstAvailableBefore),
        m_priceIncreaseAfter(priceIncreaseAfter),
        m_percentIcrease(percentIcrease),
        m_numIncreases(numIncreases)
    {

    }

    template <class TRng>
    Table<PriceRange> operator()(TRng& rng) const
    {
        PriceGenerator priceGenerator(m_minInitialPrice, m_maxInitialPrice);

        Table<PriceRange> priceRanges;

        Record::IdType id = 0;
        for (const auto& conferenceDay : *m_conferenceDays)
        {
            const auto& startDate = conferenceDay.date();
            const DateTime firstPriceDate = startDate - m_firstAvailableBefore;
            const Price firstPrice = priceGenerator(rng).rounded(Price::centsPerUnit);

            DateTime date = firstPriceDate;
            Price price = firstPrice;
            for (int i = 0; i < m_numIncreases; ++i)
            {
                priceRanges.add(
                    PriceRange(
                        id++,
                        conferenceDay,
                        date,
                        price
                    )
                );

                date += m_priceIncreaseAfter;
                price = Price(price.units() * (1.0f + m_percentIcrease)).rounded(Price::centsPerUnit);
            }
        }

        return priceRanges;
    }

private:
    const Table<ConferenceDay>* m_conferenceDays;
    Price m_minInitialPrice;
    Price m_maxInitialPrice;
    Milliseconds m_firstAvailableBefore;
    Milliseconds m_priceIncreaseAfter;
    float m_percentIcrease;
    int m_numIncreases;
};
