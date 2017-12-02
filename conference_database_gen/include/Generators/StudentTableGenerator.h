#pragma once

#include <vector>

#include "ConferenceDatabase/Tables/Student.h"
#include "ConferenceDatabase/Tables/Person.h"
#include "Database/Table.h"

#include "TableGenerator.h"

#include "MarkovChainsDictionary.h"
#include "NameGenerator.h"
#include "Common.h"

template <>
class TableGenerator<Student>
{
public:
    using RecordType = Student;
    using ResultType = Table<Student>;

    TableGenerator(
        const Table<Person>& people,
        float studentSaturation
    ) :
        m_people(&people),
        m_studentSaturation(studentSaturation)
    {

    }

    template <class TRng>
    Table<Student> operator()(TRng& rng) const
    {
        std::bernoulli_distribution dIsStudent(m_studentSaturation);
        std::uniform_int_distribution<int> dStudentCardNumber(100000, 999999);
        Table<Student> students;

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
