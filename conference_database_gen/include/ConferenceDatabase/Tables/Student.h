#pragma once

#include <string>

#include "Database/Record.h"
#include "Database/ForeignKey.h"

#include "Person.h"

class Student : public Record
{
public:
    using PrimaryKeyType = IdType;

    template <class TStudentId, class TPerson, class TCardNumber>
    Student(TStudentId&& studentId, TPerson&& person, TCardNumber&& cardNumber) :
        m_studentId(std::forward<TStudentId>(studentId)),
        m_person(std::forward<TPerson>(person)),
        m_cardNumber(std::forward<TCardNumber>(cardNumber))
    {

    }

    Student(const Student&) = default;
    Student(Student&&) noexcept = default;

    Student& operator=(const Student&) = default;
    Student& operator=(Student&&) noexcept = default;

    PrimaryKeyType primaryKey() const;

    IdType studentId() const;
    ForeignKey<Person> person() const;
    const std::string& cardNumber() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_studentId;
    ForeignKey<Person> m_person;
    std::string m_cardNumber;
};
