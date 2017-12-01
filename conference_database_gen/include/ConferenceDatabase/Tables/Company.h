#pragma once

#include <string>

#include "Database/Record.h"

class Company : public Record
{
public:
    template <
        class TCompanyId, 
        class TCompanyName, 
        class TContactPersonName,
        class TAddress, 
        class TPostalCode, 
        class TCity, 
        class TCountryId, 
        class TPhone, 
        class TEmail
    >
    Company(
        TCompanyId&& companyId, 
        TCompanyName&& companyName, 
        TContactPersonName&& contactPersonName, 
        TAddress&& address, 
        TPostalCode&& postalCode, 
        TCity&& city,
        TCountryId&& countryId, 
        TPhone&& phone, 
        TEmail&& email
    ) :
        m_companyId(std::forward<TCompanyId>(companyId)),
        m_companyName(std::forward<TCompanyName>(companyName)),
        m_contactPersonName(std::forward<TContactPersonName>(contactPersonName)),
        m_address(std::forward<TAddress>(address)),
        m_postalCode(std::forward<TPostalCode>(postalCode)),
        m_city(std::forward<TCity>(city)),
        m_countryId(std::forward<TCountryId>(countryId)),
        m_phone(std::forward<TPhone>(phone)),
        m_email(std::forward<TEmail>(email))
    {

    }

    IdType companyId() const;
    const std::string& companyName() const;
    const std::string& contactPersonName() const;
    const std::string& address() const;
    const std::string& postalCode() const;
    const std::string& city() const;
    IdType countryId() const;
    const std::string& phone() const;
    const std::string& email() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_companyId;
    std::string m_companyName;
    std::string m_contactPersonName;
    std::string m_address;
    std::string m_postalCode;
    std::string m_city;
    IdType m_countryId;
    std::string m_phone;
    std::string m_email;
};
