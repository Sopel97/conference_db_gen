#include "Generators/NameGenerator.h"

NameGenerator::NameGenerator(const DictionaryType& dictionary, int minLength, int maxOptimalLength, int maxLength) :
    m_dictionary(&dictionary),
    m_minLength(minLength),
    m_maxOptimalLength(maxOptimalLength),
    m_maxLength(maxLength)
{

}
