#pragma once

#include <string>

#include "Database/Record.h"
#include "Database/ForeignKey.h"

#include "DateTime.h"
#include "Price.h"

#include "ConferenceDay.h"

class Workshop : public Record
{
public:
    using PrimaryKeyType = IdType;

    template <
        class TWorkshopId, 
        class TConferenceDay, 
        class TName,
        class TStartDate, 
        class TEndDate, 
        class TNumSpots, 
        class TEntryPrice
    >
    Workshop(
        TWorkshopId&& workshopId, 
        TConferenceDay&& conferenceDay, 
        TName&& name,
        TStartDate&& startDate, 
        TEndDate&& endDate, 
        TNumSpots&& numSpots, 
        TEntryPrice&& entryPrice
    ) :
        m_workshopId(std::forward<TWorkshopId>(workshopId)),
        m_conferenceDay(std::forward<TConferenceDay>(conferenceDay)),
        m_name(std::forward<TName>(name)),
        m_startDate(std::forward<TStartDate>(startDate)),
        m_endDate(std::forward<TEndDate>(endDate)),
        m_numSpots(std::forward<TNumSpots>(numSpots)),
        m_entryPrice(std::forward<TEntryPrice>(entryPrice))
    {

    }

    PrimaryKeyType primaryKey() const;

    IdType workshopId() const;
    ForeignKey<ConferenceDay> conferenceDay() const;
    const std::string& name() const;
    const DateTime& startDate() const;
    const DateTime& endDate() const;
    int numSpots() const;
    Price entryPrice() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_workshopId;
    ForeignKey<ConferenceDay> m_conferenceDay;
    std::string m_name;
    DateTime m_startDate;
    DateTime m_endDate;
    int m_numSpots;
    Price m_entryPrice;
};
