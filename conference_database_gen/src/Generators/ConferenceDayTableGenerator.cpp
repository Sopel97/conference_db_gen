#include "Generators/ConferenceDayTableGenerator.h"

TableGenerator<ConferenceDay>::TableGenerator(
    const Table<Conference>& conferences,
    int avgNumDays,
    int minNumSpots,
    int maxNumSpots
) :
    m_conferences(&conferences),
    m_avgNumDays(avgNumDays),
    m_minNumSpots(minNumSpots),
    m_maxNumSpots(maxNumSpots)
{

}