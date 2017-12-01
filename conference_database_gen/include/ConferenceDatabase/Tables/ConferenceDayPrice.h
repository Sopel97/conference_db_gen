#pragma once

#include <string>

#include "Database/Record.h"

class ConferenceDayPrice : public Record
{
public:
    template <class TConferenceId, class TPriceRangeId>
    ConferenceDayPrice(TConferenceId&& conferenceId, TPriceRangeId&& priceRangeId) :
        m_conferenceId(std::forward<TConferenceId>(conferenceId)),
        m_priceRangeId(std::forward<TPriceRangeId>(priceRangeId))
    {

    }

    IdType conferenceId() const;
    IdType priceRangeId() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_conferenceId;
    IdType m_priceRangeId;
};
