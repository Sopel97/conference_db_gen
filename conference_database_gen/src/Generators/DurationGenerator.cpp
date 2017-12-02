#include "Generators/DurationGenerator.h"

DurationGenerator::DurationGenerator(Milliseconds min, Milliseconds max) :
    m_distr(min.count(), max.count())
{

}
