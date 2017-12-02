#include "Csv/CsvRecord.h"

void CsvRecord::add(const std::string& string)
{
    m_columns.emplace_back(string);
}
void CsvRecord::add(std::string&& string)
{
    m_columns.emplace_back(std::move(string));
}

// TODO: replace " to ""
std::string CsvRecord::toString() const
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

std::ostream& operator<< (std::ostream& stream, const CsvRecord& record)
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

int CsvRecord::numColumns() const
{
    return m_columns.size();
}

int CsvRecord::totalLength() const
{
    return std::accumulate(m_columns.begin(), m_columns.end(), 0, [](int acc, const std::string& str) {return acc + str.size() + 2; }) + m_columns.size() - 1;
}
