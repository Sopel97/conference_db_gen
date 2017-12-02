#include "Generators/PriceRangeTableGenerator.h"

TableGenerator<PriceRange>::TableGenerator(
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
