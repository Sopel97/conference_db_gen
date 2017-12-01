#pragma once

#include <string>

#include "Database/Record.h"
#include "DateTime.h"

class Conference : public Record
{
public:
    template <class TConferenceId, class TClientId, class TStartDate>
    Conference(TConferenceId&& conferenceId, TClientId&& clientId, TStartDate&& startDate) :
        m_conferenceId(std::forward<TConferenceId>(conferenceId)),
        m_clientId(std::forward<TClientId>(clientId)),
        m_startDate(std::forward<TStartDate>(startDate))
    {

    }

    IdType conferenceId() const;
    IdType clientId() const;
    const DateTime& startDate() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_conferenceId;
    IdType m_clientId;
    DateTime m_startDate;
};
