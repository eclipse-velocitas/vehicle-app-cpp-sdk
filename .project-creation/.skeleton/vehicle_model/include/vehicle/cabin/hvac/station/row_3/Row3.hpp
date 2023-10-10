#ifndef VEHICLE_CABIN_HVAC_STATION_ROW_3_ROW3_H
#define VEHICLE_CABIN_HVAC_STATION_ROW_3_ROW3_H

#include "sdk/DataPoint.h"
#include "sdk/Model.h"

#include "vehicle/cabin/hvac/station/row_3/left/Left.hpp"
#include "vehicle/cabin/hvac/station/row_3/right/Right.hpp"

#include <string>

namespace vehicle::cabin::hvac::station::row_3 {
using ParentClass = velocitas::Model;

/** Row3 model. */
class Row3 : public ParentClass {
public:

    Row3(const std::string& name, ParentClass* parent) :
        ParentClass(name, parent),
		Left("Left", this),
		Right("Right", this)
    {}

    /**
    * Left: branch
    * HVAC for single station in the vehicle
    *
    **/
    vehicle::cabin::hvac::station::row_3::left::Left Left;

    /**
    * Right: branch
    * HVAC for single station in the vehicle
    *
    **/
    vehicle::cabin::hvac::station::row_3::right::Right Right;

};

} // namespace vehicle::cabin::hvac::station::row_3

#endif // VEHICLE_CABIN_HVAC_STATION_ROW_3_ROW3_H
