#pragma once

#include <string>
#include <optional>

#include "Database/Record.h"

class Country : public Record
{
public:
    using PrimaryKeyType = IdType;

    template <class TCountryId, class TCountryName>
    Country(TCountryId&& countryId, TCountryName&& countryName) :
        m_countryId(std::forward<TCountryId>(countryId)),
        m_countryName(std::forward<TCountryName>(countryName))
    {

    }

    Country(const Country&) = default;
    Country(Country&&) noexcept = default;

    Country& operator=(const Country&) = default;
    Country& operator=(Country&&) noexcept = default;

    PrimaryKeyType primaryKey() const;

    IdType countryId() const;
    const std::string& countryName() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_countryId;
    std::string m_countryName;
};
