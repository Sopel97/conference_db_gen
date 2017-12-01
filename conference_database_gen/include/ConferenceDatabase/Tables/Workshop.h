#pragma once

#include <string>

#include "Database/Record.h"
#include "DateTime.h"
#include "Price.h"

class Workshop : public Record
{
public:
    template <
        class TWorkshopId, 
        class TConferenceDayId, 
        class TStartDate, 
        class TEndDate, 
        class TNumSpots, 
        class TEntryPrice
    >
    Workshop(
        TWorkshopId&& workshopId, 
        TConferenceDayId&& conferenceDayId, 
        TStartDate&& startDate, 
        TEndDate&& endDate, 
        TNumSpots&& numSpots, 
        TEntryPrice&& entryPrice
    ) :
        m_workshopId(std::forward<TWorkshopId>(workshopId)),
        m_conferenceDayId(std::forward<TConferenceDayId>(conferenceDayId)),
        m_startDate(std::forward<TStartDate>(startDate)),
        m_endDate(std::forward<TEndDate>(endDate)),
        m_numSpots(std::forward<TNumSpots>(numSpots)),
        m_entryPrice(std::forward<TEntryPrice>(entryPrice))
    {

    }

    IdType workshopId() const;
    IdType conferenceDayId() const;
    const DateTime& startDate() const;
    const DateTime& endDate() const;
    int numSpots() const;
    Price entryPrice() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_workshopId;
    IdType m_conferenceDayId;
    DateTime m_startDate;
    DateTime m_endDate;
    int m_numSpots;
    Price m_entryPrice;
};
