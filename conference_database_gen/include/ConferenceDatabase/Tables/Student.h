#pragma once

#include <string>

#include "Database/Record.h"

class Student : public Record
{
public:
    template <class TStudentId, class TPersonId, class TCardNumber>
    Student(TStudentId&& studentId, TPersonId&& personId, TCardNumber&& cardNumber) :
        m_studentId(std::forward<TStudentId>(studentId)),
        m_personId(std::forward<TPersonId>(personId)),
        m_cardNumber(std::forward<TCardNumber>(cardNumber))
    {

    }

    IdType studentId() const;
    IdType personId() const;
    const std::string& cardNumber() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_studentId;
    IdType m_personId;
    std::string m_cardNumber;
};
