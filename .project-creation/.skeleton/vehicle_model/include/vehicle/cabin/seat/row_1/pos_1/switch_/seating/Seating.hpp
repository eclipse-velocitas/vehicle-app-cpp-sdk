#ifndef VEHICLE_CABIN_SEAT_ROW_1_POS_1_SWITCH__SEATING_SEATING_H
#define VEHICLE_CABIN_SEAT_ROW_1_POS_1_SWITCH__SEATING_SEATING_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::cabin::seat::row_1::pos_1::switch_::seating {
using ParentClass = velocitas::Model;

/** Seating model. */
class Seating : public ParentClass {
public:

    Seating(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsBackwardEngaged("IsBackwardEngaged", this),
		IsForwardEngaged("IsForwardEngaged", this)
    {}

    /**
    * IsBackwardEngaged: actuator
    * Is switch to decrease seating length engaged (SingleSeat.Seating.Length).
    *
    **/
    velocitas::DataPointBoolean IsBackwardEngaged;

    /**
    * IsForwardEngaged: actuator
    * Is switch to increase seating length engaged (SingleSeat.Seating.Length).
    *
    **/
    velocitas::DataPointBoolean IsForwardEngaged;

};

} // namespace vehicle::cabin::seat::row_1::pos_1::switch_::seating

#endif // VEHICLE_CABIN_SEAT_ROW_1_POS_1_SWITCH__SEATING_SEATING_H
