#pragma once

class CsvRecord;

class Record
{
public:
    using IdType = int;

    virtual CsvRecord toCsvRecord() const = 0;
protected:
};
