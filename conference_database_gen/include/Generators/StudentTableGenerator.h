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

    TableGenerator(
        const Table<Person>& people,
        float studentSaturation
    );

    template <class TRng>
    Table<Student> operator()(TRng& rng) const
    {
        static constexpr int minStudentCardNumber = 100000;
        static constexpr int maxStudentCardNumber = 999999;

        std::bernoulli_distribution dIsStudent(m_studentSaturation);
        std::uniform_int_distribution<int> dStudentCardNumber(minStudentCardNumber, maxStudentCardNumber);
        Table<Student> students;
        {
            float f = 1.0f - m_studentSaturation;
            f *= f; // since we're dealing with probability reserving is done with a margin
            students.reserve(static_cast<int>(m_people->size() * (1.0f - f)));
        }

        Record::IdType id = 0;
        for (const auto& person : *m_people)
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
    const Table<Person>* m_people;
    float m_studentSaturation;
};
