#pragma once

#include <string>

#include "Database/Record.h"
#include "Database/ForeignKey.h"
#include "DateTime.h"

#include "Customer.h"

class Conference : public Record
{
public:
    using PrimaryKeyType = IdType;

    template <class TConferenceId, class TCustomer, class TName, class TStartDate>
    Conference(TConferenceId&& conferenceId, TCustomer&& customer, TName&& conferenceName, TStartDate&& startDate) :
        m_conferenceId(std::forward<TConferenceId>(conferenceId)),
        m_customer(std::forward<TCustomer>(customer)),
        m_conferenceName(std::forward<TName>(conferenceName)),
        m_startDate(std::forward<TStartDate>(startDate))
    {

    }

    Conference(const Conference&) = default;
    Conference(Conference&&) noexcept = default;

    Conference& operator=(const Conference&) = default;
    Conference& operator=(Conference&&) noexcept = default;

    PrimaryKeyType primaryKey() const;

    IdType conferenceId() const;
    ForeignKey<Customer> customer() const;
    const std::string& conferenceName() const;
    const DateTime& startDate() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_conferenceId;
    ForeignKey<Customer> m_customer;
    std::string m_conferenceName;
    DateTime m_startDate;
};
