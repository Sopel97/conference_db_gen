#include "Csv/CsvDocument.h"

void CsvDocument::add(const CsvRecord& record)
{
    m_records.emplace_back(record);
}

void CsvDocument::add(CsvRecord&& record)
{
    m_records.emplace_back(std::move(record));
}

std::string CsvDocument::toString() const
{
    std::string buffer;

    bool isFirst = true;
    for (const auto& record : m_records)
    {
        if (!isFirst)
        {
            buffer += '\n';
        }
        isFirst = true;

        buffer += record.toString();
    }

    return buffer;
}

std::ostream& operator<< (std::ostream& stream, const CsvDocument& doc)
{
    bool isFirst = true;
    for (const auto& record : doc.m_records)
    {
        if (!isFirst)
        {
            stream << '\n';
        }
        isFirst = false;

        stream << record;
    }

    return stream;
}
