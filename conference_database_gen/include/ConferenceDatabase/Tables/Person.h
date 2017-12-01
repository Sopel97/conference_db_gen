#pragma once

#include <string>

#include "Database/Record.h"

#include "DateTime.h"

class Person : public Record
{
public:
    template <
        class TPersonId, 
        class TFirstName, 
        class TLastName, 
        class TBirthDate, 
        class TAddress, 
        class TPostalCode, 
        class TCity, 
        class TCountryId, 
        class TPhone, 
        class TEmail
    >
    Person(
        TPersonId&& personId, 
        TFirstName&& firstName, 
        TLastName&& lastName, 
        TBirthDate&& birthDate, 
        TAddress&& address, 
        TPostalCode&& postalCode,
        TCity&& city, 
        TCountryId&& countryId, 
        TPhone&& phone, 
        TEmail&& email
    ) :
        m_personId(std::forward<TPersonId>(personId)),
        m_firstName(std::forward<TFirstName>(firstName)),
        m_lastName(std::forward<TLastName>(lastName)),
        m_birthDate(std::forward<TBirthDate>(birthDate)),
        m_address(std::forward<TAddress>(address)),
        m_postalCode(std::forward<TPostalCode>(postalCode)),
        m_city(std::forward<TCity>(city)),
        m_countryId(std::forward<TCountryId>(countryId)),
        m_phone(std::forward<TPhone>(phone)),
        m_email(std::forward<TEmail>(email))
    {

    }

    IdType personId() const;
    const std::string& firstName() const;
    const std::string& lastName() const;
    const DateTime& birthDate() const;
    const std::string& address() const;
    const std::string& postalCode() const;
    const std::string& city() const;
    IdType countryId() const;
    const std::string& phone() const;
    const std::string& email() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_personId;
    std::string m_firstName;
    std::string m_lastName;
    DateTime m_birthDate;
    std::string m_address;
    std::string m_postalCode;
    std::string m_city;
    IdType m_countryId;
    std::string m_phone;
    std::string m_email;
};
