#ifndef VEHICLE_CABIN_SEAT_ROW_2_POS_2_SWITCH__BACKREST_BACKREST_H
#define VEHICLE_CABIN_SEAT_ROW_2_POS_2_SWITCH__BACKREST_BACKREST_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/cabin/seat/row_2/pos_2/switch_/backrest/lumbar/Lumbar.hpp"
#include "vehicle/cabin/seat/row_2/pos_2/switch_/backrest/side_bolster/SideBolster.hpp"

#include <string>

namespace vehicle::cabin::seat::row_2::pos_2::switch_::backrest {
using ParentClass = velocitas::Model;

/** Backrest model. */
class Backrest : public ParentClass {
public:

    Backrest(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		IsReclineBackwardEngaged("IsReclineBackwardEngaged", this),
		IsReclineForwardEngaged("IsReclineForwardEngaged", this),
		Lumbar("Lumbar", this),
		SideBolster("SideBolster", this)
    {}

    /**
    * IsReclineBackwardEngaged: actuator
    * Backrest recline backward switch engaged (SingleSeat.Backrest.Recline).
    *
    **/
    velocitas::DataPointBoolean IsReclineBackwardEngaged;

    /**
    * IsReclineForwardEngaged: actuator
    * Backrest recline forward switch engaged (SingleSeat.Backrest.Recline).
    *
    **/
    velocitas::DataPointBoolean IsReclineForwardEngaged;

    /**
    * Lumbar: branch
    * Switches for SingleSeat.Backrest.Lumbar.
    *
    **/
    vehicle::cabin::seat::row_2::pos_2::switch_::backrest::lumbar::Lumbar Lumbar;

    /**
    * SideBolster: branch
    * Switches for SingleSeat.Backrest.SideBolster.
    *
    **/
    vehicle::cabin::seat::row_2::pos_2::switch_::backrest::side_bolster::SideBolster SideBolster;

};

} // namespace vehicle::cabin::seat::row_2::pos_2::switch_::backrest

#endif // VEHICLE_CABIN_SEAT_ROW_2_POS_2_SWITCH__BACKREST_BACKREST_H
