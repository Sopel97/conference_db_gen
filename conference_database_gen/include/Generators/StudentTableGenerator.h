#pragma once

#include <vector>

#include "ConferenceDatabase/Tables/Student.h"
#include "ConferenceDatabase/Tables/Person.h"
#include "Database/Table.h"

#include "TableGenerator.h"

template <>
class TableGenerator<Student>
{
public:
    using RecordType = Student;
    using ResultType = Table<Student>;

    struct Params
    {
        const Table<Person>* people;
        float studentSaturation;
        int minStudentCardNumber;
        int maxStudentCardNumber;
    };

    TableGenerator(const Params& params);

    template <class TRng>
    Table<Student> operator()(TRng& rng) const
    {
        std::bernoulli_distribution dIsStudent(m_params.studentSaturation);
        std::uniform_int_distribution<int> dStudentCardNumber(m_params.minStudentCardNumber, m_params.maxStudentCardNumber);
        Table<Student> students;
        {
            float f = 1.0f - m_params.studentSaturation;
            f *= f; // since we're dealing with probability reserving is done with a margin
            students.reserve(static_cast<int>(m_params.people->size() * (1.0f - f)));
        }

        Record::IdType id = 0;
        for (const auto& person : *m_params.people)
        {
            if (!dIsStudent(rng)) continue;

            students.add(
                Student(
                    id++,
                    person,
                    std::to_string(dStudentCardNumber(rng))
                )
            );
        }

        return students;
    }

private:
    Params m_params;
};
