#include "Generators/DateTimeGenerator.h"

DateTimeGenerator::DateTimeGenerator(DateTime min, DateTime max) :
    m_distr(min.milliseconds().count(), max.milliseconds().count())
{

}
