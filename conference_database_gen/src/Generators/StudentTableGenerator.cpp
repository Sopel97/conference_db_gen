#include "Generators/StudentTableGenerator.h"

TableGenerator<Student>::TableGenerator(
    const Table<Person>& people,
    float studentSaturation
) :
    m_people(&people),
    m_studentSaturation(studentSaturation)
{

}
