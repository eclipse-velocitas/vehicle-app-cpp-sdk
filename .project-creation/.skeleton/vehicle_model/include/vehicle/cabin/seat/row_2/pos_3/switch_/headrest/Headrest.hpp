#ifndef VEHICLE_CABIN_SEAT_ROW_2_POS_3_SWITCH__HEADREST_HEADREST_H
#define VEHICLE_CABIN_SEAT_ROW_2_POS_3_SWITCH__HEADREST_HEADREST_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include <string>

namespace vehicle::cabin::seat::row_2::pos_3::switch_::headrest {
using ParentClass = velocitas::Model;

/** Headrest model. */
class Headrest : public ParentClass {
public:

    Headrest(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsBackwardEngaged("IsBackwardEngaged", this),
		IsDownEngaged("IsDownEngaged", this),
		IsForwardEngaged("IsForwardEngaged", this),
		IsUpEngaged("IsUpEngaged", this)
    {}

    /**
    * IsBackwardEngaged: actuator
    * Head rest backward switch engaged (SingleSeat.Headrest.Angle).
    *
    **/
    velocitas::DataPointBoolean IsBackwardEngaged;

    /**
    * IsDownEngaged: actuator
    * Head rest down switch engaged (SingleSeat.Headrest.Height).
    *
    **/
    velocitas::DataPointBoolean IsDownEngaged;

    /**
    * IsForwardEngaged: actuator
    * Head rest forward switch engaged (SingleSeat.Headrest.Angle).
    *
    **/
    velocitas::DataPointBoolean IsForwardEngaged;

    /**
    * IsUpEngaged: actuator
    * Head rest up switch engaged (SingleSeat.Headrest.Height).
    *
    **/
    velocitas::DataPointBoolean IsUpEngaged;

};

} // namespace vehicle::cabin::seat::row_2::pos_3::switch_::headrest

#endif // VEHICLE_CABIN_SEAT_ROW_2_POS_3_SWITCH__HEADREST_HEADREST_H
