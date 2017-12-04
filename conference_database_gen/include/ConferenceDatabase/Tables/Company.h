#pragma once

#include <string>
#include <optional>

#include "Database/Record.h"
#include "Database/ForeignKey.h"

#include "Country.h"

class Company : public Record
{
public:
    using PrimaryKeyType = IdType;

    template <
        class TCompanyId, 
        class TCompanyName, 
        class TContactPersonName,
        class TVatin,
        class TAddress, 
        class TPostalCode, 
        class TCity, 
        class TCountry, 
        class TPhone, 
        class TEmail
    >
    Company(
        TCompanyId&& companyId, 
        TCompanyName&& companyName, 
        TContactPersonName&& contactPersonName, 
        TVatin&& vatin,
        TAddress&& address, 
        TPostalCode&& postalCode, 
        TCity&& city,
        TCountry&& country, 
        TPhone&& phone, 
        TEmail&& email
    ) :
        m_companyId(std::forward<TCompanyId>(companyId)),
        m_companyName(std::forward<TCompanyName>(companyName)),
        m_contactPersonName(std::forward<TContactPersonName>(contactPersonName)),
        m_vatin(std::forward<TVatin>(vatin)),
        m_address(std::forward<TAddress>(address)),
        m_postalCode(std::forward<TPostalCode>(postalCode)),
        m_city(std::forward<TCity>(city)),
        m_country(std::forward<TCountry>(country)),
        m_phone(std::forward<TPhone>(phone)),
        m_email(std::forward<TEmail>(email))
    {

    }

    Company(const Company&) = default;
    Company(Company&&) noexcept = default;

    Company& operator=(const Company&) = default;
    Company& operator=(Company&&) noexcept = default;

    PrimaryKeyType primaryKey() const;

    IdType companyId() const;
    const std::string& companyName() const;
    const std::string& contactPersonName() const;
    const std::optional<std::string>& vatin() const;
    const std::string& address() const;
    const std::string& postalCode() const;
    const std::string& city() const;
    ForeignKey<Country> country() const;
    const std::string& phone() const;
    const std::string& email() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_companyId;
    std::string m_companyName;
    std::string m_contactPersonName;
    std::optional<std::string> m_vatin;
    std::string m_address;
    std::string m_postalCode;
    std::string m_city;
    ForeignKey<Country> m_country;
    std::string m_phone;
    std::string m_email;
};
