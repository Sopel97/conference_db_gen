#include "Generators/PriceGenerator.h"

PriceGenerator::PriceGenerator(Price min, Price max) :
    m_distr(min.cents(), max.cents())
{

}
