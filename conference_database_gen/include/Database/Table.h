#pragma once

#include <vector>

#include "Csv/CsvDocument.h"

template <class T>  
class Table
{
public:
    using RecordType = T;

    void add(const T& record)
    {
        m_records.emplace_back(record);
    }
    void add(T&& record)
    {
        m_records.emplace_back(std::move(record));
    }

    const std::vector<RecordType>& records() const
    {
        return m_records;
    }

    CsvDocument toCsvDocument() const
    {
        CsvDocument doc;
        for (const auto& record : m_records)
        {
            doc.add(record.toCsvRecord());
        }
        return doc;
    }

    const RecordType& operator[](int i) const
    {
        return m_records[i];
    }

    int size() const
    {
        return m_records.size();
    }

private:
    std::vector<RecordType> m_records;
};
