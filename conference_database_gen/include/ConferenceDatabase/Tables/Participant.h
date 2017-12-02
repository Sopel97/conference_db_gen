#pragma once

#include <string>

#include "Database/Record.h"
#include "Database/ForeignKey.h"

#include "Person.h"

class Participant : public Record
{
public:
    using PrimaryKeyType = IdType;

    template <class TParticipantId, class TPerson>
    Participant(TParticipantId&& participantId, TPerson&& person) :
        m_participantId(std::forward<TParticipantId>(participantId)),
        m_person(std::forward<TPerson>(person))
    {

    }

    PrimaryKeyType primaryKey() const;

    IdType participantId() const;
    ForeignKey<Person> person() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_participantId;
    ForeignKey<Person> m_person;
};
