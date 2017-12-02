#include "Generators/ConferenceTableGenerator.h"

TableGenerator<Conference>::TableGenerator(
    const Table<Customer>& customers,
    const DateTimeGenerator& startingDateGenerator,
    int numConferences
) :
    m_customers(&customers),
    m_startingDateGenerator(startingDateGenerator),
    m_numConferences(numConferences)
{

}
