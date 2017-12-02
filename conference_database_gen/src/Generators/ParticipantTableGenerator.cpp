#include "Generators/ParticipantTableGenerator.h"

TableGenerator<Participant>::TableGenerator(
    const Table<Person>& people,
    const Table<Company>& companies,
    float participantSaturation,
    float fromCompanySaturation
) :
    m_people(&people),
    m_companies(&companies),
    m_participantSaturation(participantSaturation),
    m_fromCompanySaturation(fromCompanySaturation)
{

}
