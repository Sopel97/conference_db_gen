#include "Generators/PersonTableGenerator.h"

TableGenerator<Person>::TableGenerator(
    const std::vector<std::string>& firstNames, 
    const std::vector<std::string>& lastNames,
    const DateTimeGenerator& birthDateGenerator,
    const DictionaryType& addressDictionary,
    const DictionaryType& postalCodeDictionary,
    const Table<Country>& countries,
    const std::vector<std::vector<std::string>>& cityNamesByCountry,
    const std::vector<PhoneNumberGenerator>& phoneNumberGenerators,
    int numRecords
) :
    m_firstNames(&firstNames),
    m_lastNames(&lastNames),
    m_birthDateGenerator(birthDateGenerator),
    m_addressDictionary(&addressDictionary),
    m_postalCodeDictionary(&postalCodeDictionary),
    m_countries(&countries),
    m_cityNamesByCountry(&cityNamesByCountry),
    m_phoneNumberGenerators(&phoneNumberGenerators),
    m_numRecords(numRecords)
{

}
