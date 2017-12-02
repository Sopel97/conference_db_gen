#pragma once

#include <vector>
#include <optional>

#include "ConferenceDatabase/Tables/Participant.h"
#include "ConferenceDatabase/Tables/Person.h"
#include "ConferenceDatabase/Tables/Company.h"
#include "Database/Table.h"

#include "TableGenerator.h"

#include "Common.h"

template <>
class TableGenerator<Participant>
{
public:
    using RecordType = Participant;
    using ResultType = Table<Participant>;

    TableGenerator(
        const Table<Person>& people,
        const Table<Company>& companies,
        float participantSaturation,
        float fromCompanySaturation
    );

    template <class TRng>
    Table<Participant> operator()(TRng& rng) const
    {
        std::bernoulli_distribution dIsParticipant(m_participantSaturation);
        std::bernoulli_distribution dIsFromCompany(m_fromCompanySaturation);
        Table<Participant> participants;

        Record::IdType id = 0;
        for (const auto& person : *m_people)
        {
            if (!dIsParticipant(rng)) continue;

            participants.add(
                Participant(
                    id++,
                    person,
                    dIsFromCompany(rng) ? std::make_optional<ForeignKey<Company>>(Common::chooseSqr(m_companies->records(), rng)) : std::nullopt
                )
            );

        }

        return participants;
    }

private:
    const Table<Person>* m_people;
    const Table<Company>* m_companies;
    float m_participantSaturation;
    float m_fromCompanySaturation;
};
