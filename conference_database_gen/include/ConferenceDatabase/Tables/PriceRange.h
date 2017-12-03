#pragma once

#include <string>

#include "Database/Record.h"
#include "Database/ForeignKey.h"

#include "DateTime.h"
#include "Price.h"

#include "ConferenceDay.h"

class PriceRange : public Record
{
public:
    using PrimaryKeyType = IdType;

    template <class TPriceRangeId, class TConferenceDay, class TStartDate, class TPrice>
    PriceRange(TPriceRangeId&& priceRangeId, TConferenceDay&& conferenceDay, TStartDate&& startDate, TPrice&& price) :
        m_priceRangeId(std::forward<TPriceRangeId>(priceRangeId)),
        m_conferenceDay(std::forward<TConferenceDay>(conferenceDay)),
        m_startDate(std::forward<TStartDate>(startDate)),
        m_price(std::forward<TPrice>(price))
    {

    }

    PriceRange(const PriceRange&) = default;
    PriceRange(PriceRange&&) noexcept = default;

    PriceRange& operator=(const PriceRange&) = default;
    PriceRange& operator=(PriceRange&&) noexcept = default;

    PrimaryKeyType primaryKey() const;

    IdType priceRangeId() const;
    ForeignKey<ConferenceDay> conferenceDay() const;
    DateTime startDate() const;
    Price price() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_priceRangeId;
    ForeignKey<ConferenceDay> m_conferenceDay;
    DateTime m_startDate;
    Price m_price;
};
