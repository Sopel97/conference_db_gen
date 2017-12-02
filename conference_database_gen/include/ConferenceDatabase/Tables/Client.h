#pragma once

#include <string>
#include <optional>

#include "Database/Record.h"
#include "Database/ForeignKey.h"

#include "Person.h"
#include "Company.h"

class Client : public Record
{
public:
    using PrimaryKeyType = IdType;

    template <class TClientId, class TPerson, class TCompany>
    Client(TClientId&& clientId, TPerson&& person, TCompany&& company) :
        m_clientId(std::forward<TClientId>(clientId)),
        m_person(std::forward<TPerson>(person)),
        m_company(std::forward<TCompany>(company))
    {

    }

    PrimaryKeyType primaryKey() const;

    IdType clientId() const;
    std::optional<ForeignKey<Person>> person() const;
    std::optional<ForeignKey<Company>> company() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_clientId;
    std::optional<ForeignKey<Person>> m_person;
    std::optional<ForeignKey<Company>> m_company;
};
