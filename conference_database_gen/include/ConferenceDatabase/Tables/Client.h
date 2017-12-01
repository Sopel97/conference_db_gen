#pragma once

#include <string>
#include <optional>

#include "Database/Record.h"

class Client : public Record
{
public:
    template <class TClientId, class TPersonId, class TCompanyId>
    Client(TClientId&& clientId, TPersonId&& personId, TCompanyId&& companyId) :
        m_clientId(std::forward<TClientId>(clientId)),
        m_personId(std::forward<TPersonId>(personId)),
        m_companyId(std::forward<TCompanyId>(companyId))
    {

    }

    int clientId() const;
    std::optional<IdType> personId() const;
    std::optional<IdType> companyId() const;

    CsvRecord toCsvRecord() const override;

private:
    int m_clientId;
    std::optional<IdType> m_personId;
    std::optional<IdType> m_companyId;
};
