#pragma once

#include <string>

#include "Database/Record.h"

class Participant : public Record
{
public:
    template <class TParticipantId, class TPersonId>
    Participant(TParticipantId&& participantId, TPersonId&& personId) :
        m_participantId(std::forward<TParticipantId>(participantId)),
        m_personId(std::forward<TPersonId>(personId))
    {

    }

    IdType participantId() const;
    IdType personId() const;

    CsvRecord toCsvRecord() const override;

private:
    IdType m_participantId;
    IdType m_personId;
};
