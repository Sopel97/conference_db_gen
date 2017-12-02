#pragma once

#include <vector>

#include "ConferenceDatabase/Tables/Client.h"
#include "ConferenceDatabase/Tables/Person.h"
#include "ConferenceDatabase/Tables/Company.h"
#include "Database/Table.h"

#include "TableGenerator.h"

#include "MarkovChainsDictionary.h"
#include "NameGenerator.h"
#include "Common.h"

template <>
class TableGenerator<Client>
{
public:
    using RecordType = Client;
    using ResultType = Table<Client>;

    TableGenerator(
        const Table<Person>& people,
        const Table<Company>& companies,
        float individualClientSaturation
    ) :
        m_people(&people),
        m_companies(&companies),
        m_individualClientSaturation(individualClientSaturation)
    {

    }

    template <class TRng>
    Table<Client> operator()(TRng& rng) const
    {
        std::bernoulli_distribution dIsIndividualClient(m_individualClientSaturation);
        Table<Client> clients;

        Record::IdType id = 0;
        for (const auto& person : *m_people)
        {
            if (!dIsIndividualClient(rng)) continue;

            clients.add(Client(id++, person.personId(), std::nullopt));
        }

        for (const auto& company : *m_companies)
        {
            clients.add(Client(id++, std::nullopt, company.companyId()));
        }

        return clients;
    }

private:
    const Table<Person>* m_people;
    const Table<Company>* m_companies;
    float m_individualClientSaturation;
};
