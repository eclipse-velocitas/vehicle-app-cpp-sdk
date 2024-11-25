#ifndef VEHICLE_CABIN_HVAC_STATION_ROW_2_ROW2_H
#define VEHICLE_CABIN_HVAC_STATION_ROW_2_ROW2_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/cabin/hvac/station/row_2/left/Left.hpp"
#include "vehicle/cabin/hvac/station/row_2/right/Right.hpp"

#include <string>

namespace vehicle::cabin::hvac::station::row_2 {
using ParentClass = velocitas::Model;

/** Row2 model. */
class Row2 : public ParentClass {
public:

    Row2(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Left("Left", this),
		Right("Right", this)
    {}

    /**
    * Left: branch
    * HVAC for single station in the vehicle
    *
    **/
    vehicle::cabin::hvac::station::row_2::left::Left Left;

    /**
    * Right: branch
    * HVAC for single station in the vehicle
    *
    **/
    vehicle::cabin::hvac::station::row_2::right::Right Right;

};

} // namespace vehicle::cabin::hvac::station::row_2

#endif // VEHICLE_CABIN_HVAC_STATION_ROW_2_ROW2_H
