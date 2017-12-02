#include "Generators/WorkshopReservationTableGenerator.h"

TableGenerator<WorkshopReservation>::TableGenerator(
    const Table<ConferenceDayReservation>& conferenceDayReservations,
    const Table<ConferenceDay>& conferenceDays,
    const Table<Workshop>& workshops,
    float paymentSaturation
) :
    m_conferenceDayReservations(&conferenceDayReservations),
    m_conferenceDays(&conferenceDays),
    m_workshops(&workshops),
    m_paymentSaturation(paymentSaturation)
{

}
