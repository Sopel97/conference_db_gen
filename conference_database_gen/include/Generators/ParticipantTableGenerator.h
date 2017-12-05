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

    struct Params
    {
        const Table<Person>* people;
        const Table<Company>* companies;
        float participantSaturation;
        float fromCompanySaturation;
    };

    TableGenerator(const Params& params);

    template <class TRng>
    Table<Participant> operator()(TRng& rng) const
    {
        std::bernoulli_distribution dIsParticipant(m_params.participantSaturation);
        std::bernoulli_distribution dIsFromCompany(m_params.fromCompanySaturation);
        Table<Participant> participants;
        {
            float f = 1.0f - m_params.participantSaturation;
            f *= f; // since we're dealing with probability reserving is done with a margin
            participants.reserve(static_cast<int>(m_params.people->size() * (1.0f - f)));
        }

        Record::IdType id = 0;
        for (const auto& person : *(m_params.people))
        {
            if (!dIsParticipant(rng)) continue;

            participants.add(
                Participant(
                    id++,
                    person,
                    dIsFromCompany(rng) ? std::make_optional<ForeignKey<Company>>(Common::chooseSqr(m_params.companies->records(), rng)) : std::nullopt
                )
            );

        }

        return participants;
    }

private:
    Params m_params;
};
