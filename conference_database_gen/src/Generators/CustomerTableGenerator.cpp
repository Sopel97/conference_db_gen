#include "Generators/CustomerTableGenerator.h"

TableGenerator<Customer>::TableGenerator(
    const Table<Person>& people,
    const Table<Company>& companies,
    float individualCustomerSaturation
) :
    m_people(&people),
    m_companies(&companies),
    m_individualCustomerSaturation(individualCustomerSaturation)
{

}