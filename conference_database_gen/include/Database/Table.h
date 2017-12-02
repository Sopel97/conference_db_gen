#pragma once

#include <vector>
#include <algorithm>

#include "Csv/CsvDocument.h"

template <class T>
class Table
{
public:
    using RecordType = T;
    using ConstIterator = typename std::vector<RecordType>::const_iterator;

    Table() = default;

    template <class InputIt>
    Table(InputIt first, InputIt last) :
        m_records(first, last)
    {

    }

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

    ConstIterator begin() const
    {
        return m_records.begin();
    }
    ConstIterator end() const
    {
        return m_records.end();
    }
    ConstIterator cbegin() const
    {
        return m_records.cbegin();
    }
    ConstIterator cend() const
    {
        return m_records.cend();
    }

    template <class TResult, class TValue>
    int findFirstIndexOrdered(TResult(RecordType::*Getter)() const, const TValue& value) const
    {
        auto iter = std::lower_bound(
            m_records.begin(),
            m_records.end(),
            value,
            [Getter](const RecordType& lhs, const TValue& rhs) {
            return (lhs.*Getter)() < rhs;
        }
        );

        if (iter != m_records.end() && ((*iter).*Getter)() == value)
        {
            return std::distance(m_records.begin(), iter);
        }

        return -1;
    }

    template <class TResult, class TValue>
    bool existsOrdered(TResult(RecordType::*Getter)() const, const TValue& value) const
    {
        auto iter = std::lower_bound(
            m_records.begin(),
            m_records.end(),
            value,
            [Getter](const RecordType& lhs, const TValue& rhs) {
                return (lhs.*Getter)() < rhs;
            }
        );

        return iter != m_records.end() && ((*iter).*Getter)() == value;
    }

    template <class TResult, class TValue>
    Table<RecordType> queryOrdered(TResult(RecordType::*Getter)() const, const TValue& value) const
    {
        auto lower = std::lower_bound(
            m_records.begin(),
            m_records.end(),
            value,
            [Getter](const RecordType& lhs, const TValue& rhs) {
            return (lhs.*Getter)() < rhs;
        }
        );

        auto upper = std::upper_bound(
            m_records.begin(),
            m_records.end(),
            value,
            [Getter](const RecordType& lhs, const TValue& rhs) {
            return (lhs.*Getter)() < rhs;
        }
        );

        return Table<RecordType>(lower, upper);
    }
private:
    std::vector<RecordType> m_records;
};
