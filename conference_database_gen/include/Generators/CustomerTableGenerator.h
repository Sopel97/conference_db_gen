#pragma once

#include <vector>

#include "ConferenceDatabase/Tables/Customer.h"
#include "ConferenceDatabase/Tables/Person.h"
#include "ConferenceDatabase/Tables/Company.h"
#include "Database/Table.h"

#include "TableGenerator.h"

template <>
class TableGenerator<Customer>
{
public:
    using RecordType = Customer;
    using ResultType = Table<Customer>;

    TableGenerator(
        const Table<Person>& people,
        const Table<Company>& companies,
        float individualCustomerSaturation
    );

    template <class TRng>
    Table<Customer> operator()(TRng& rng) const
    {
        std::bernoulli_distribution dIsIndividualCustomer(m_individualCustomerSaturation);
        Table<Customer> customers;
        {
            float f = 1.0f - m_individualCustomerSaturation;
            f *= f; // since we're dealing with probability reserving is done with a margin
            customers.reserve(static_cast<int>(m_people->size() * (1.0f - f)) + m_companies->size());
        }

        Record::IdType id = 0;
        for (const auto& person : *m_people)
        {
            if (!dIsIndividualCustomer(rng)) continue;

            customers.add(Customer(id++, person, std::nullopt));
        }

        for (const auto& company : *m_companies)
        {
            customers.add(Customer(id++, std::nullopt, company));
        }

        return customers;
    }

private:
    const Table<Person>* m_people;
    const Table<Company>* m_companies;
    float m_individualCustomerSaturation;
};
