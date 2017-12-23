#pragma once

#include <vector>
#include <optional>

#include "ConferenceDatabase/Tables/Conference.h"
#include "ConferenceDatabase/Tables/Customer.h"
#include "Database/Table.h"

#include "TableGenerator.h"

#include "DateTimeGenerator.h"
#include "NameGenerator.h"
#include "Common.h"

#include "DateTime.h"

template <>
class TableGenerator<Conference>
{
public:
    using RecordType = Conference;
    using ResultType = Table<Conference>;

    struct Params
    {
        const Table<Customer>* customers;
        NameGenerator nameGenerator;
        DateTimeGenerator startingDateGenerator;
        int numConferences;
        Milliseconds dateRounding;
    };

    TableGenerator(const Params& params);

    template <class TRng>
    Table<Conference> operator()(TRng& rng) const
    {
        Table<Conference> conferences;
        conferences.reserve(m_params.numConferences);

        Record::IdType id = 0;
        for (int i = 0; i < m_params.numConferences; ++i)
        {
            conferences.add(
                Conference(
                    id++,
                    Common::chooseSqr(m_params.customers->records(), rng),
                    m_params.nameGenerator(rng),
                    m_params.startingDateGenerator(rng).rounded(m_params.dateRounding)
                )
            );
        }

        return conferences;
    }

private:
    Params m_params;
};
