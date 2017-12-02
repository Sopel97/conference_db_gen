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

    void add(const std::string& string);
    void add(std::string&& string);

    template <class T>
    void add(T&& val)
    {
        using std::to_string;

        m_columns.emplace_back(to_string(std::forward<T>(val)));
    }

    std::string toString() const;

    friend std::ostream& operator<< (std::ostream& stream, const CsvRecord& record);

    int numColumns() const;
private:
    std::vector<std::string> m_columns;

    int totalLength() const;
};
