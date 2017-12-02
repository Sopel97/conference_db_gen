#include "Generators/CountryTableGenerator.h"

TableGenerator<Country>::TableGenerator(
    DictionaryType& countryDictionary,
    int numCountries
) :
    m_countryDictionary(&countryDictionary),
    m_numCountries(numCountries)
{

}