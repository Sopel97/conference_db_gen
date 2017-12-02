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

    template <class TConferenceId, class TCustomer, class TStartDate>
    Conference(TConferenceId&& conferenceId, TCustomer&& customer, TStartDate&& startDate) :
        m_conferenceId(std::forward<TConferenceId>(conferenceId)),
        m_customer(std::forward<TCustomer>(customer)),
        m_startDate(std::forward<TStartDate>(startDate))
    {

    }

    PrimaryKeyType primaryKey() const;

    IdType conferenceId() const;
    ForeignKey<Customer> customer() const;
    const DateTime& startDate() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_conferenceId;
    ForeignKey<Customer> m_customer;
    DateTime m_startDate;
};
