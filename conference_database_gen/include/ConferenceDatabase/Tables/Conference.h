#pragma once

#include <string>

#include "Database/Record.h"
#include "Database/ForeignKey.h"
#include "DateTime.h"

#include "Client.h"

class Conference : public Record
{
public:
    using PrimaryKeyType = IdType;

    template <class TConferenceId, class TClient, class TStartDate>
    Conference(TConferenceId&& conferenceId, TClient&& client, TStartDate&& startDate) :
        m_conferenceId(std::forward<TConferenceId>(conferenceId)),
        m_client(std::forward<TClient>(client)),
        m_startDate(std::forward<TStartDate>(startDate))
    {

    }

    PrimaryKeyType primaryKey() const;

    IdType conferenceId() const;
    ForeignKey<Client> client() const;
    const DateTime& startDate() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_conferenceId;
    ForeignKey<Client> m_client;
    DateTime m_startDate;
};
