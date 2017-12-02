#pragma once

#include <string>
#include <optional>

#include "Database/Record.h"
#include "Database/ForeignKey.h"

#include "Person.h"
#include "Company.h"

class Participant : public Record
{
public:
    using PrimaryKeyType = IdType;

    template <class TParticipantId, class TPerson, class TCompany>
    Participant(TParticipantId&& participantId, TPerson&& person, TCompany&& company) :
        m_participantId(std::forward<TParticipantId>(participantId)),
        m_person(std::forward<TPerson>(person)),
        m_company(std::forward<TCompany>(company))
    {

    }

    PrimaryKeyType primaryKey() const;

    IdType participantId() const;
    ForeignKey<Person> person() const;
    std::optional<ForeignKey<Company>> company() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_participantId;
    ForeignKey<Person> m_person;
    std::optional<ForeignKey<Company>> m_company;
};
