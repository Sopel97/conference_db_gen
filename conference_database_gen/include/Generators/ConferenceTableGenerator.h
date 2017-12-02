#pragma once

#include <vector>
#include <optional>

#include "ConferenceDatabase/Tables/Conference.h"
#include "ConferenceDatabase/Tables/Client.h"
#include "Database/Table.h"

#include "TableGenerator.h"

#include "MarkovChainsDictionary.h"
#include "NameGenerator.h"
#include "DateTimeGenerator.h"
#include "Common.h"

#include "DateTime.h"

template <>
class TableGenerator<Conference>
{
public:
    using RecordType = Conference;
    using ResultType = Table<Conference>;

    TableGenerator(
        const Table<Client>& clients,
        const DateTimeGenerator& startingDateGenerator,
        int numConferences
    ) :
        m_clients(&clients),
        m_startingDateGenerator(startingDateGenerator),
        m_numConferences(numConferences)
    {

    }

    template <class TRng>
    Table<Conference> operator()(TRng& rng) const
    {
        Table<Conference> conferences;

        Record::IdType id = 0;
        for (int i = 0; i < m_numConferences; ++i)
        {
            conferences.add(
                Conference(
                    id++, 
                    Common::chooseSqr(m_clients->records(), rng), 
                    m_startingDateGenerator(rng).rounded(Days{ 1 })
                )
            );
        }

        return conferences;
    }

private:
    const Table<Client>* m_clients;
    DateTimeGenerator m_startingDateGenerator;
    int m_numConferences;
};
