#pragma once

#include <tuple>
#include <utility>

#include "Csv/CsvDocument.h"

#include "Table.h"

template <class... TTables>
class Database
{
protected:
    using StorageType = std::tuple<Table<TTables>...>;
    static constexpr size_t numTables = sizeof...(TTables);
    using CsvArrayType = std::array<std::pair<std::string, CsvDocument>, numTables>;
    using NameArrayType = std::array<std::string, numTables>;

public:
    Database() = default;

    template <class T>
    const Table<T>& table() const
    {
        return std::get<Table<T>>(m_tables);
    }

    template <class T>
    void add(T&& record)
    {
        table<std::decay_t<T>>().add(std::forward<T>(record));
    }

    virtual const NameArrayType& tableNames() const = 0;

    virtual CsvArrayType toCsvArray() const
    {
        CsvArrayType csvs;
        const auto& names = tableNames();

        toCsvArrayHelper(csvs, names, std::index_sequence_for<TTables...>());

        return csvs;
    }

protected:
    StorageType m_tables;

    Database(StorageType&& tables) :
        m_tables(std::move(tables))
    {

    }

    template <class T>
    Table<T>& table()
    {
        return std::get<Table<T>>(m_tables);
    }

private:
    template <size_t... Inds>
    void toCsvArrayHelper(CsvArrayType& csvs, const NameArrayType& names, std::index_sequence<Inds...>) const
    {
        int unused[] = { 0, (void(csvs[Inds] = std::make_pair(names[Inds], std::get<Inds>(m_tables).toCsvDocument())), 0)... };
        (void)unused;
    }
};
