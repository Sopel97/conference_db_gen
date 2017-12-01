#pragma once

#include <string>
#include <optional>

#include "Database/Record.h"

class Country : public Record
{
public:
    template <class TCountryId, class TCountryName>
    Country(TCountryId&& countryId, TCountryName&& countryName) :
        m_countryId(std::forward<TCountryId>(countryId)),
        m_countryName(std::forward<TCountryName>(countryName))
    {

    }

    IdType countryId() const;
    const std::string& countryName() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_countryId;
    std::string m_countryName;
};
