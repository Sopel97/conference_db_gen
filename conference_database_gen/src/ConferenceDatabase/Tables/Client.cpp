#include "ConferenceDatabase/Tables/Client.h"

#include "Csv/CsvRecord.h"

int Client::clientId() const
{
    return m_clientId;
}

std::optional<Record::IdType> Client::personId() const
{
    return m_personId;
}

std::optional<Record::IdType> Client::companyId() const
{
    return m_companyId;
}

CsvRecord Client::toCsvRecord() const
{
    return CsvRecord(
        std::to_string(m_clientId),
        m_personId.has_value() ? std::to_string(m_personId.value()) : "NULL",
        m_companyId.has_value() ? std::to_string(m_companyId.value()) : "NULL"
    );
}
