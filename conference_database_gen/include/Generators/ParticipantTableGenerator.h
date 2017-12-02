#pragma once

#include <vector>
#include <optional>

#include "ConferenceDatabase/Tables/Participant.h"
#include "ConferenceDatabase/Tables/Person.h"
#include "ConferenceDatabase/Tables/Company.h"
#include "Database/Table.h"

#include "TableGenerator.h"

#include "MarkovChainsDictionary.h"
#include "NameGenerator.h"
#include "Common.h"

template <>
class TableGenerator<Participant>
{
public:
    using RecordType = Participant;
    using ResultType = Table<Participant>;

    TableGenerator(
        const Table<Person>& people,
        float participantSaturation
    ) :
        m_people(&people),
        m_participantSaturation(participantSaturation)
    {

    }

    template <class TRng>
    Table<Participant> operator()(TRng& rng) const
    {
        std::bernoulli_distribution dIsParticipant(m_participantSaturation);
        Table<Participant> participants;

        Record::IdType id = 0;
        for (const auto& person : *m_people)
        {
            if (!dIsParticipant(rng)) continue;

            participants.add(
                Participant(
                    id++, 
                    person
                )
            );

        }

        return participants;
    }

private:
    const Table<Person>* m_people;
    float m_participantSaturation;
};
