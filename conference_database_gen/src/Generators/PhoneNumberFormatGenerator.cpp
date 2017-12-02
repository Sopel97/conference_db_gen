#include "Generators/PhoneNumberFormatGenerator.h"

PhoneNumberFormatGenerator::PhoneNumberFormatGenerator(
    int minSegmentLength,
    int maxSegmentLength,
    int minAreaCodeLength,
    int maxAreaCodeLength,
    int minNumberLength,
    int maxNumberLength,
    float separatorBias
) :
    m_minSegmentLength(minSegmentLength),
    m_maxSegmentLength(maxSegmentLength),
    m_minAreaCodeLength(minAreaCodeLength),
    m_maxAreaCodeLength(maxAreaCodeLength),
    m_minNumberLength(minNumberLength),
    m_maxNumberLength(maxNumberLength),
    m_separatorBias(separatorBias)
{

}
