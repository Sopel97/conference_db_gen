#pragma once

#include <string>

template <class T>
class ForeignKey
{
public:
    using RecordType = T;
    using PrimaryKeyType = typename T::PrimaryKeyType;

    ForeignKey(const T& record) :
        m_reference(&record)
    {

    }

    bool operator==(const ForeignKey<T>& rhs) const
    {
        return m_reference == rhs.m_reference;
    }

    bool operator!=(const ForeignKey<T>& rhs) const
    {
        return !operator==(rhs);
    }

    bool operator<(const ForeignKey<T>& rhs) const
    {
        return primaryKey() < rhs.primaryKey();
    }

    std::string toString() const
    {
        return std::to_string(m_reference->primaryKey());
    }

    const T* operator->() const
    {
        return m_reference;
    }

    const T& operator*() const
    {
        return *m_reference;
    }

    decltype(auto) primaryKey() const
    {
        return m_reference->primaryKey();
    }

private:
    const T* m_reference;
};
