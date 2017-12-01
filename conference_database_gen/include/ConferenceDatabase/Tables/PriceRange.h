#pragma once

#include <string>

#include "Database/Record.h"
#include "DateTime.h"
#include "Price.h"

class PriceRange : public Record
{
public:
    template <class TPriceRangeId, class TStartDate, class TPrice>
    PriceRange(TPriceRangeId&& priceRangeId, TStartDate&& startDate, TPrice&& price) :
        m_priceRangeId(std::forward<TPriceRangeId>(priceRangeId)),
        m_startDate(std::forward<TStartDate>(startDate)),
        m_price(std::forward<TEntryPrice>(price))
    {

    }

    IdType priceRangeId() const;
    DateTime startDate() const;
    Price price() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_priceRangeId;
    DateTime m_startDate;
    Price m_price;
};
