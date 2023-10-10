#ifndef VEHICLE_CABIN_SEAT_ROW_2_POS_1_BACKREST_BACKREST_H
#define VEHICLE_CABIN_SEAT_ROW_2_POS_1_BACKREST_BACKREST_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/cabin/seat/row_2/pos_1/backrest/lumbar/Lumbar.hpp"
#include "vehicle/cabin/seat/row_2/pos_1/backrest/side_bolster/SideBolster.hpp"

#include <string>

namespace vehicle::cabin::seat::row_2::pos_1::backrest {
using ParentClass = velocitas::Model;

/** Backrest model. */
class Backrest : public ParentClass {
public:

    Backrest(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Lumbar("Lumbar", this),
		Recline("Recline", this),
		SideBolster("SideBolster", this)
    {}

    /**
    * Lumbar: branch
    * Adjustable lumbar support mechanisms in seats allow the user to change the seat back shape.
    *
    **/
    vehicle::cabin::seat::row_2::pos_1::backrest::lumbar::Lumbar Lumbar;

    /**
    * Recline: actuator
    * Backrest recline compared to seat z-axis (seat vertical axis). 0 degrees = Upright/Vertical backrest. Negative degrees for forward recline. Positive degrees for backward recline.
    *
    * Seat z-axis depends on seat tilt. This means that movement of backrest due to seat tilting will not affect Backrest.Recline as long as the angle between Seating and Backrest are constant. Absolute recline relative to vehicle z-axis can be calculated as Tilt + Backrest.Recline.
    *
    * Unit: degrees
    **/
    velocitas::DataPointFloat Recline;

    /**
    * SideBolster: branch
    * Backrest side bolster (lumbar side support) settings.
    *
    **/
    vehicle::cabin::seat::row_2::pos_1::backrest::side_bolster::SideBolster SideBolster;

};

} // namespace vehicle::cabin::seat::row_2::pos_1::backrest

#endif // VEHICLE_CABIN_SEAT_ROW_2_POS_1_BACKREST_BACKREST_H
