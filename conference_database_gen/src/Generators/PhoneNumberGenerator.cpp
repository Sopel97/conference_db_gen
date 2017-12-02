#include "Generators/PhoneNumberGenerator.h"

PhoneNumberGenerator::PhoneNumberGenerator(const std::string& format) :
    m_format(format)
{

}
PhoneNumberGenerator::PhoneNumberGenerator(std::string&& format) :
    m_format(std::move(format))
{

}

