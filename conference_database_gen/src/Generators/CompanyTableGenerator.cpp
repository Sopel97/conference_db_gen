#include "Generators/CompanyTableGenerator.h"

TableGenerator<Company>::TableGenerator(
    DictionaryType& companyNameDictionary,
    const std::vector<std::string>& firstNames,
    const std::vector<std::string>& lastNames,
    const DictionaryType& addressDictionary,
    const DictionaryType& postalCodeDictionary,
    const Table<Country>& countries,
    const std::vector<std::vector<std::string>>& cityNamesByCountry,
    const std::vector<PhoneNumberGenerator>& phoneNumberGenerators,
    int numCompanies
) :
    m_companyNameDictionary(&companyNameDictionary),
    m_firstNames(&firstNames),
    m_lastNames(&lastNames),
    m_addressDictionary(&addressDictionary),
    m_postalCodeDictionary(&postalCodeDictionary),
    m_countries(&countries),
    m_cityNamesByCountry(&cityNamesByCountry),
    m_phoneNumberGenerators(&phoneNumberGenerators),
    m_numCompanies(numCompanies)
{

}