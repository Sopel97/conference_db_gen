#pragma once

#include <vector>
#include <optional>

#include "ConferenceDatabase/Tables/PriceRange.h"
#include "ConferenceDatabase/Tables/ConferenceDay.h"
#include "Database/Table.h"

#include "TableGenerator.h"

#include "PriceGenerator.h"

#include "DateTime.h"

template <>
class TableGenerator<PriceRange>
{
public:
    using RecordType = PriceRange;
    using ResultType = Table<PriceRange>;

    struct Params
    {
        const Table<ConferenceDay>* conferenceDays;
        Price minInitialPrice;
        Price maxInitialPrice;
        Milliseconds firstAvailableBefore;
        Milliseconds priceIncreaseAfter;
        float percentIcrease;
        int numIncreases;
    };

    TableGenerator(const Params& params);

    template <class TRng>
    Table<PriceRange> operator()(TRng& rng) const
    {
        PriceGenerator priceGenerator(m_params.minInitialPrice, m_params.maxInitialPrice);

        Table<PriceRange> priceRanges;
        priceRanges.reserve(m_params.conferenceDays->size() * m_params.numIncreases);

        Record::IdType id = 0;
        for (const auto& conferenceDay : *m_params.conferenceDays)
        {
            const auto& startDate = conferenceDay.date();
            const DateTime firstPriceDate = startDate - m_params.firstAvailableBefore;
            const Price firstPrice = priceGenerator(rng).rounded(Price::centsPerUnit);

            DateTime date = firstPriceDate;
            Price price = firstPrice;
            for (int i = 0; i < m_params.numIncreases; ++i)
            {
                priceRanges.add(
                    PriceRange(
                        id++,
                        conferenceDay,
                        date,
                        price
                    )
                );

                date += m_params.priceIncreaseAfter;
                price = Price(price.units() * (1.0f + m_params.percentIcrease)).rounded(Price::centsPerUnit);
            }
        }

        return priceRanges;
    }

private:
    Params m_params;
};
