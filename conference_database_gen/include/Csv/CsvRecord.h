#pragma once

#include <vector>
#include <string>
#include <numeric>
#include <ostream>

class CsvRecord
{
public:
    template <class... Args>
    CsvRecord(Args&&... args)
        : m_columns{std::forward<Args>(args)...}
    {

    }

    CsvRecord() = default;

    void add(const std::string& string)
    {
        m_columns.emplace_back(string);
    }
    void add(std::string&& string)
    {
        m_columns.emplace_back(std::move(string));
    }

    template <class T>
    void add(T&& val)
    {
        using std::to_string;

        m_columns.emplace_back(to_string(std::forward<T>(val)));
    }

    // TODO: replace " to ""
    std::string toString() const
    {
        std::string buffer;

        bool isFirst = true;
        for (const auto& str : m_columns)
        {
            if (!isFirst)
            {
                buffer += ',';
            }
            isFirst = false;

            buffer += '\"';
            buffer += str;
            buffer += '\"';
        }

        return buffer;
    }

    friend std::ostream& operator<< (std::ostream& stream, const CsvRecord& record)
    {
        bool isFirst = true;
        for (const auto& str : record.m_columns)
        {
            if (!isFirst)
            {
                stream << ',';
            }
            isFirst = false;

            stream << '\"';
            stream << str;
            stream << '\"';
        }

        return stream;
    }

    int numColumns() const
    {
        return m_columns.size();
    }
private:
    std::vector<std::string> m_columns;

    int totalLength() const
    {
        return std::accumulate(m_columns.begin(), m_columns.end(), 0, [](int acc, const std::string& str) {return acc + str.size() + 2; }) + m_columns.size() - 1;
    }
};
