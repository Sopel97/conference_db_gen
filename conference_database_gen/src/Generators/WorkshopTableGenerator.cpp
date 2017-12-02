#include "Generators/WorkshopTableGenerator.h"

TableGenerator<Workshop>::TableGenerator(
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
