#pragma once

#include <vector>
#include <optional>

#include "ConferenceDatabase/Tables/Conference.h"
#include "ConferenceDatabase/Tables/Customer.h"
#include "Database/Table.h"

#include "TableGenerator.h"

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
        const Table<Customer>& customers,
        const DateTimeGenerator& startingDateGenerator,
        int numConferences
    );

    template <class TRng>
    Table<Conference> operator()(TRng& rng) const
    {
        static constexpr Milliseconds dateRounding = Hours{ 1 };

        Table<Conference> conferences;
        conferences.reserve(m_numConferences);

        Record::IdType id = 0;
        for (int i = 0; i < m_numConferences; ++i)
        {
            conferences.add(
                Conference(
                    id++, 
                    Common::chooseSqr(m_customers->records(), rng), 
                    m_startingDateGenerator(rng).rounded(dateRounding)
                )
            );
        }

        return conferences;
    }

private:
    const Table<Customer>* m_customers;
    DateTimeGenerator m_startingDateGenerator;
    int m_numConferences;
};
