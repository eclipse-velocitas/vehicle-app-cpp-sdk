#ifndef VEHICLE_CABIN_SEAT_ROW_2_POS_3_SWITCH__SWITCH_H
#define VEHICLE_CABIN_SEAT_ROW_2_POS_3_SWITCH__SWITCH_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/cabin/seat/row_2/pos_3/switch_/backrest/Backrest.hpp"
#include "vehicle/cabin/seat/row_2/pos_3/switch_/headrest/Headrest.hpp"
#include "vehicle/cabin/seat/row_2/pos_3/switch_/massage/Massage.hpp"
#include "vehicle/cabin/seat/row_2/pos_3/switch_/seating/Seating.hpp"

#include <string>

namespace vehicle::cabin::seat::row_2::pos_3::switch_ {
using ParentClass = velocitas::Model;

/** Switch model. */
class Switch : public ParentClass {
public:

    Switch(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Backrest("Backrest", this),
		Headrest("Headrest", this),
		IsBackwardEngaged("IsBackwardEngaged", this),
		IsCoolerEngaged("IsCoolerEngaged", this),
		IsDownEngaged("IsDownEngaged", this),
		IsForwardEngaged("IsForwardEngaged", this),
		IsTiltBackwardEngaged("IsTiltBackwardEngaged", this),
		IsTiltForwardEngaged("IsTiltForwardEngaged", this),
		IsUpEngaged("IsUpEngaged", this),
		IsWarmerEngaged("IsWarmerEngaged", this),
		Massage("Massage", this),
		Seating("Seating", this)
    {}

    /**
    * Backrest: branch
    * Describes switches related to the backrest of the seat.
    *
    **/
    vehicle::cabin::seat::row_2::pos_3::switch_::backrest::Backrest Backrest;

    /**
    * Headrest: branch
    * Switches for SingleSeat.Headrest.
    *
    **/
    vehicle::cabin::seat::row_2::pos_3::switch_::headrest::Headrest Headrest;

    /**
    * IsBackwardEngaged: actuator
    * Seat backward switch engaged (SingleSeat.Position).
    *
    **/
    velocitas::DataPointBoolean IsBackwardEngaged;

    /**
    * IsCoolerEngaged: actuator
    * Cooler switch for Seat heater (SingleSeat.Heating).
    *
    **/
    velocitas::DataPointBoolean IsCoolerEngaged;

    /**
    * IsDownEngaged: actuator
    * Seat down switch engaged (SingleSeat.Height).
    *
    **/
    velocitas::DataPointBoolean IsDownEngaged;

    /**
    * IsForwardEngaged: actuator
    * Seat forward switch engaged (SingleSeat.Position).
    *
    **/
    velocitas::DataPointBoolean IsForwardEngaged;

    /**
    * IsTiltBackwardEngaged: actuator
    * Tilt backward switch engaged (SingleSeat.Tilt).
    *
    **/
    velocitas::DataPointBoolean IsTiltBackwardEngaged;

    /**
    * IsTiltForwardEngaged: actuator
    * Tilt forward switch engaged (SingleSeat.Tilt).
    *
    **/
    velocitas::DataPointBoolean IsTiltForwardEngaged;

    /**
    * IsUpEngaged: actuator
    * Seat up switch engaged (SingleSeat.Height).
    *
    **/
    velocitas::DataPointBoolean IsUpEngaged;

    /**
    * IsWarmerEngaged: actuator
    * Warmer switch for Seat heater (SingleSeat.Heating).
    *
    **/
    velocitas::DataPointBoolean IsWarmerEngaged;

    /**
    * Massage: branch
    * Switches for SingleSeat.Massage.
    *
    **/
    vehicle::cabin::seat::row_2::pos_3::switch_::massage::Massage Massage;

    /**
    * Seating: branch
    * Describes switches related to the seating of the seat.
    *
    **/
    vehicle::cabin::seat::row_2::pos_3::switch_::seating::Seating Seating;

};

} // namespace vehicle::cabin::seat::row_2::pos_3::switch_

#endif // VEHICLE_CABIN_SEAT_ROW_2_POS_3_SWITCH__SWITCH_H
