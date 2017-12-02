#include "Generators/ConferenceDayReservationTableGenerator.h"

TableGenerator<ConferenceDayReservation>::TableGenerator(
    const Table<Participant>& participants,
    const Table<ConferenceDay>& conferenceDays,
    const Table<PriceRange>& priceRanges,
    const Table<Student>& students,
    float paymentSaturation,
    Milliseconds priceRangeDuration // price ranges only hold starting date, this may need to be changed in the future
) :
    m_participants(&participants),
    m_conferenceDays(&conferenceDays),
    m_priceRanges(&priceRanges),
    m_students(&students),
    m_paymentSaturation(paymentSaturation),
    m_priceRangeDuration(priceRangeDuration)
{

}