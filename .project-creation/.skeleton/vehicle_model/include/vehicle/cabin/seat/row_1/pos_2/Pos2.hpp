#ifndef VEHICLE_CABIN_SEAT_ROW_1_POS_2_POS2_H
#define VEHICLE_CABIN_SEAT_ROW_1_POS_2_POS2_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/cabin/seat/row_1/pos_2/airbag/Airbag.hpp"
#include "vehicle/cabin/seat/row_1/pos_2/backrest/Backrest.hpp"
#include "vehicle/cabin/seat/row_1/pos_2/headrest/Headrest.hpp"
#include "vehicle/cabin/seat/row_1/pos_2/occupant/Occupant.hpp"
#include "vehicle/cabin/seat/row_1/pos_2/seating/Seating.hpp"
#include "vehicle/cabin/seat/row_1/pos_2/switch_/Switch.hpp"

#include <string>

namespace vehicle::cabin::seat::row_1::pos_2 {
using ParentClass = velocitas::Model;

/** Pos2 model. */
class Pos2 : public ParentClass {
public:

    Pos2(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Airbag("Airbag", this),
		Backrest("Backrest", this),
		Headrest("Headrest", this),
		Heating("Heating", this),
		Height("Height", this),
		IsBelted("IsBelted", this),
		IsOccupied("IsOccupied", this),
		Massage("Massage", this),
		Occupant("Occupant", this),
		Position("Position", this),
		Seating("Seating", this),
		Switch("Switch", this),
		Tilt("Tilt", this)
    {}

    /**
    * Airbag: branch
    * Airbag signals.
    *
    **/
    vehicle::cabin::seat::row_1::pos_2::airbag::Airbag Airbag;

    /**
    * Backrest: branch
    * Describes signals related to the backrest of the seat.
    *
    **/
    vehicle::cabin::seat::row_1::pos_2::backrest::Backrest Backrest;

    /**
    * Headrest: branch
    * Headrest settings.
    *
    **/
    vehicle::cabin::seat::row_1::pos_2::headrest::Headrest Headrest;

    /**
    * Heating: actuator
    * Seat cooling / heating. 0 = off. -100 = max cold. +100 = max heat.
    *
    * Value range: [-100, 100]
    * Unit: percent
    **/
    velocitas::DataPointInt32 Heating;

    /**
    * Height: actuator
    * Seat position on vehicle z-axis. Position is relative within available movable range of the seating. 0 = Lowermost position supported.
    *
    * Value range: [0, ]
    * Unit: mm
    **/
    velocitas::DataPointUint32 Height;

    /**
    * IsBelted: sensor
    * Is the belt engaged.
    *
    **/
    velocitas::DataPointBoolean IsBelted;

    /**
    * IsOccupied: sensor
    * Does the seat have a passenger in it.
    *
    **/
    velocitas::DataPointBoolean IsOccupied;

    /**
    * Massage: actuator
    * Seat massage level. 0 = off. 100 = max massage.
    *
    * Value range: [0, 100]
    * Unit: percent
    **/
    velocitas::DataPointUint32 Massage;

    /**
    * Occupant: branch
    * Occupant data.
    *
    **/
    vehicle::cabin::seat::row_1::pos_2::occupant::Occupant Occupant;

    /**
    * Position: actuator
    * Seat position on vehicle x-axis. Position is relative to the frontmost position supported by the seat. 0 = Frontmost position supported.
    *
    * Value range: [0, ]
    * Unit: mm
    **/
    velocitas::DataPointUint32 Position;

    /**
    * Seating: branch
    * Describes signals related to the seating/base of the seat.
    *
    * Seating is here considered as the part of the seat that supports the thighs. Additional cushions (if any) for support of lower legs is not covered by this branch.
    *
    **/
    vehicle::cabin::seat::row_1::pos_2::seating::Seating Seating;

    /**
    * Switch: branch
    * Seat switch signals
    *
    **/
    vehicle::cabin::seat::row_1::pos_2::switch_::Switch Switch;

    /**
    * Tilt: actuator
    * Tilting of seat relative to vehicle z-axis. 0 = seating is flat, seat and vehicle z-axis are parallel. Positive degrees = seat tilted backwards, seat z-axis is tilted backward.
    *
    * Unit: degrees
    **/
    velocitas::DataPointFloat Tilt;

};

} // namespace vehicle::cabin::seat::row_1::pos_2

#endif // VEHICLE_CABIN_SEAT_ROW_1_POS_2_POS2_H
