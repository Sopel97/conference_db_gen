#pragma once

#include <string>
#include <optional>

#include "Database/Record.h"
#include "Database/ForeignKey.h"

#include "Person.h"
#include "Company.h"

class Customer : public Record
{
public:
    using PrimaryKeyType = IdType;

    template <class TCustomerId, class TPerson, class TCompany>
    Customer(TCustomerId&& customerId, TPerson&& person, TCompany&& company) :
        m_customerId(std::forward<TCustomerId>(customerId)),
        m_person(std::forward<TPerson>(person)),
        m_company(std::forward<TCompany>(company))
    {

    }

    Customer(const Customer&) = default;
    Customer(Customer&&) noexcept = default;

    Customer& operator=(const Customer&) = default;
    Customer& operator=(Customer&&) noexcept = default;

    PrimaryKeyType primaryKey() const;

    IdType customerId() const;
    std::optional<ForeignKey<Person>> person() const;
    std::optional<ForeignKey<Company>> company() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_customerId;
    std::optional<ForeignKey<Person>> m_person;
    std::optional<ForeignKey<Company>> m_company;
};
