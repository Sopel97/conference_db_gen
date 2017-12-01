#pragma once

#include <vector>

#include "CsvRecord.h"

class CsvDocument
{
public:
    void add(const CsvRecord& record);
    void add(CsvRecord&& record);

    std::string toString() const;

    friend std::ostream& operator<< (std::ostream& stream, const CsvDocument& doc);

private:
    std::vector<CsvRecord> m_records;
};
